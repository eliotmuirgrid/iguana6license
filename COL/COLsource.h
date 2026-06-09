#ifndef __COL_SOURCE_H__
#define __COL_SOURCE_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsource
//
// Description:
//
// This is the base class for all objects that can act as
// a data source.  Data in a COLsource object is transferred to
// an attached COLsink object when writeAll() is called.
//
// The COLsource and COLsink abstract base classes should be used
// to implement all reading/writing of data between application 
// objects.
//
// The COLsource works by having a pointer to a "next" COLsink
// object.
//
// Subclasses need to implement the writeChunk() method
// found on this interface. Inside this method, the implementor 
// must write data to the attached COLsink by issuing a call to 
// next().write() as many times as necessary to write out as close
// to the indicated ChunkSize as possible.  The writeChunk() 
// implementation must signal that the source is done writing
// when it finishes writing the entire source by calling 
// setEndOfSource(true).
//
// The data transfer process from a COLsource to a COLsink is
// initiated when the writeAll() method is called on the source.
// The writeAll() method will invoke the source's resetSourcePosition()
// method and then iteratively call the writeChunk() 
// implementation until EndOfSource is set to true.
//
// Example usage:
//
//   MYsinkClass SinkObject;
//   ...
//   MYsourceClass SourceObject(SinkObject);
//   ...
//   SourceObject.writeAll();
//
// Important exception to the above: If the COLsink that is attached
// to this COLsource implements the readAll() method (and returns
// true from it) then the COLsink assumes responsibility for getting
// the COLsource to send its data by calling writeChunk()
// on the source.  This gives the sink the ability to pull data from
// the source exactly when it needs it so that the sink doesn't need
// to buffer too much data.  Otherwise (in the case that readAll() is not 
// implemented by the sink) the source will repeatedly
// call writeChunk() until EndOfStream is true as described
// earlier.
//
// Author: Ryan Dewsbury, Rob Moyse
// Date:   November 12, 2004
//
//
//---------------------------------------------------------------------------
#include <COL/COLminimumInclude.h>

#include <COL/COLattachedSink.h>

class COLsink;

class COL_DLL COLsource:
   public COLattachedSink
   {
public:

   static const COLuint32 COL_SOURCE_DEFAULT_CHUNK_SIZE;

   /**
    * Constructor takes:
    *
    * 1) A pointer to the COLsink object that
    *    will be the destination for this source's data.  
    *    May be NULL.  However, setNext() must be issued to set
    *    a COLsink target before writeAll() is called.
    */
   COLsource( COLsink* ipNext = NULL );
   virtual ~COLsource();

   /**
    * Returns true if this source has no more data to write
    * to the destination COLsink.
    */   
   virtual bool isEndOfSource() const;

   /**
    * Sets if this source has any more data to write
    * to the destination COLsink. (true = no more data)
    */   
   void setEndOfSource( bool iEndOfSource = true );

   /**
    * This method is called to initiate the transfer of data
    * from the COLsource to the COLsink.
    *
    * Will start by calling the subclass's resetSourcePosition()
    * method.  Then will first call the attached sink's readAll()
    * method to give the sink the option of of pulling the data, if
    * this returns false then writeAll() will iteratively call 
    * writeChunk() until EndOfSource is set.
    */   
   void writeAll(COLuint32 ChunkSize = COL_SOURCE_DEFAULT_CHUNK_SIZE);

   /**
    * The implementation of this method must write data to the 
    * attached COLsink by issuing  calls using the next().write()
    * API as many times as is appropriate for the current
    * logical 'chunk'.  If all the data is written out during
    * this process, then the writeChunk() method must also
    * set the EndOfSource value to true using setEndOfSource().
    *
    * Parameters:
    * 
    * 1) The ChunkSize is the size of the chunk that should
    *    be written by every call to writeChunk().  Some sources
    *    will use this only as a guideline (this is because some
    *    sources have no notion of binary size until the source
    *    is already streamed out to binary).
    */
   virtual void writeChunk(COLuint32 ChunkSize = COL_SOURCE_DEFAULT_CHUNK_SIZE) = 0; 

   /**
    * Resets the position of the source (written out in writeChunk)
    * to be the beginning of the source.
    */
   virtual void resetSourcePosition() = 0;

private:
   bool EndOfSource;
   COLsource(const COLsource& Source);
   COLsource& operator=(const COLsource& Source);
};

#endif // end of defensive include

