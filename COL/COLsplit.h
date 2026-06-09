#ifndef __COL_SPLIT_H__
#define __COL_SPLIT_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2012 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsplit
//
// Description:
//
// COLsplit class.
//
// Author: Dmitri Klimenko
// Date:   Monday, January 25th, 2010 @ 11:17:51 AM
//
//---------------------------------------------------------------------------

#include "COLvar.h"
#include "COLvector.h"
#include "COLstring.h"

void COLsplit(COLvector<COLstring>* pOutPartList, const COLstring& String, const COLstring& Separator);
void COLsplit(COLvar* pOutPartList, const COLstring& String, const COLstring& Separator);
void COLjoin(COLstring* pJoined, const COLvector<COLstring>& PartList, const COLstring& Separator, bool Quote = false);
void COLjoin(COLstring* pJoined, const COLvar& PartList, const COLstring& Separator, bool Quote = false);


COLindex COLsplitWhiteSpace(COLvector<COLstring>* pOutPartList, const COLstring& Source, bool KeepSpace=false);


#endif // end of defensive include
