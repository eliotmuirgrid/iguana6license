//---------------------------------------------------------------------------
// Copyright (C) 1997-2013 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: 
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Mon  4 Nov 2013 09:45:55 EST
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include "COLfile.h"
#include "COLlog.h"
COL_LOG_MODULE;

#include <fcntl.h>
#include <stdio.h>

#ifdef WIN32
#   include <io.h>
#   define COLopen _open
#   define COLlseek64 _lseeki64
#   define COLclose   _close
#   define COLwrite   _write
#   define COLread    _read
#else // POSIX
#   include <sys/stat.h>
#   include <sys/types.h>
#   include <string.h>
#   include <unistd.h>
#   include <errno.h>
// Only Windows is dumb enough to define the O_BINARY flag.
#   define O_BINARY 0
#   define COLopen open
#   define COLlseek64 lseek
#   define COLclose   close
#   define COLwrite   write
#   define COLread    read
#endif

/*

#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>


#ifdef WIN32
#   include <io.h>
#   include <direct.h>
#   include <winbase.h>
    const COLuint32 FIL_READABLE = 4;
    const COLuint32 FIL_WRITABLE = 2;
#else // POSIX
#   include <errno.h>
#   include <limits.h>
#   include <pwd.h>
#   include <unistd.h>

#   ifdef CHM_MAC
#      include <mach-o/dyld.h>
#   endif

#   if defined(CHM_HPUX_ITANIUM) || defined(CHM_HPUX)
#      define _PSTAT64
#      include <sys/pstat.h>
#   endif

    const COLuint32 FIL_READABLE = R_OK;
    const COLuint32 FIL_WRITABLE = W_OK;
    const COLuint32 FIL_EXECUTABLE = X_OK;

    // Should be thread safe in this context #10052
    static COLstring s_ExecutableFileDirectory;
#endif // WIN32
*/
// This version of FILreadFileIntoString doesn't throw exceptions.
bool COLfile::fileToString(const COLstring& FileName, COLstring* pDestination, int* pErr){
   COL_FUNCTION(COLfile::fileToString);
   int FileHandle = COLopen(FileName.c_str(), O_RDONLY | O_BINARY);
   if (FileHandle == -1){
      *pErr = errno;
      return false;
   }
   COLint64 SizeOfFile = COLlseek64(FileHandle, 0, SEEK_END);
   if (SizeOfFile == -1){
      *pErr = errno;
      COLclose(FileHandle);
      return false;
   }
   COL_VAR(SizeOfFile);
   pDestination->setCapacity(SizeOfFile+1);
   COL_VAR(pDestination->capacity());
   if (-1 == COLlseek64(FileHandle, 0, SEEK_SET)){
      *pErr = errno;
      COLclose(FileHandle);
      return false;
   }
   COLint64 AmountRead = ::COLread(FileHandle, pDestination->get_buffer(), SizeOfFile);
   COL_VAR(AmountRead);
   if (AmountRead < 0){
      *pErr = errno;
      COLclose(FileHandle);
      return false;
   }
   // AmountRead could be less than the SizeOfFile in a pathological case.
   pDestination->setSize(AmountRead);

   COLclose(FileHandle);
   return true;
}

COLstring COLfile::err(int Err){
   COL_FUNCTION(COLfile::err);
   // In theory we could define our own additional error codes here.
   return strerror(Err);
}
