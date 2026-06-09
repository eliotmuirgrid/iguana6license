//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLattachedSink
//
// Description:
//
// This is the base class for all classes that have a pointer to 
// a COLsink next() object.
//
// See direct subclasses: COLsource, COLfilter
// 
// Author: Ryan Dewsbury, Rob Moyse
// Date:   November 12, 2004
//
//
//---------------------------------------------------------------------------

#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLattachedSink.h>


COLattachedSink::COLattachedSink( COLsink* ipNext)
{
   pNext = ipNext;
}

COLattachedSink::~COLattachedSink()
{
}

void COLattachedSink::setNext( COLsink* ipNext )
{
   pNext = ipNext;
}

bool COLattachedSink::hasNext() const
{
   if (pNext)
   {
      return true;
   }
   else
   {
      return false;
   }
}

COLsink& COLattachedSink::next() const
{
   COLPRECONDITION(pNext != NULL); 
   return *pNext;
}

