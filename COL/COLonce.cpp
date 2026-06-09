//---------------------------------------------------------------------------
// Copyright (C) 1997-2012 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLonce
//
// Description:
//
// Implementation
//
// Author: Steven Dodd
// Date:   Friday, October 8th, 2010 @ 04:30:58 PM
//
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include "COLonce.h"
#include "COLatomic.h"

#include <COL/COLlog.h>
#include <COL/COLutils.h>
COL_LOG_MODULE;

#ifdef CHM_WIN

#ifdef COL_ONCE_USE_CRITICAL_SECTION

static CRITICAL_SECTION s_OnceSection;

void COLonceInit()
{
   static bool Initialized = false;
   if (!Initialized)
   {
      ::InitializeCriticalSection(&s_OnceSection);
      Initialized = true;
   }
}

int COLonce(COLonce_t volatile * pOnceControl, void (*InitRoutine)())
{
   // Simplistic, but 100% correct implementation of COLonce.  Requires client to call COLonceInit() 
   // before any threads call COLonce.  This largely defeats the purpose of COLonce(), however, this
   // implementation is useful in debugging the real implementation of COLonce.

   ::EnterCriticalSection(&s_OnceSection);

   if (*pOnceControl == 0)
   {
      try
      {
         InitRoutine();
         *pOnceControl = 1;
      }
      catch (...)
      {
         COLabort();
      }
   }

   ::LeaveCriticalSection(&s_OnceSection);

   return 0;
}

#else // !COL_ONCE_USE_CRITICAL_SECTION

int COLonce(COLonce_t volatile * pOnceControl, COLinitRoutine_t InitRoutine)
{
   COL_FUNCTION(COLonce);
   COL_VAR((COLonce_t*)pOnceControl);

   // Implemented using a simple interlocked spin-lock.  The tri-state lock ensures that
   // threads only spin while waiting for initialization, and never afterwards.
   // 
   // This implementation is subject to priority inversion, and possibly starvation when
   // used by threads with different priorities.  Don't think this is problem for us now.
   // 
   // See http://www.drdobbs.com/cpp/199203083 for other approaches.
   // 
   // TODO - use InitOnceExecute on Windows Vista+, and use this implementation only for
   // Windows 2000/XP.

   for (;;)
   {
      COLonce_t OldOnceControl = COLatomicCompareExchange32(pOnceControl, 1, 0);
      COL_VAR(OldOnceControl);

      if (OldOnceControl == 0) // 0 -> uninitialized
      {
         break;
      }
      else if (OldOnceControl == 1) // 1 -> initialization in progress (spin)
      {
         // avoid spinning using the interlocked instruction, but call it once again on the way
         // out for memory barrier effect
         do 
         {
            COLsleep(0);
         }
         while (*pOnceControl == 1);
      }
      else // 2 -> initialization complete
      {
         return 0; 
      }
   }

   try
   {
      COL_TRC("Running InitRoutine");
      InitRoutine();
   }
   catch (...)
   {
      // Not sure if pthread_once handles C++ exceptions properly on all platforms.
      // To be safe - probably best for InitRoutine to not throw C++ exceptions.
      COLabort();
   }

   COL_TRC("Finished InitRoutine");
   COLatomicExchange32(pOnceControl, 2);
   return 0;
}

#endif // !COL_ONCE_USE_CRITICAL_SECTION

#else // !CHM_WIN

int COLonce(COLonce_t volatile* pOnceControl, void (*pInitRoutine)())
{
   COL_FUNCTION(COLonce);
   COL_VAR((COLonce_t*)pOnceControl);

   return pthread_once((COLonce_t*) pOnceControl, pInitRoutine);
}

#endif // !CHM_WIN
