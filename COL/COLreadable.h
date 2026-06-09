#ifndef __COL_READABLE_H__
#define __COL_READABLE_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLreadable
//
// Description:
//
// Don't waste time reading this code - it's not that useful for real applications.
//
// This is an abstract interface for all objects that wish to provide
// a read() style interface.  This supports application code that
// wants to actively read data from an object. Subclasses must implement
// the methods: read(), position() and setPosition().
//
// NOTE: The preferable interface for a source-style object to implement
//       is the COLsource interface which pushes data to a sink.  This
//       COLreadable interface is different in that it allows users to
//       actively pull from a source.
//
// Author: Rob Moyse
// Date:   November 12, 2004
//---------------------------------------------------------------------------

#include "COLattachedSink.h"

class COL_DLL COLreadable
{

public:

   COLreadable();
   virtual ~COLreadable();

   // Attempts to populate the buffer, pBuffer, with SizeOfBuffer
   // bytes from this source object.  Returns the actual number of
   // bytes read and increments the position inside this source.
   virtual COLuint32 read(void* pBuffer, COLuint32 SizeOfBuffer) = 0;

   // Returns the current position within the readable.  This is
   // set by setPosition() and incremented by read().
   virtual COLindex readPosition() const = 0;

   // Sets the position within the source which will be the 
   // location of the next read.
   virtual void setReadPosition(COLindex PositionIndex) = 0;

   // Returns true if there is nothing left to read (ie. the
   // position is now at the end of the readable object).
   virtual bool isEndOfReadable() const = 0;
private:
   COLreadable(const COLreadable& Orig);
   COLreadable& operator=(const COLreadable& Orig);
};

#endif // end of defensive include

