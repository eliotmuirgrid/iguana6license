//---------------------------------------------------------------------------
// Copyright (C) 1997-2011 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLmutex
//
// Description:
//
// Implementation.  Renamed from COLcriticalSection.cpp.
//
// Author: Eliot Muir
// Date:   Wed 09/23/1998 
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLmutex.h>

#ifdef WIN32
class COLmutexPrivate
{
public:
   COLmutexPrivate() 
   {
      ::InitializeCriticalSection(&CriticalSectionHandle);  // void function - no return code      
   }

   ~COLmutexPrivate()
   {
      ::DeleteCriticalSection(&CriticalSectionHandle);      // void function - no return code
   }

   bool tryLock()
   {
      return !!::TryEnterCriticalSection(&CriticalSectionHandle);
   }

   void lock()
   {
      ::EnterCriticalSection(&CriticalSectionHandle);       // void function - no return code
   }

   void unlock()
   {
      ::LeaveCriticalSection(&CriticalSectionHandle);       // void function - no return code
   }

   void* handle()
   {
      return (void*)&CriticalSectionHandle;
   }

   CRITICAL_SECTION CriticalSectionHandle;
};
#else  // POSIX

#include <pthread.h>
#include <stdio.h>
#include <errno.h>

class COLmutexPrivate
{
public:
   COLmutexPrivate();
   ~COLmutexPrivate();

   bool tryLock();
   void lock();
   void unlock();
   
   void* handle()
   {
      return (void*)&Mutex;
   }

private:
   pthread_mutex_t Mutex;

   // private - not allowed:
   COLmutexPrivate(const COLmutexPrivate& Orig);
   COLmutexPrivate& operator=(const COLmutexPrivate& Orig); 
};

inline COLmutexPrivate::COLmutexPrivate()
{
   //No point checking error codes here - see ticket #10168
   pthread_mutexattr_t recursiveAttr;
   pthread_mutexattr_init(&recursiveAttr);
   pthread_mutexattr_settype(&recursiveAttr, PTHREAD_MUTEX_RECURSIVE);
   pthread_mutex_init(&Mutex, &recursiveAttr);
   pthread_mutexattr_destroy(&recursiveAttr);
}

inline COLmutexPrivate::~COLmutexPrivate()
{
   //No point checking error codes here - see ticket #10168
   pthread_mutex_destroy(&Mutex); 
}

inline void COLmutexPrivate::lock()
{
   //No point checking error codes here - see ticket #10168
   pthread_mutex_lock(&Mutex);
}

inline bool COLmutexPrivate::tryLock()
{
   return pthread_mutex_trylock(&Mutex) == 0;
}

inline void COLmutexPrivate::unlock()
{
   //No point checking error codes here - see ticket #10168
   pthread_mutex_unlock(&Mutex);
}
#endif  // end of POSIX implementation.

COLmutex::COLmutex() 
{
   pMember = new COLmutexPrivate();
}

COLmutex::~COLmutex() 
{
   delete pMember;
}

bool COLmutex::tryLock()
{
   return pMember->tryLock();
}

void COLmutex::lock()
{
   pMember->lock();
}

void COLmutex::unlock()
{
   pMember->unlock();
}

void* COLmutex::handle()
{
   return pMember->handle();
}

// In ticket #10168 - comment from Andrew Vajoczki 
// Mutexes created with PTHREAD_MUTEX_RECURSIVE will never return EDEADLK in 
// locking a single mutex by design (it's recursive).
//
// Multi-lock deadlock detection is not done in any pthread implementation that 
// I'm am aware of. It would be too computationally expensive to implement in a
// standard threading library, and would slow multithreaded applications down considerably.
//
// Checking the return code of pthread_mutex_init could be done, but it is unlikely to 
// ever fail because most major operating system's pthread libraries implement it in a
// way that doesn't require any memory allocation or operating system resources.
 
