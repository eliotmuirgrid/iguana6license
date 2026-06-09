//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLmath
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Fri 01/16/2004 
//
//---------------------------------------------------------------------------

#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLmath.h>
#include <time.h>
#include <ctype.h>

#ifndef WIN32
#include <wchar.h>
#endif
void COLmath::resetRandomGenerator()
{
   // we use the machine type call
   srand((unsigned int)time(NULL));
}

COLuint32 COLmath::randomNumber(COLuint32 Max)
{
   COLPRECONDITION(Max < RAND_MAX);
   return rand() % Max;
}

// It's okay to have this here.
char COLmath::toUpper(char Character)
{
   // Cannot error check since there is no return code for an error
   return toupper(Character);
}

COLint64 COLmath::atoi64(const char* pNumberString)
{
#ifdef WIN32
   return _atoi64( pNumberString );
#elif defined(CHM_HPUX) || defined(CHM_HPUX_ITANIUM)
   COLint8 AsciiCode0 = 48;
   COLint64 NewValue = 0;
   COLint8 SignOfNewValue = +1;

   COLstring NumberString(pNumberString);
   NumberString = NumberString.strip(COLstring::Leading, ' ');
   COLuint32 CountOfDigit = NumberString.size();

   if (NumberString[0] == '-')
   {
      CountOfDigit--;
      NumberString.remove(0,1);
      SignOfNewValue = -1;
   }

   // check the string for invalid characters.  The behaviour of the WIN32 and
   // POSIX APIs is to drop the invalid character and everything after it; we'll
   // do the same.
   for (COLuint32 CharacterIndex = 0; CharacterIndex < CountOfDigit; CharacterIndex++)
   {
      COLint64 CurrentDigit = (COLint8)NumberString[CharacterIndex] - AsciiCode0;
      if(CurrentDigit < 0 || CurrentDigit > 9)
      {
         // an invalid character exists in the string.
         // drop this character and everything after it.
         NumberString.remove(CharacterIndex);
         CountOfDigit = CharacterIndex;
         break;
      }
   }

   // convert the string into a number
   for(COLuint32 DigitIndex = 0; DigitIndex < CountOfDigit; DigitIndex++)
   { 
      COLint64 CurrentDigit = (COLint8)NumberString[DigitIndex] - AsciiCode0;         
      COLint64 PowerBase = power(10, CountOfDigit - DigitIndex - 1);
      NewValue += (CurrentDigit*PowerBase);
   } 

   return (SignOfNewValue*NewValue);
#else
   return atoll( pNumberString );
#endif
}

COLuint64 COLmath::parseUint64(const char* pNumberString, COLuint32 CountOfDigit)
{
   COLint8 AsciiCode0 = 48;
   COLint8 AsciiCode9 = 57;
   COLuint64 NewValue = 0;

   // If the number has greater than 20 digits, then it is too large.
   // If the number is 20 digits and starts with a digit > 1, then
   // it is too large.
   if (CountOfDigit > 20 ||
      (CountOfDigit == 20 && (COLuint8)pNumberString[0] > AsciiCode0+1 && (COLuint8)pNumberString[0] <= AsciiCode9))
   {
      return COL_UINT_64_MAX;
   }

   // convert the string into a number
   for (COLuint32 DigitIndex = 0; DigitIndex < CountOfDigit; DigitIndex++)
   {
      COLint8 CurrentChar = (COLint8)pNumberString[DigitIndex];

      // Check if the current character is invalid
      if (CurrentChar < AsciiCode0 || CurrentChar > AsciiCode9)
      {
         return 0;
      }

      COLuint64 CurrentDigit = CurrentChar - AsciiCode0;         
      COLuint64 PowerBase = power(10, CountOfDigit - DigitIndex - 1);
      COLuint64 CurrentDigitValue = CurrentDigit*PowerBase;

      // If adding CurrentDigitValue to NewValue would cause an integer
      // overflow, then just return COL_UINT_64_MAX.
      if (COL_UINT_64_MAX - NewValue < CurrentDigitValue)
      {
         return COL_UINT_64_MAX;
      }

      NewValue += CurrentDigitValue;
   } 

   return NewValue;
}

COLint64 COLmath::power(const COLint64 Base, const COLint64 Exponent)
{
   COLPRECONDITION(Exponent >= 0);
   COLint64 Result = 1;
   for (COLuint32 Power = 0; Power < Exponent; Power++)
   {
      Result *= Base;
   }
   return Result;
}

void COLmath::incMultiBytePointer(COLuint8*& pPointer)
{
#ifndef CHM_MULTI_BYTE_SUPPORT_OFF

#ifdef WIN32
   // TODO - it would be interesting to profile relative
   // performance on these functions in Windows.  Some
   // day we should perhaps add a compile flag to not support
   // multi-byte operations for performance.
   pPointer = _mbsinc(pPointer);
#else
   int Increment = mblen((const char*)pPointer, MB_CUR_MAX);
   if (Increment == -1)
   {
      COL_ERROR_STREAM("Invalid multibyte sequence", COLerror::PreCondition);
   }
   if (Increment == 0)
   {
      Increment = 1;
   }
   pPointer = pPointer + Increment;
#endif

#else
   pPointer = pPointer + sizeof(char);
#endif
}

COLuint32 COLmath::multiByteLength(COLuint8*& pPointer)
{
#ifndef CHM_MULTI_BYTE_SUPPORT_OFF
   int Length = mblen((const char*) pPointer, MB_CUR_MAX);

   if (Length == -1)
   {
      COL_ERROR_STREAM("Invalid multibyte sequence", COLerror::PreCondition);
   }

   if (Length == 0)
   {
      Length = 1;
   }

   return Length;
#else
   return sizeof(char);
#endif
}
