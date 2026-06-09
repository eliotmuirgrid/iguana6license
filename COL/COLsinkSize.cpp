//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsinkSize
//
// Description:
//
// Implementation
//
// Author: Ryan Dewsbury
//

//---------------------------------------------------------------------------

#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLsinkSize.h>

COLsinkSize::COLsinkSize():Size(0)
{
}

COLsinkSize::~COLsinkSize()
{
}

COLuint32 COLsinkSize::write
( 
   const void* pBuffer, 
   COLuint32 SizeOfBuffer 
)
{
   Size += SizeOfBuffer;

   return SizeOfBuffer;
}

