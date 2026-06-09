#ifndef __COL_ATOMIC_H__
#define __COL_ATOMIC_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2012 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLatomic
//
// Description:
//
// Portable atomic operations.
// 
// Add support for new types as needed...
//
// Eliot - you probably won't need to use this since all for singletons, static iniatialization and
// other stuff that a well designed app should not be using.
//
// Author: Steven Dodd
// Date:   Tuesday, May 29th, 2012 @ 10:27:27 AM
//---------------------------------------------------------------------------

#include <COL/COLminimumInclude.h>

//
// 32-bit
// 

COLint32 COLatomicCompareExchange32(COLint32 volatile* pVal, COLint32 New, COLint32 Old);
COLint32 COLatomicExchange32(COLint32 volatile* pVal, COLint32 New);
COLint32 COLatomicIncrement32(COLint32 volatile* pVal);
COLint32 COLatomicDecrement32(COLint32 volatile* pVal);

inline COLuint32 COLatomicCompareExchange32(COLuint32 volatile* pVal, COLuint32 New, COLuint32 Old)
{
   return (COLuint32) COLatomicCompareExchange32((COLint32 volatile*)pVal, (COLint32)New, (COLint32)Old);
}

inline COLuint32 COLatomicExchange32(COLuint32 volatile* pVal, COLuint32 New)
{
   return (COLuint32) COLatomicExchange32((COLint32 volatile*)pVal, (COLint32)New);
}

inline COLuint32 COLatomicIncrement32(COLuint32 volatile* pVal)
{
   return (COLuint32) COLatomicIncrement32((COLint32 volatile*)pVal);
}

inline COLuint32 COLatomicDecrement32(COLuint32 volatile* pVal)
{
   return (COLuint32) COLatomicDecrement32((COLint32 volatile*)pVal);
}

#endif // end of defensive include
