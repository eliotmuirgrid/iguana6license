//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
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
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLsink.h>
#include <COL/COLsource.h>


const COLuint32 COLsource::COL_SOURCE_DEFAULT_CHUNK_SIZE = 1024;

COLsource::COLsource( COLsink* ipNext)
: COLattachedSink(ipNext),
  EndOfSource(false)
{
}

COLsource::~COLsource()
{
}

void COLsource::setEndOfSource( bool iEndOfSource)
{
   EndOfSource = iEndOfSource;
}

bool COLsource::isEndOfSource() const
{
   return EndOfSource;
}

void COLsource::writeAll(COLuint32 ChunkSize)
{

   if (!hasNext())
   {
      COL_ERROR_STREAM("This COLsource object has no next() value (ie. there is no COLsink to write to).",
                       COLerror::PreCondition);
   }
   
   setEndOfSource(false);
   if( !next().readAll( *this ) )
   {
      while( !isEndOfSource())
      {  
         writeChunk(ChunkSize);
      }
      next().flush();
   }
   next().onEndStream();
   resetSourcePosition();
}
