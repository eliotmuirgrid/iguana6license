//---------------------------------------------------------------------------
// Copyright (C) 1997-2012 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLatomic
//
// Description:
//
// Implementation
//
// Author: Dmitri Klimenko
// Date:   Tuesday, May 29th, 2012 @ 10:27:27 AM
//
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include "COLatomic.h"

#include <COL/COLlog.h>
COL_LOG_MODULE;


//
// 32-bit signed/unsigned
// 

COLint32 COLatomicCompareExchange32(COLint32 volatile* pVal, COLint32 New, COLint32 Old)
{
#if defined(_MSC_VER) && _MSC_VER <= 1200 // VC6
   return (COLint32) InterlockedCompareExchange((void**)pVal, (void*)New, (void*) Old);
#elif defined(_MSC_VER) && _MSC_VER > 1200
   return (COLint32) InterlockedCompareExchange((LONG volatile *)pVal, (LONG)New, (LONG)Old);
#elif defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 )
   return __sync_val_compare_and_swap(pVal, Old, New); 
#else
   COLint32 Temp = *pVal;
   if (*pVal == Old) {
      *pVal = New;
   }
   return Temp;
#endif
}

COLint32 COLatomicExchange32(COLint32 volatile* pVal, COLint32 New)
{
#if defined(_MSC_VER) && _MSC_VER <= 1200 // VC6
   return (COLint32) InterlockedExchange((LONG*)pVal, (LONG)New);
#elif defined(_MSC_VER) && _MSC_VER > 1200
   return (COLint32) InterlockedExchange((LONG volatile *)pVal, (LONG)New);
#elif defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 )
   return __sync_lock_test_and_set(pVal, New);
#else
   COLint32 Temp = *pVal;
   *pVal = New;
   return Temp;
#endif
}

COLint32 COLatomicIncrement32(COLint32 volatile* pVal)
{
#if defined(_MSC_VER) && _MSC_VER <= 1200 // VC6
   return (COLint32) InterlockedIncrement((LONG*)pVal);
#elif defined(_MSC_VER) && _MSC_VER > 1200
   return (COLint32) InterlockedIncrement((LONG volatile *)pVal);
#elif defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 )
   return __sync_add_and_fetch(pVal, 1);
#else
   return ++(*pVal);
#endif
}

COLint32 COLatomicDecrement32(COLint32 volatile* pVal)
{
#if defined(_MSC_VER) && _MSC_VER <= 1200 // VC6
   return (COLint32) InterlockedDecrement((LONG*)pVal);
#elif defined(_MSC_VER) && _MSC_VER > 1200
   return (COLint32) InterlockedDecrement((LONG volatile *)pVal);
#elif defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 )
   return __sync_add_and_fetch(pVal, 1);
#else
   return ++(*pVal);
#endif
}
