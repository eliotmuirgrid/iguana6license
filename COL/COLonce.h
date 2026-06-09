#ifndef __COL_ONCE_H__
#define __COL_ONCE_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2012 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLonce
//
// Description:
//
// COLonce - some nonsense that you should not need to use if you are writing a well designed
// application which doesn't require singletons and static initialization
//
// Author: Steven Dodd
// Date:   Thursday, March 3rd, 2011 @ 10:11:13 AM
//---------------------------------------------------------------------------

#include <COL/COLminimumInclude.h>

typedef void (*COLinitRoutine_t)();

#ifdef CHM_WIN

typedef COLint32 COLonce_t;
#define COL_ONCE_INIT 0

#ifdef COL_ONCE_USE_CRITICAL_SECTION
   void COLonceInit();
#endif

#else
#   include <pthread.h>
    typedef pthread_once_t COLonce_t;
#   define COL_ONCE_INIT PTHREAD_ONCE_INIT;

#endif

// Supports thread-safe one-time initialization, by ensuring that
// the given initialization routine is called only once.
// 
// pOnceControl must be initialized with COL_ONCE_INIT, must not have
// automatic storage duration, and should be declared volatile.
//
// The initialization routine must not throw C++ exceptions.
//
// Returns 0 on success, and a system specific error code on failure.
int COLonce(COLonce_t volatile* pOnceControl, COLinitRoutine_t InitRoutine);

#endif // end of defensive include
