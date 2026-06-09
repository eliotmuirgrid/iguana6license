//---------------------------------------------------------------------------
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLutils
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Thu  6 Nov 2014 12:08:37 EST
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include "COLvar.h"
#include "COLvarUtils.h"
#include "COLlog.h"
COL_LOG_MODULE;

void COLsetDefault(COLvar* pMap, const COLstring& Param, const char* pDefaultValue){
   COL_FUNCTION(COLsetDefault-const-char);
   COL_VAR2(Param,pDefaultValue);
   COL_VAR(pMap->json(true));
   if (!pMap->exists(Param)){
      pMap->operator [](Param) = pDefaultValue;
   }
   COL_VAR(pMap->json(true));
}

void COLsetDefault(COLvar* pMap, const COLstring& Param, const COLstring& DefaultValue){
   COL_FUNCTION(COLsetDefault-string);
   COL_VAR2(Param,DefaultValue);
   COL_VAR(pMap->json(true));
   if (!pMap->exists(Param)){
      pMap->operator [](Param) = DefaultValue;
   }
   COL_VAR(pMap->json(true));
}

void COLsetDefault(COLvar* pMap, const COLstring& Param, int DefaultValue){
   COL_FUNCTION(COLsetDefault-int);
   COL_VAR2(Param,DefaultValue);
   COL_VAR(pMap->json(true));
   if (!pMap->exists(Param)){
      pMap->operator [](Param) = DefaultValue;
   }
   COL_VAR(pMap->json(true));
}

void COLsetDefault(COLvar* pMap, const COLstring& Param, bool DefaultValue){
   COL_FUNCTION(COLsetDefault-bool);
   COL_VAR2(Param,DefaultValue);
   COL_VAR(pMap->json(true));
   if (!pMap->exists(Param)){
      pMap->operator [](Param) = DefaultValue;
   }
   COL_VAR(pMap->json(true));
}

void COLsetDefaultMap(COLvar* pMap, const COLstring& Param){
   COL_FUNCTION(COLsetDefault);
   COL_VAR(Param);
   COL_VAR(pMap->json(true));
   if (!pMap->exists(Param)){
      pMap->operator [](Param).setMapType();
   }
   COL_VAR(pMap->json(true));
}

void COLsetDefaultArray(COLvar* pMap, const COLstring& Param){
   COL_FUNCTION(COLsetDefault);
   COL_VAR(Param);
   COL_VAR(pMap->json(true));
   if (!pMap->exists(Param)){ 
      pMap->operator [](Param).setArrayType();
   }
   COL_VAR(pMap->json(true));
}

const COLstring& COLvarGet(const COLvar& Map, const COLstring& Param, const COLstring& Default){
   COL_FUNCTION(COLvarGet);
   if (Map.exists(Param)){
      return Map[Param].str();
   } else {
      return Default;
   }
}

int COLvarGet(const COLvar& Map, const COLstring& Param, int Default){
   COL_FUNCTION(COLvarGet);
   if (Map.exists(Param)){
      return Map[Param];
   } else {
      return Default;
   }
}

bool COLvarCast(const COLvar& Value, int* pToInt) {
   COL_FUNCTION(COLvarCast-toInt);
   COL_VAR(Value);
   if (Value.isInteger()) {
      COL_TRC("It's already an integer");
      *pToInt = Value.asInt();
      return true;
   }
   if (Value.isDouble()) {
      int Floored = int(Value.asDouble());
      if (Floored == Value.asDouble()) {
         *pToInt = Floored;
         return true;
      } else {
         return false;
      }
   }
   if (Value.isString()) {
      // The hardest case... parse various integer representations
      const COLstring& ValueStr = Value.str();
      int StringLength = ValueStr.size();
      if (ValueStr.is_null()) {
         COL_TRC("Null string");
         return false;
      }
      int StringIndex = 0;
      bool IsNegative = false;
      int Base = 10;
      if (ValueStr[StringIndex] == '-') {
         StringIndex++;
         IsNegative = true;
      }
      if (StringIndex >= StringLength) {
         COL_TRC("No digits");
         return false;
      }
      if (ValueStr[StringIndex] == '0') {
         // Look-ahead parsing
         if (StringIndex + 1 >= StringLength) {
            COL_TRC("Single 0 followed by nothing");
            *pToInt = 0;
            return true;
         }
         if (ValueStr[StringIndex + 1] == 'x') {
            // 0x ... - we're in hex at this point
            StringIndex += 2;
            Base = 16;
         } else {
            // TODO : Should we even support octal?
            // 0 ... - we're in octal at this point
            StringIndex++;
            Base = 8;
         }
      }
      if (StringIndex >= StringLength) {
         COL_TRC("No digits");
         return false;
      }
      // Now we're into the digits, finally - starting accumulating values
      int Accumulation = 0;
      while (StringIndex < StringLength) {
         char c = ValueStr[StringIndex];
         int CurrentDigitValue;
         if      (c >= '0' && c <= '9') { CurrentDigitValue = c - '0';      }
         else if (c >= 'a' && c <= 'f') { CurrentDigitValue = c - 'a' + 10; } 
         else if (c >= 'A' && c <= 'F') { CurrentDigitValue = c - 'A' + 10; }
         else {
            COL_TRC("Invalid digit: " << c);
            return false;
         }
         if (CurrentDigitValue >= Base) {
            COL_TRC("Invalid digit for base " << Base << ": " << c);
            return false;
         }
         Accumulation = Base * Accumulation + CurrentDigitValue;
         StringIndex++;
      }
      *pToInt = IsNegative ? -Accumulation : Accumulation;
      return true;
   }

   COL_TRC("Not going to cast COLvar::type = " << Value.type());
   return false;
}

bool COLvarCast(const COLvar& Value, bool* pToBool) {
   COL_FUNCTION(COLvarCast-toBool);
   COL_VAR(Value);
   if (Value.isInteger() || Value.isDouble()) {
      COL_TRC("Not casting numeric types to bool");
      return false;
   }
   if (Value.isBool()) {
      COL_TRC("It's already a boolean");
      *pToBool = Value.asBool();
      return true;
   }
   if (Value.isString()) {
      // This list isn't extensive, more may be required as the need arises
      const COLstring& ValueStr = Value.str();
#define __stringToBool(str, val)                                  \
      do {                                                        \
         if (ValueStr == str) { *pToBool = val; return true; }    \
      } while (0)
      __stringToBool("t", true);
      __stringToBool("T", true);
      __stringToBool("f", false);
      __stringToBool("F", false);
      __stringToBool("true", true);
      __stringToBool("True", true);
      __stringToBool("TRUE", true);
      __stringToBool("false", false);
      __stringToBool("False", false);
      __stringToBool("FALSE", false);
#undef __stringToBool
      COL_TRC("Unrecognized string: " << ValueStr);
      return false;
   }

   COL_TRC("Not going to cast COLvar::type = " << Value.type());
   return false;
}

bool COLvarCast(const COLvar& Value, COLstring* pToString) {
   COL_FUNCTION(COLvarCast-toString);
   COL_VAR(Value);
   if (Value.isScalar()) {
      *pToString = Value.asString();
      return true;
   }

   COL_TRC("Not going to cast COLvar::type = " << Value.type());
   return false;
}
