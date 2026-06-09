#ifndef __DRG_PRECOMP_H__
#define __DRG_PRECOMP_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Description:
//
// Precompiled header file for the DRG library.
//
// Author:   Eliot Muir
// Date:     Tuesday, May 13th, 2008 @ 02:21:22 PM
//
//---------------------------------------------------------------------------

#include <COL/COLminimumInclude.h>

// Uncomment the next define to switch on error logging.  Please
// remember never to check this file into CVS with this define switched on.
//#define DRG_DEBUG

#ifdef DRG_DEBUG
#   define DRG_LOG(Message) { COLcout << __FILE__ << ':' << __LINE__ << ' ' << Message << ::newline; }
#else
#   define DRG_LOG(Message)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <COL/COLerror.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#endif // end of defensive include
