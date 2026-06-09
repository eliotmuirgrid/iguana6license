#ifndef __COL_SINK_WINDOWS_DEBUG_H__
#define __COL_SINK_WINDOWS_DEBUG_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsinkWindowsDebug
//
// Description:
//
// Redirects to Win32 debug string call.  This is handy for debugging
// windows applications.  An example of using it is:
//
// COLcout.setSink(new COLsinkWindowsDebug(), true);
//
// Author: Nasron Cheong
// Date:   Feb 2004 
//
//
//---------------------------------------------------------------------------

#include "COLsink.h"

#ifdef WIN32

class COL_DLL COLsinkWindowsDebug : public COLsink
{
public:
   COLsinkWindowsDebug() {}
   virtual ~COLsinkWindowsDebug() {}

   virtual COLuint32 write(const void* cpBuffer, 
                           COLuint32   SizeOfBuffer);
};

#else
#include <COL/COLsinkStandard.h>
//use stderr if in posix
typedef COLsinkStandardErr COLsinkWindowsDebug;
#endif

#endif // end of defensive include
