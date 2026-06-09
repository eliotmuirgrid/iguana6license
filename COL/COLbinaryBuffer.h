#ifndef __COL_BINARY_BUFFER_H__
#define __COL_BINARY_BUFFER_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLbinaryBuffer
//
// WARNING: THIS CLASS IS LEGACY, SEE IF YOU CAN USE COLstring INSTEAD.
//
// Description:
//
// Buffer class - used to provide a easy buffer interface
// for taking in arbitrary large amounts of binary data
//
// Useful for working with C API functions.  Typical
// usage is as follows:
//
// CHMbinaryBuffer Buffer;
// COLindex SizeOfBuffer = 1456;
// void* pBuffer = Buffer.getSpaceForNewChunk(SizeOfBuffer);
// size_t CountOfCharsRead = recv(Socket, pBuffer, SizeOfBuffer);
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
// unsigned char* pBuffer = Buffer.data();
//
// Then to remove used data use
// Buffer.removeDataFromStart(23);
//
// Author: Eliot Muir
// Date:   Thu 12/02/1999 
//
//---------------------------------------------------------------------------
#include "COLsink.h"
#include "COLsourceBinary.h"

class COLbinaryBufferPrivate;
class COLstring;

class COL_DLL COLbinaryBuffer:
   public COLsink,
   public COLsourceBinary
{
public:
   enum COLgrowType
   {
      GrowGeometrically,
      GrowAritmetically
   };
   COLbinaryBuffer(size_t InitialSize=1024, 
                   COLgrowType GrowType=GrowGeometrically, 
                   size_t GrowFactor =2);

   COLbinaryBuffer(const char* MessageData); 
     // copies data using strlen to obtain length of data

   ~COLbinaryBuffer();

   void copyToString(COLstring& String,
                  size_t StartPosition, 
                  size_t EndPosition) const;

   void copyToBuffer(COLbinaryBuffer& Buffer, 
                  size_t StartPosition, 
                  size_t EndPosition) const;

   void removeChunkFromStart(size_t Size);

   char operator[](size_t Position);

   char* getSpaceForNewChunk(size_t SizeOfChunk);
   void setAmountNotUsed(size_t CountOfBytesNotUsed);

   void addBuffer(const COLbinaryBuffer& Buffer);

   void addChunk(char* pBuffer, size_t Count);
  
   char* start();
   const char* start() const;
   char* data();
   const char* data() const;
   size_t size()     const; 
   size_t capacity() const;

   // COLsink interface method
   COLuint32 write( const void* cpBuffer, COLuint32 SizeOfBuffer );

   /**
    * COLsource interface method.
    *
    * Resets the position of the source (written out in writeChunk)
    * to be the beginning of the source.
    */
   virtual void resetSourcePosition();

   /**
    * COLsourceBinary interface method.
    *
    * Sets the position of the source (written out in writeChunk)
    * to be the given offset.
    */
   virtual void setSourcePosition(COLuint32 ByteOffset);

   /**
    * COLsourceBinary
    *
    * Sets the position of the end of the source (written out in writeChunk)
    * to be the given offset.
    *
    * If the ByteOffset is set to 0 then this should mean that the
    * end position is actually the end of the source.. (ie. the
    * source's capacity).
    */
   virtual void setSourceEndPosition(COLuint32 ByteOffset);

   /**
    * COLsource interface method used to write a logical block of
    * the contents from this buffer (the source) to the attached 
    * sink object starting at the current source position.
    */
   virtual void writeChunk(COLuint32 ChunkSize); 

protected:

   COLuint32 sourceEndPosition() const;

private:
   COLbinaryBufferPrivate* pMember;

   COLbinaryBuffer(const COLbinaryBuffer& Buffer);
   COLbinaryBuffer& operator=(const COLbinaryBuffer& Buffer);
};
#endif // end of defensive include
