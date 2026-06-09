#ifndef _CMD_PRECOMP_H__
#define _CMD_PRECOMP_H__
//------------------------------------------------------------------------
// Copyright (C) 1997-2005 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  CMDprecomp
// 
// Description: Precompiled header file
//
// Author: Roman Leifer
// Date:   Thu 19/08/2002 
//
//---------------------------------------------------------------------------


#define CHM_NO_BSTR

#include <COL/COLrefCounted.h>
#include <COL/COLminimumInclude.h>
#include <COL/COLstring.h>
#include <string.h>

//#define CMD_DEBUG

#ifdef CMD_DEBUG
#   define CMD_LOG(Message) { COLcout << __FILE__ << ":" << __LINE__ << " " << Message << ::newline; }
#else
#   define CMD_LOG(Message)
#endif

#endif // end of defensive include
