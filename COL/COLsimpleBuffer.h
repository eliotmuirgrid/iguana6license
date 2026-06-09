#ifndef __COL_SIMPLE_BUFFER_H__
#define __COL_SIMPLE_BUFFER_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsimpleBuffer
//
// Description:
//
// DEPRECATED - modern applications should use COLstring instead - Eliot
//
// Author: Ryan Dewsbury
// Date:   Wed 21/01/2004 
//---------------------------------------------------------------------------
#include <COL/COLsink.h>
#include <COL/COLsourceBinary.h>
#include <COL/COLreadable.h>

// TODO should get this to support COLsource
class COL_DLL COLsimpleBuffer 
   : public COLsink, public COLsourceBinary, public COLreadable
{
public:
   //constructs a buffer with a specified size
   COLsimpleBuffer( COLuint32 InitialSize = 0 );

   //destructor... deletes the internal buffer
   virtual ~COLsimpleBuffer();

   //returns a pointer to the begining of the buffer
   COLuint8* data();
   const COLuint8* data() const;

   inline COLuint32 start() const { return SinkStartIndex; }
   inline void setStart(COLuint32 iStartIndex) { SinkStartIndex = iStartIndex;}
   // return a pointer to the end of the buffer - useful for loops
   // finding the end of the data.
   const COLuint8* end() const;

   //changes the size of the buffer
   void resize( COLuint32 NewSize );

   //returns the size of the buffer
   COLuint32 size() const;

   // COLsink interface method which is called from a source object
   // when it wishes to pass this buffer (the sink) the provided data
   virtual COLuint32 write(const void* pBuffer, COLuint32 SizeOfBuffer);

   // COLsource interface method used to write a logical block of
   // the contents from this buffer (the source) to the attached 
   // sink object
   virtual void writeChunk(COLuint32 ChunkSize); 

   /**
    * Resets the position of the source (written out in writeChunk)
    * to be the beginning of the source.
    */
   virtual void resetSourcePosition();

   /**
    * Sets the position of the source (written out in writeChunk)
    * to be the given offset.
    */
   virtual void setSourcePosition(COLuint32 ByteOffset);

   /**
    * Sets the position of the end of the source (written out in writeChunk)
    * to be the given offset.
    *
    * If the ByteOffset is set to 0 then this should mean that the
    * end position is actually the end of the source.. (ie. the
    * source's capacity).
    */
   virtual void setSourceEndPosition(COLuint32 ByteOffset);

   /**
    * COLreadable interface method.
    * 
    * Attempts to populate the buffer, pBuffer, with SizeOfBuffer
    * bytes from this source object.  Returns the actual number of
    * bytes read and increments the position inside this source.
    *
    * Note: to stream all data from this source into some sink
    * it is preferable to attach a sink object and call writeAll()
    * on the source object to invoke the writeChunk() implementation.
    */
   virtual COLuint32 read(void* cpBuffer, COLuint32 SizeOfBuffer);

   /**
    * COLreadable interface method.
    *
    * Returns the current position within the readable.  This is
    * set by setPosition() and incremented by read().
    */
   virtual COLindex readPosition() const;

   /**
    * COLreadable interface method.
    *
    * Sets the position within the readable which will be the 
    * location of the next read.
    */
   virtual void setReadPosition(COLindex PositionIndex);

   /**
    * COLreadable interface method.
    *
    * Returns true if there is nothing left to read (ie. the
    * position is now at the end of the readable object).
    */
   virtual bool isEndOfReadable() const;

   /**
    * Resets the write position to the beginning of the buffer.
    */
   void resetSinkPosition();

   COLsimpleBuffer& operator=(const char* pString);
   COLsimpleBuffer& operator=(const COLsimpleBuffer& Buffer);
   COLsimpleBuffer(const COLsimpleBuffer& Buffer);
   
   COLuint8& operator[](const COLuint32 PositionIndex);
   const COLuint8 operator[](const COLuint32& PositionIndex) const;

protected:

   COLuint32 sourceEndPosition() const;

private:
   COLuint8* pBuffer;
   COLuint8* pEnd;
   COLuint32 SinkStartIndex;
   COLuint32 ReadStartIndex;
   COLuint32 SourceStartIndex;
   COLuint32 SourceEndPosition;
   COLuint32 Size;
   COLuint32 Capacity;
};

bool operator==(const COLsimpleBuffer& Buffer1, const COLsimpleBuffer& Buffer2);

inline bool operator!=(const COLsimpleBuffer& Buffer1, const COLsimpleBuffer& Buffer2)
{
   return !(Buffer1 == Buffer2);
}

#endif // end of defensive include
