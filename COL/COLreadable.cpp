//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLreadable
//
// Description:
//
// This is an abstract interface for all objets that wish to provide
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
//
//
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop
#include <COL/COLreadable.h>

// constructor and destructor have to go here because this function
// gets exported in a DLL
COLreadable::COLreadable()
{
   // do nothing
}

COLreadable::~COLreadable()
{
   // do nothing
}
