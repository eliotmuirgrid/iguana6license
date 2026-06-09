#ifndef __COL_FILTER_H__
#define __COL_FILTER_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLfilter
//
// Description:
//
// Don't spend much time reading this - not that useful in practice.
//
// COLfilter subclasses are designed to sit between a COLsource and
// a COLsink and filter the data flow in some way.  In order to 
// achieve this, the subclass must implement COLsink's 
// write() method and, much like the COLsource, it must pass the
// data on to the next COLsink in the chain using a next().write()
// call.
//
// Author: Ryan Dewsbury, Rob Moyse
// Date:   November 12, 2004
//---------------------------------------------------------------------------

#include <COL/COLsink.h>
#include <COL/COLattachedSink.h>

class COL_DLL COLfilter: 
   public COLsink,
   public COLattachedSink
{
public:

   // Constructor takes a pointer to the COLsink object that
   // will be the destination for this filter's data.  
   COLfilter( COLsink* pNext ) : COLattachedSink( pNext ){};
   virtual ~COLfilter(){}
   
   virtual bool readAll( COLsource& Source ){ return next().readAll( Source ); }

   // Indicates that any state information retained between write() calls
   // is reset. This allows reuse of a filter with a new stream
   // without reconstructing a new filter.
   virtual void resetFilter() = 0;

   virtual void onEndStream(){ next().onEndStream(); };
   virtual void flush(){ next().flush(); };
private:
   COLfilter(const COLfilter& Filter);
   COLfilter& operator=(const COLfilter& Filter);
};

#endif  // end of defensive include
