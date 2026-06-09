//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: Implementation of COLbinaryBuffer
//
// Description:
//
// Buffer class - used to provide a easy buffer interface
// for taking in arbitrary large amounts of binary data
//
// Useful for working with C API functions.  Typical
// usage is as follows:
//
// COLbinaryBuffer Buffer;
// size_t SizeOfBuffer = 1456;
// void* pBuffer = Buffer.getSpaceForNewChunk(SizeOfBuffer);
// COLindex CountOfCharsRead = recv(Socket, pBuffer, SizeOfBuffer);
// Buffer.setAmountNotUsed(SizeOfBuffer - CountOfCharsRead);
//
// Then accessing the buffer data is a matter of using [] operator
// for (COLindex CharIndex = 0;
//      CharIndex < Buffer.size();
//      CharIndex)
// {
//    if Buffer[CharIndex] == ...
// }
// To copy data from the buffer one can
// access it like a string using
// COLstring AString;
// Buffer.copyToString(AString, 1, 23);  // copies string from 1, 23
// or as a buffer
//
// COLbinaryBuffer SubBuffer;
// Buffer.copyToBuffer(SubBuffer,1,23) // copies buffer from 1, 23 into SubBuffer
// Then to get a pointer to the raw buffer
// char* pBuffer = Buffer.data();
//
// Then to remove used data use
// Buffer.removeDataFromStart(23);
//
// Author: Eliot Muir
// Date:   Thu 12/02/1999 
//
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLbinaryBuffer.h>

#include <COL/COLerror.h>
#include <COL/COLostream.h>
#include <COL/COLstring.h>

class COLbinaryBufferPrivate
{
public:
   COLbinaryBufferPrivate(size_t                       iInitialSize,
                          COLbinaryBuffer::COLgrowType iGrowType,
                          size_t                       iGrowFactor);

   ~COLbinaryBufferPrivate();

   bool bufferOkay() const;

   void growTo(size_t NewSize);
   void copyDataToStart();

   char* pData;
   size_t Capacity;
   size_t Start;
   size_t SourceStartIndex;
   size_t SourceEndPosition;
   size_t Length;
   size_t GrowFactor;
   COLbinaryBuffer::COLgrowType GrowType;
};

COLbinaryBufferPrivate::COLbinaryBufferPrivate
(
   size_t      iInitialSize,
   COLbinaryBuffer::COLgrowType iGrowType,
   size_t      iGrowFactor
) : GrowType(iGrowType), 
    GrowFactor(iGrowFactor),
    pData(NULL), Capacity(iInitialSize), Length(0), Start(0), SourceStartIndex(0),SourceEndPosition(0)
{
   pData = new char[iInitialSize];
}

COLbinaryBufferPrivate::~COLbinaryBufferPrivate()
{
   delete[] pData;
}

bool COLbinaryBufferPrivate::bufferOkay() const
{
   COLPRECONDITION(Start + Length <= Capacity);
   COLPRECONDITION((pData == NULL && Capacity == 0)
                   || (pData != NULL));
   return true;
}

void COLbinaryBufferPrivate::copyDataToStart()
{
   memmove(pData, pData + Start, Length);
   Start = 0;
}

void COLbinaryBufferPrivate::growTo(size_t CapacityRequired)
{
   COLPRECONDITION(bufferOkay());
   size_t NewCapacity;
   if (GrowType == COLbinaryBuffer::GrowGeometrically)
   {
      NewCapacity = Capacity;
      if (0 == NewCapacity)
      {
         NewCapacity = 10;
      }
      while (NewCapacity < CapacityRequired)
      {
         NewCapacity *= GrowFactor;            
      }
   }
   else
   {
      NewCapacity = ((CapacityRequired % GrowFactor) + 1) * GrowFactor;
   }
   COLPOSTCONDITION(NewCapacity >= CapacityRequired);
   char* pNewData = new char[NewCapacity];
   memcpy(pNewData, pData + Start, Length);
   delete[] pData;
   pData = pNewData;
   Start = 0;
   Capacity = NewCapacity;
   
   COLPOSTCONDITION(bufferOkay());
}

COLbinaryBuffer::COLbinaryBuffer
(
   size_t InitialSize, 
   COLgrowType GrowType,
   size_t GrowFactor
   ):COLsourceBinary(NULL)
{
   pMember = new COLbinaryBufferPrivate(InitialSize, GrowType, GrowFactor);
   COLPOSTCONDITION(pMember->bufferOkay());
}

COLbinaryBuffer::COLbinaryBuffer
(
   const char* pData
):COLsourceBinary(NULL)
{
   pMember = new COLbinaryBufferPrivate(strlen(pData)+1, GrowGeometrically, 2);
   strcpy(pMember->pData, pData);
   pMember->Length = strlen(pData);
   COLPOSTCONDITION(pMember->bufferOkay());
}

COLbinaryBuffer::~COLbinaryBuffer()
{
   delete pMember;
}

void COLbinaryBuffer::copyToString
(
   COLstring& String,
   size_t StartPosition, 
   size_t EndPosition
) const
{
   COLPRECONDITION(pMember->bufferOkay());
   COLPRECONDITION(StartPosition < pMember->Length);
   COLPRECONDITION(EndPosition < pMember->Length);
   COLPRECONDITION(EndPosition >= StartPosition);

   char SavedChar = pMember->pData[pMember->Start + EndPosition];
   pMember->pData[pMember->Start + EndPosition] = 0;
   String = &pMember->pData[pMember->Start + StartPosition];
   pMember->pData[pMember->Start + EndPosition] = SavedChar;
   COLPOSTCONDITION(pMember->bufferOkay());
}

