#ifndef __FIL_PRECOMP_H__
#define __FIL_PRECOMP_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Description:
//
// Precompiled header file
//
// Author: Eliot Muir
// Date:   Fri 06/08/1999 
//
//---------------------------------------------------------------------------

#if defined(CHM_HPUX) || defined(CHM_HPUX_ITANIUM)
#  define _LARGEFILE64_SOURCE
#elif defined(CHM_MAC) && !defined(CHM_MAC_INTEL)
#  define  stat64  stat  // :-[  stat64() is in Leopard, not Tiger.
#  define lstat64 lstat
#  define fstat64 fstat
#endif

#include <COL/COLerror.h>

#ifndef CHM_WIN
#   include <dirent.h>
#   include <sys/stat.h>
#   include <sys/types.h>
#else
#   include <COL/COLwindows.h>
#endif


#endif // end of defensive include
