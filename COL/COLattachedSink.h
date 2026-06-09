#ifndef __COL_ATTACHED_SINK_H__
#define __COL_ATTACHED_SINK_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc. All Rights Reserved
//
// Module: COLattachedSink
//
// Description:
//
// This is the base class for all classes that have a pointer to 
// a COLsink next() object.
//
// Direct subclasses: COLsource, COLfilter
// 
// Eliot - don't spend time with this file - it's not that useful for real applications.
//
// Author: Ryan Dewsbury, Rob Moyse
// Date:   November 12, 2004
//---------------------------------------------------------------------------
#include <COL/COLminimumInclude.h>

class COLsink;

class COL_DLL COLattachedSink
{
public:
    // Constructor takes a pointer to the COLsink object that
    // will be the destination for this source's data.  
    //
    // May be NULL.  SetNext() can be issued at any time to set
    // a COLsink target.
   COLattachedSink( COLsink* ipNext = NULL );
   virtual ~COLattachedSink();

   void setNext( COLsink* ipNext );
   bool hasNext() const;
   COLsink& next() const;

private:
   COLsink* pNext;
   COLattachedSink(COLattachedSink& Orig);   
   COLattachedSink& operator=(COLattachedSink& Orig);   
};

#endif // end of defensive include