void COLbinaryBuffer::copyToBuffer
(
   COLbinaryBuffer& Buffer, 
   size_t StartPosition, 
   size_t EndPosition
) const
{
   COLPRECONDITION(pMember->bufferOkay());
   COLPRECONDITION(Buffer.pMember->bufferOkay());
   COLPRECONDITION(StartPosition < pMember->Length);
   COLPRECONDITION(EndPosition < pMember->Length);
   COLPRECONDITION(EndPosition >= StartPosition);
   delete[] Buffer.pMember->pData;
   Buffer.pMember->pData = new char[EndPosition - StartPosition];
   memcpy(Buffer.pMember->pData, 
          pMember->pData + pMember->Start + StartPosition,
          EndPosition-StartPosition);
   Buffer.pMember->Length = EndPosition - StartPosition;
   Buffer.pMember->Capacity = EndPosition - StartPosition;
   Buffer.pMember->Start = 0;
   COLPOSTCONDITION(pMember->bufferOkay());
   COLPOSTCONDITION(Buffer.pMember->bufferOkay());
}

void COLbinaryBuffer::removeChunkFromStart(size_t Size)
{
   if (Size == pMember->Length)
   {
      // empty buffer
      pMember->Start = 0;
      pMember->Length = 0;
   }
   else
   {
      pMember->Start += Size;
      pMember->Length -= Size;
      COLPRECONDITION( pMember->Start <= pMember->Capacity );
   }
}

char COLbinaryBuffer::operator[](size_t Position)
{
   COLPRECONDITION(pMember->bufferOkay());
   COLPRECONDITION(Position < pMember->Length);

   return pMember->pData[pMember->Start + Position];
}

char* COLbinaryBuffer::getSpaceForNewChunk
(
  size_t SizeOfChunk
)
{
   COLPRECONDITION(pMember->bufferOkay());
   if (SizeOfChunk > (pMember->Capacity - pMember->Length))
   {
      // got to grow to new capacity
      pMember->growTo(SizeOfChunk + pMember->Length);
   }
   else if (pMember->Length + pMember->Start + SizeOfChunk > pMember->Capacity)
   {
      pMember->copyDataToStart();
   }
   char* pData = pMember->pData + pMember->Start + pMember->Length;
   pMember->Length += SizeOfChunk;
   COLPOSTCONDITION(pMember->bufferOkay());
   return pData;
}

void COLbinaryBuffer::setAmountNotUsed(size_t CountOfBytesNotUsed)
{
   COLPRECONDITION(pMember->bufferOkay());
   COLPRECONDITION(pMember->Length >= CountOfBytesNotUsed);
   pMember->Length -= CountOfBytesNotUsed;
   COLPOSTCONDITION(pMember->bufferOkay());
}
  
char* COLbinaryBuffer::data()
{
   return pMember->pData;
}

const char* COLbinaryBuffer::data() const
{
   return pMember->pData;
}

char* COLbinaryBuffer::start()
{
   return pMember->pData + pMember->Start;
}

const char* COLbinaryBuffer::start() const
{
   return pMember->pData + pMember->Start;
}


size_t COLbinaryBuffer::size() const
{
   return pMember->Length;
}

size_t COLbinaryBuffer::capacity() const
{
   return pMember->Capacity;
}

void COLbinaryBuffer::addBuffer(const COLbinaryBuffer& Buffer)
{
   char* pNewBlock = getSpaceForNewChunk(Buffer.size());
   memcpy(pNewBlock, Buffer.data(), Buffer.size());
}

void COLbinaryBuffer::addChunk(char* pBuffer, size_t Count)
{
   char* pNewBlock = getSpaceForNewChunk(Count);
   memcpy(pNewBlock, pBuffer, Count);
}

COLuint32 COLbinaryBuffer::write
( 
   const void* cpBuffer, 
   COLuint32 SizeOfBuffer 
)
{
   addChunk( (COLint8*)cpBuffer, SizeOfBuffer );

   return SizeOfBuffer;
}

void COLbinaryBuffer::resetSourcePosition()
{
   pMember->SourceStartIndex = 0;
}

void COLbinaryBuffer::setSourcePosition(COLuint32 ByteOffset)
{
   pMember->SourceStartIndex = ByteOffset;
}

void COLbinaryBuffer::setSourceEndPosition(COLuint32 ByteOffset)
{
   pMember->SourceEndPosition = ByteOffset;
}

COLuint32 COLbinaryBuffer::sourceEndPosition() const
{

   if ((pMember->SourceEndPosition == 0) || (pMember->SourceEndPosition >= size()))
   {
      return size();
   }
   else
   {
      return pMember->SourceEndPosition;
   }
}

void COLbinaryBuffer::writeChunk(COLuint32 ChunkSize)
{

//  next().write(data(), size());
//  setEndOfSource();

// UNCOMMENTING THE FOLLOWING screws up TRENET



   if (pMember->SourceStartIndex >= sourceEndPosition())
   {
      setEndOfSource(true);
      return;
   }

   if (pMember->SourceStartIndex + ChunkSize >= sourceEndPosition())
   {
      ChunkSize = sourceEndPosition() - pMember->SourceStartIndex;
      setEndOfSource(true);
   }
  
   next().write(start() + pMember->SourceStartIndex, ChunkSize);

   pMember->SourceStartIndex += ChunkSize;

}
