#ifndef __COL_FIFO_BUFFER_H__
#define __COL_FIFO_BUFFER_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc. All Rights Reserved
//
// Module: COLfifoBuffer
//
// Description:
//
// TOTALLY DEPRECATED - DON"T WASTE TIME READING THIS!
//
// First in, first out buffer that resizes automatically.
//
// COLfifoBuffer works by allocating a block of memory of the size requested
// in the constructor.  Reading is always done on one end and writing is done
// on the other end.  The buffer is implemented such that once the end of the
// buffer has been written to and data has been read from the beginning of the
// buffer, the data wraps around and continues to be written at the beginning
// of the buffer (assuming there is room).  If there is not enough room, the
// buffer size is increased geometrically.  This algorithm reduces the amount
// of memory allocation needed, since the buffer is often reused when it is
// written to.
//
// The complication comes from the fact that when a COLfifoBuffer is used,
// the data being read should be continuous.  However, if the data wraps
// around the buffer, a pointer cannot be returned that points to a
// continuous block of data. To handle this situation the class has the
// functions beginReadRequest, which copies the data to a temporary
// continuous buffer, and endReadRequest, which destroys that buffer.  This
// means that any code reading from a COLfifoBuffer can always be sure it
// will get a continuous stream of data.
//
// Two classes have been provided, COLfifoBufferRead for reading and
// COLfifoBufferWrite for writing.  Those classes should be used when
// accessing the data in a COLfifoBuffer.
//
// Author: Ryan Dewsbury
// Date:   Thu 12/02/1999 
//
// Author (rewrite): Andrew Vajoczki
// January 10, 2009
//
//
//
//---------------------------------------------------------------------------
#include <COL/COLminimumInclude.h>

class COLfifoBufferPrivate;
class COLfifoBufferRead;
class COLfifoBufferWrite;

class COL_DLL COLfifoBuffer
{
public:
   COLfifoBuffer(int InitialSize = 0x1000);
   ~COLfifoBuffer();

   // The number of readable bytes in this in-memory fifo.
   int size() const;

   // Clear the in-memory fifo.
   void clear();

   // Return pointer to bytes to read in fifo, given a size.
   // Does not call bytesRead() or advance the read position of the fifo. 
   // Returns 0 if Size > size().
   const COLuint8* read(int Size);

   // Set number of bytes actually read after read() called.
   void bytesRead(int Size);

   // Reserve a new chunk of memory to write to of size 'Size'.
   COLuint8* reserveWrite(int Size);
   void clearReserve(int Size);

   // Set number of bytes actually written after reserveWrite() called.
   void bytesWritten(int Size);

   // Append Buffer to fifo. Calls bytesWritten().
   void write(const COLuint8* Buffer, int Size);

private:
   COLfifoBufferPrivate* pMember;

   COLfifoBuffer(const COLfifoBuffer& Buffer);
   COLfifoBuffer& operator=(const COLfifoBuffer& Buffer);
};

class COL_DLL COLfifoBufferRead
{
public:
   COLfifoBufferRead(COLfifoBuffer& iBuffer, int iSize);
   ~COLfifoBufferRead();

   const COLuint8* data() const;
   int size();
   void setAmountRead(int AmountActuallyRead);

private:
   COLfifoBuffer& Buffer;
   const COLuint8* pData;
   int Size;
   int AmountRead;

   COLfifoBufferRead(const COLfifoBufferRead& Buffer);
   COLfifoBufferRead& operator=(const COLfifoBufferRead& Buffer);
};

class COL_DLL COLfifoBufferWrite
{
public:
   COLfifoBufferWrite(COLfifoBuffer& iBuffer, int iSize);
   ~COLfifoBufferWrite();

   COLuint8* data();
   int size();
   void setAmountWritten(int AmountActuallyWritten);

private:
   COLfifoBuffer& Buffer;
   COLuint8* pData;
   int Size;
   int AmountWritten;

   COLfifoBufferWrite(const COLfifoBufferWrite& Buffer);
   COLfifoBufferWrite& operator=(const COLfifoBufferWrite& Buffer);
};

#endif // end of defensive include
