//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLfifoBuffer
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Thu 12/02/1999 
//
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLminimumInclude.h>
#include <COL/COLfifoBuffer.h>
#include <COL/COLmutex.h>
#include <COL/COLlocker.h>
#include <COL/COLlog.h>
COL_LOG_MODULE;

#include <memory.h>
#include <assert.h>

#undef IDEAL_MAX_BUFSZ
#define IDEAL_MAX_BUFSZ (16*1024)


class COLfifoBufferPrivate
{
public:
   COLfifoBufferPrivate(int InitialSize)
   {
      m_Capacity = InitialSize < minimumCapacity ? minimumCapacity : InitialSize;
      m_Reserved = 0;
      m_Start = 0;
      m_End = 0;
      m_Data = (COLuint8*)malloc(m_Capacity);
      COLMSGPRECONDITION(m_Data, "Out of memory.");
   }

   ~COLfifoBufferPrivate()
   {
      ::free(m_Data);
   }

   void bytesRead(int Size)
   {
      if (Size > 0)
      {
         COLPRECONDITION(m_Start + Size <= m_End);
         m_Start += Size;
         COLPRECONDITION(m_Start <= m_End);

         // If there's nothing left to read, and nothing being written,
         // we can shrink over-sized buffers.
         if(m_Start == m_End && !m_Reserved && m_Capacity > IDEAL_MAX_BUFSZ) {
            COLuint8* pNew = (COLuint8*)realloc(m_Data, IDEAL_MAX_BUFSZ);
            if(pNew) {
               m_Data = pNew;
               m_Capacity = IDEAL_MAX_BUFSZ;
               m_Start = m_End = 0;
            }
         }
      }
   }

   void bytesWritten(int Size)
   {
      if (Size > 0)
      {
         COLPRECONDITION(m_End + Size <= m_Capacity);
         m_End += Size;
         COLPRECONDITION(m_End <= m_Capacity);
         clearReserve(Size);
      }
   }

   int size() const
   {
      return m_End - m_Start;
   }

   void clear()
   {
      m_Start = 0;
      m_End = 0;
      // m_Capacity and m_Data remain unchanged.
   }

   const COLuint8* read(int Size)
   {
      if (Size >= 0)
      {
         if (Size <= m_End - m_Start)
         {
            return m_Data + m_Start;
         }
      }
      // return NULL if Size is invalid
      return 0;
   }

   COLuint8* reserveWrite(int Size)
   {
      //COLMSGPRECONDITION(!m_Reserved, "Space already reserved.");
      int FreeAtEnd = m_Capacity - m_End;
      if(Size <= FreeAtEnd) {
         /* Do nothing. */
      } else if(Size <= FreeAtEnd + m_Start) {
         compact();  // Shuffling things will do.
         COLASSERT(m_Capacity - m_End >= Size);
      } else {
         // must resize the internal buffer
         int NewCapacity = m_End - m_Start + Size;
         if(NewCapacity < m_Capacity * 2) {
            NewCapacity = m_Capacity * 2;
         }
         COLuint8* pNew = (COLuint8*)realloc(m_Data, NewCapacity);
         COLMSGPRECONDITION(pNew, "Out of memory.");
         m_Data = pNew;
         m_Capacity = NewCapacity;
         if(Size > m_Capacity - m_End) {
            compact();
            COLASSERT(m_Capacity - m_End >= Size);
         }
      }
      m_Reserved += Size;
      return m_Data + m_End;
   }

   void write(const COLuint8* Buffer, int Size)
   {
      if (Size > 0)
      {
         COLuint8* data = reserveWrite(Size);
         COLPRECONDITION(Size <= m_Capacity - m_End);
         ::memcpy(data, Buffer, Size);
         bytesWritten(Size);
      }
   }

   void clearReserve(int Size)
   {
      m_Reserved -= Size;
      if(m_Reserved < 0) {
         m_Reserved = 0;
      }
   }

private:
   void compact()
   {
      if (m_Start > 0)
      {
         // COLcout<<"compact: size="<<size()<<", cap="<<m_Capacity
         //    <<", start="<<m_Start<<", end="<<m_End<<newline;
         ::memmove(m_Data, m_Data + m_Start, m_End - m_Start);
         m_End -= m_Start;
         m_Start = 0;
         // m_Capacity remains the same
      }
   }

   COLfifoBufferPrivate(const COLfifoBufferPrivate&);
   COLfifoBufferPrivate& operator=(const COLfifoBufferPrivate&);

   enum { minimumCapacity = 256 };

   int m_Capacity;
   int m_Reserved;
   int m_Start;
   int m_End;
   COLuint8* m_Data;
};

//---------------------------------------------------------------------------

COLfifoBuffer::COLfifoBuffer(int InitialSize)
{
   pMember = new COLfifoBufferPrivate(InitialSize);
}

COLfifoBuffer::~COLfifoBuffer()
{
   delete pMember;
}

int COLfifoBuffer::size() const
{
   return pMember->size();
}

void COLfifoBuffer::clear()
{
   pMember->clear();
}

const COLuint8* COLfifoBuffer::read(int Size)
{
   return pMember->read(Size);
}

COLuint8* COLfifoBuffer::reserveWrite(int Size)
{
   return pMember->reserveWrite(Size);
}

void COLfifoBuffer::write(const COLuint8* Buffer, int Size)
{
   pMember->write(Buffer, Size);
}

void COLfifoBuffer::bytesRead(int Size)
{
   pMember->bytesRead(Size);
}

void COLfifoBuffer::bytesWritten(int Size)
{
   pMember->bytesWritten(Size);
}

void COLfifoBuffer::clearReserve(int Size)
{
   pMember->clearReserve(Size);
}

//---------------------------------------------------------------------------

COLfifoBufferRead::COLfifoBufferRead(COLfifoBuffer& iBuffer, int iSize)
:  Buffer(iBuffer),
   Size(iSize),
   AmountRead(0)
{
   assert(Size <= Buffer.size());
   pData = Buffer.read(Size);
}

COLfifoBufferRead::~COLfifoBufferRead()
{
   assert(AmountRead <= Size); // a fatal non-recoverable condition
   Buffer.bytesRead(AmountRead);
}

const COLuint8* COLfifoBufferRead::data() const
{
   return pData;
}

int COLfifoBufferRead::size()
{
   return Size;
}

void COLfifoBufferRead::setAmountRead(int AmountActuallyRead)
{
   AmountRead = AmountActuallyRead;
}

//---------------------------------------------------------------------------

COLfifoBufferWrite::COLfifoBufferWrite(COLfifoBuffer& iBuffer, int iSize)
:  Buffer(iBuffer),
   Size(iSize),
   pData(0),
   AmountWritten(0)
{
   pData = Buffer.reserveWrite(Size);
}

COLfifoBufferWrite::~COLfifoBufferWrite()
{
   try
   {
      COLPRECONDITION(AmountWritten <= Size);
      Buffer.bytesWritten(AmountWritten);
      if(AmountWritten < Size) {
         Buffer.clearReserve(Size - AmountWritten);
      }
   }
   catch(const COLerror& Error)
   {
      COL_VAR(Error);
   }
}

COLuint8* COLfifoBufferWrite::data()
{
   return pData;
}

int COLfifoBufferWrite::size()
{
   return Size;
}

void COLfifoBufferWrite::setAmountWritten(int AmountActuallyWritten)
{
   AmountWritten = AmountActuallyWritten;
}

