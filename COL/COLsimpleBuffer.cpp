//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: Implementation of COLsimpleBuffer
//
// Description:
//
// Buffer class - used to allocate a block of
// memory 
//
// Author: Ryan Dewsbury
// Date:   Wed 09/09/1998 
//
//---------------------------------------------------------------------------

#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLsimpleBuffer.h>

COLsimpleBuffer::COLsimpleBuffer( COLuint32 InitialSize ):
   Capacity( 0 ),
   Size( 0 ),
   pBuffer( NULL ),
   pEnd ( NULL ),
   SinkStartIndex( 0 ),
   ReadStartIndex( 0 ),
   SourceStartIndex( 0 ),
   SourceEndPosition( 0 )
{
   if(InitialSize) {
      resize(InitialSize);
   }
}

COLsimpleBuffer::~COLsimpleBuffer()
{
   ::free(pBuffer);
}

COLuint8* COLsimpleBuffer::data()
{
   return pBuffer;
}

const COLuint8* COLsimpleBuffer::data() const
{
   return pBuffer;
}

const COLuint8* COLsimpleBuffer::end() const
{
   return pEnd;
}

void COLsimpleBuffer::resize( COLuint32 NewSize )
{
   if(!NewSize) {
      ::free(pBuffer);
      pBuffer = pEnd = NULL;
      Size = Capacity = 0;
      SinkStartIndex = ReadStartIndex = 0;
      SourceStartIndex = SourceEndPosition = 0;
      return;
   }

   // We want to shrink sometimes, but not defeat doubling algorithms.
   if(NewSize > Capacity || NewSize < (Capacity >> 1)) {
      COLuint8* pNewBuffer = (COLuint8*)realloc(pBuffer, NewSize);
      COLMSGPRECONDITION(pNewBuffer, "Out of memory.");
      pBuffer = pNewBuffer;
      Capacity = NewSize;
   }
   Size = NewSize;
   if(NewSize < SinkStartIndex)
   {
      SinkStartIndex = NewSize;
   }
   pEnd = pBuffer + Size;
}

COLuint32 COLsimpleBuffer::size() const
{
   return Size;
}

COLuint8& COLsimpleBuffer::operator[](const COLuint32 PositionIndex)
{
   COLPRECONDITION(PositionIndex < size());
   return pBuffer[PositionIndex];
}

const COLuint8 COLsimpleBuffer::operator[](const COLuint32& PositionIndex) const
{
   COLPRECONDITION(PositionIndex < size());
   return pBuffer[PositionIndex];
}
   
   
bool operator==(const COLsimpleBuffer& Buffer1, const COLsimpleBuffer& Buffer2)
{
   if (Buffer1.size() != Buffer2.size())
   {
      return false;
   }
   return (0 == memcmp(Buffer1.data(), Buffer2.data(), Buffer1.size()));
}


COLsimpleBuffer& COLsimpleBuffer::operator=(const char* pString)
{
   COLuint32 SizeOfString = strlen(pString) + 1;

   resize(SizeOfString);
   memcpy(pBuffer, pString, SizeOfString);
   return *this;
}

COLsimpleBuffer& COLsimpleBuffer::operator=(const COLsimpleBuffer& Buffer)
{
   resize(Buffer.size());
   memcpy(pBuffer,Buffer.data(),Buffer.size());
   return *this;
}

COLsimpleBuffer::COLsimpleBuffer(const COLsimpleBuffer& Buffer) :
   Capacity( 0 ),
   Size( 0 ),
   pBuffer( NULL ),
   pEnd ( NULL ),
   SinkStartIndex( 0 ),
   ReadStartIndex( 0 ),
   SourceStartIndex( 0 ),
   SourceEndPosition( 0 )
{
   *this = Buffer;
}

COLuint32 COLsimpleBuffer::write(const void* cpBuffer, COLuint32 SizeOfBuffer)
{
   if (SinkStartIndex + SizeOfBuffer > Capacity)
   {
      //COLsink interface doubles size    
      resize(2*(SinkStartIndex + SizeOfBuffer));
      resize(SinkStartIndex + SizeOfBuffer);
   }
   memcpy(pBuffer + SinkStartIndex,cpBuffer, SizeOfBuffer);
   SinkStartIndex += SizeOfBuffer;
   pEnd = COL_MAX((pBuffer + SinkStartIndex), pEnd);
   Size = COL_MAX(SinkStartIndex, Size);

   return SizeOfBuffer;
}

void COLsimpleBuffer::resetSourcePosition()
{
   SourceStartIndex = 0;
}

void COLsimpleBuffer::setSourcePosition(COLuint32 ByteOffset)
{
   SourceStartIndex = ByteOffset;
}

void COLsimpleBuffer::setSourceEndPosition(COLuint32 ByteOffset)
{
   SourceEndPosition = ByteOffset;
}

COLuint32 COLsimpleBuffer::sourceEndPosition() const
{
   if ((SourceEndPosition == 0) || (SourceEndPosition >= size()))
   {
      return size();
   }
   else 
   {
      return SourceEndPosition;
   }
}

void COLsimpleBuffer::writeChunk(COLuint32 ChunkSize)
{

   if (SourceStartIndex >= sourceEndPosition())
   {
      setEndOfSource(true);
      return;
   }

   if (SourceStartIndex + ChunkSize >= sourceEndPosition())
   {
      setEndOfSource(true);
      ChunkSize = sourceEndPosition() - SourceStartIndex;
   }
  
   next().write(pBuffer + SourceStartIndex, ChunkSize);
   SourceStartIndex += ChunkSize;

}

COLuint32 COLsimpleBuffer::read(void* cpBuffer, COLuint32 SizeOfBuffer)
{
   if (ReadStartIndex + SizeOfBuffer > Capacity)
   {
      SizeOfBuffer = Capacity - ReadStartIndex;
   }
   
   memcpy(cpBuffer, pBuffer + ReadStartIndex, SizeOfBuffer);
   ReadStartIndex += SizeOfBuffer;
   return SizeOfBuffer;
}

COLindex COLsimpleBuffer::readPosition() const
{
   return ReadStartIndex; 
}

void COLsimpleBuffer::setReadPosition(COLindex PositionIndex)
{
   ReadStartIndex = PositionIndex;
}

bool COLsimpleBuffer::isEndOfReadable() const
{
   if (ReadStartIndex >= Capacity)
   {
      return true;
   }
   return false;
}

void COLsimpleBuffer::resetSinkPosition()
{
   SinkStartIndex = 0;
}
