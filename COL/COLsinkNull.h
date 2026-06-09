#ifndef __COL_SINK_NULL_H__
#define __COL_SINK_NULL_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2007 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsinkNull
//
// Description:
//
// Null sink object.  Basically this sink object doesn't do anything
// with the data - it just ignores any data written out.  So it can be
// used to suppress output i.e. 
//
// COLcout.setSink(new COLsinkNull());
// 
// would suppress all output from COLcout.
//
// Author: Eliot Muir
// Date:   Fri 01/16/2004 
//
//---------------------------------------------------------------------------
#include "COLsink.h"

class COLstring;

class COL_DLL COLsinkNull : public COLsink
{
public:
   COLsinkNull();
   virtual ~COLsinkNull();
   virtual COLuint32 write(const void* cpBuffer, COLuint32 SizeOfBuffer);
private:
   COLsinkNull(const COLsinkNull& Orig); // not allowed
   COLsinkNull& operator=(const COLsinkNull& Orig); // not allowed
};
#endif // end of defensive include

