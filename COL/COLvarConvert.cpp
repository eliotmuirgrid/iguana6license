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

#include "COLprecomp.h"
#pragma hdrstop

#include "COLvarConvert.h"
#include "COLlog.h"
COL_LOG_MODULE;

void COLvarConvertFromHashMap(const COLhashmap<COLstring, COLstring>& TheMap, COLvar* pTheVar) {
   COL_FUNCTION(COLvarConvertFromHashMap);
   COLvar& TheVar = *pTheVar;
   TheVar.reset();
   TheVar.setMapType();

   for (COLhashmap<COLstring, COLstring>::const_iterator i = TheMap.cbegin(); i != TheMap.cend(); ++i) {
      COL_VAR2(i->first, i->second);
      TheVar[i->first] = i->second;
   }
   COL_VAR(TheVar);
}

void COLvarConvertToHashMap(const COLvar& TheVar, COLhashmap<COLstring, COLstring>* pTheMap) {
   COL_FUNCTION(COLvarConvertToHashMap);
   if (! TheVar.isMap()) {
      return;
   }
   COLhashmap<COLstring, COLstring>& TheMap = *pTheMap;
   TheMap.clear();
   const COLvar::COLvarMap& TheVarMap = TheVar.map();
   for (COLvar::COLvarMap::const_iterator i = TheVarMap.begin(); i != TheVarMap.end(); ++i) {
      TheMap[i->first] = i->second.asString();
   }
}

void COLvarConvertFromVector(const COLvector<COLstring>& TheVector, COLvar* pTheVar) {
   COL_FUNCTION(COLvarConvertFromVector);
   COLvar& TheVar = *pTheVar;
   TheVar.reset();
   TheVar.setArrayType();
   // TODO - could we size the COLvar vector to fit the size of the vector that we are copying from 

   for (COLvector<COLstring>::const_iterator i = TheVector.begin(); i != TheVector.end(); ++i) {
      TheVar.push_back(*i);
   }
}

void COLvarConvertToVector(const COLvar& TheVar, COLvector<COLstring>* pTheVector) {
   COL_FUNCTION(COLvarConvertToVector);
   if (! TheVar.isArray()) {
      return;
   }
   COLvector<COLstring>& TheVector = *pTheVector;
   const COLvar::COLvarVec& TheVarVec = TheVar.array();
   for (COLvar::COLvarVec::const_iterator i = TheVarVec.begin(); i != TheVarVec.end(); ++i) {
      // No checking is done here to see if the type is a string
      TheVector.push_back(i->asString()); 
   }
}

bool COLvarIsVector(const COLvar& TheVar) {
   COL_FUNCTION(COLvarIsVector);
   if (! TheVar.isArray()) {
      return false;
   }
   const COLvar::COLvarVec& TheVarVec = TheVar.array();
   for (COLvar::COLvarVec::const_iterator i = TheVarVec.begin(); i != TheVarVec.end(); ++i) {
      if (! i->isString()) {
         return false;
      }
   }
   // This will therefore return true if the array is empty as well
   return true;
}

void COLvarKeys(const COLvar& TheVar, COLvector<COLstring>* pKeys) {
   COL_FUNCTION(COLvarKeys);
   if (! TheVar.isMap()) {
      return;
   }
   COLvector<COLstring>& Keys = *pKeys;
   Keys.resize(TheVar.size());
   const COLvar::COLvarMap& TheMap = TheVar.map();
   int j = 0;
   for (COLvar::COLvarMap::const_iterator i = TheMap.begin(); i != TheMap.end(); ++i) {
      Keys[j++] = i->first;
   }
}

void COLvarAddKeys(const COLvector<COLstring> Keys, COLvar* pTheVar, bool SkipEmpties) {
   COL_FUNCTION(COLvarAddKeys);
   COLvar& TheVar = *pTheVar;
   for (COLvector<COLstring>::const_iterator i = Keys.begin(); i != Keys.end(); ++i) {
      if (! i->is_null() || ! SkipEmpties) {
         TheVar[*i];
      }
   }
}

void COLvarReverseVarArray(COLvar* pTheVar){
   COL_FUNCTION(COLvarReverseArray);
   COL_VAR( pTheVar->json(true) );
   COLvar& TheVar = *pTheVar;
   COLvar ReversedArray;
   int Size = TheVar.size();
   for(int i = 0; i < Size; i++){
      int CurrentEndIndex = Size - (i + 1); 
      ReversedArray[CurrentEndIndex].swap( &TheVar[i] );
   }
   ReversedArray.swap( pTheVar );
   COL_VAR( pTheVar->json(true) );
}

COLstring COLvarRequiredString(const COLvar& Args, const COLstring& Name){
   COL_FUNCTION(COLvarRequiredString);
   if (!Args.exists(Name)) { throw COLerror(Name + " parameter is required.", COLerror::PreCondition);}
   COL_VAR2(Name, Args[Name]);
   return Args[Name];
}

bool COLvarRequiredBool(const COLvar& Args, const COLstring& Name){
   COL_FUNCTION(COLvarRequiredBool);
   if (!Args.exists(Name)) { throw COLerror(Name + " parameter is required.", COLerror::PreCondition);}
   bool BoolArg = Args[Name]; // convert to a boolean.
   COL_VAR3(Name, Args[Name], BoolArg);
   return BoolArg;
}

int COLvarRequiredInt(const COLvar& Args, const COLstring& Name){
   COL_FUNCTION(COLvarRequiredInt);
   if (!Args.exists(Name)) { throw COLerror(Name + " parameter is required.", COLerror::PreCondition);}
   int intArg = Args[Name].asInt();
   COL_VAR3(Name, Args[Name], intArg);
   return intArg;
}


