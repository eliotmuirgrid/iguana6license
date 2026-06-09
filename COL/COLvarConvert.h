#ifndef __COL_VAR_CONVERT_H__
#define __COL_VAR_CONVERT_H__
//-----------------------------------------------------------------------------
// Copyright (C) 1997-2014 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  COLvarConvert
//
// Author:  Bret Dawson
//
// Description:
//
//   A simple set of functions for converting standard containers full of strings
//   into COLvars, and vice versa.
//
//-----------------------------------------------------------------------------

#include "COLlist.h"
#include "COLhashmap.h"
#include "COLvector.h"
#include "COLvar.h"

// TODO - lets rename this file to COLvarUtils.h - Eliot

void COLvarConvertFromHashMap(const COLhashmap<COLstring, COLstring>& TheMap, COLvar* pTheVar); 
void COLvarConvertToHashMap(const COLvar& TheVar, COLhashmap<COLstring, COLstring>* pTheMap);
void COLvarConvertFromVector(const COLvector<COLstring>& TheVector, COLvar* pTheVar);
// NOTE : Does not check that the individual elements are of type string
void COLvarConvertToVector(const COLvar& TheVar, COLvector<COLstring>* pTheVector);

// NOTE : This returns true for empty arrays, which may or may not be desirable in certain usages
bool COLvarIsVector(const COLvar& TheVar);

// Fills a vector with the keys used in the COLvar
void COLvarKeys(const COLvar& TheVar, COLvector<COLstring>* pKeys);
// Populates a map using the strings from the vector as keys
void COLvarAddKeys(const COLvector<COLstring> Keys, COLvar* pTheVar, bool SkipEmpties = false);

//Reverses the order of the entries in a COLvar array.
void COLvarReverseVarArray(COLvar* pTheVar);

// These functions will throw a COLerror if the required named variable does not exist.  There might be a case for
// adding these as methods to COLvar itself one day.
COLstring COLvarRequiredString(const COLvar& Args, const COLstring& Name);
bool      COLvarRequiredBool  (const COLvar& Args, const COLstring& Name);
int       COLvarRequiredInt   (const COLvar& Args, const COLstring& Name); 

#endif  // end of defensive include

