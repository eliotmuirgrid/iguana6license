//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLmemmem
//
// Description:
//
// Implementation
//
// Author: Nasron Cheong
// Date:   Fri 01/16/2004 
//
//---------------------------------------------------------------------------

#include "COLprecomp.h"
#pragma hdrstop

void* COLmemmem(const void* pHayStack, COLuint32 SizeOfHaystack, const void* pNeedle, COLuint32 SizeOfNeedle)
{
   if (!SizeOfNeedle || !SizeOfHaystack || !pHayStack || !pNeedle)
   {
      return NULL;
   }
   const COLint8* pHayStackData = (const COLint8*)pHayStack;
   const COLint8* pNeedleData = (const COLint8*)pNeedle;
   
   //this isn't a great search algorithm, but it should do
   while (pHayStackData < pHayStackData+SizeOfHaystack)
   {
      const COLint8* pFound = (const COLint8*)::memchr(pHayStackData,*pNeedleData,SizeOfHaystack);
      if (pFound)
      {
         // check if its the substring. If it isn't, update the search pointer 
         // and try again
   
         //note, no truncation here
         unsigned long AmountLeft = (unsigned long)pHayStackData;
         AmountLeft += SizeOfHaystack;
         AmountLeft -= (unsigned long)pFound;

         if ( AmountLeft < SizeOfNeedle )
         {
            //the pattern is bigger than whatever data is left
            return NULL;
         }
         else
         {
            //check if it is the substring
            if ( ::memcmp(pFound,pNeedleData,SizeOfNeedle) == 0 )
            {
               //it is...
               return (void*)pFound;
            }
            else
            {
               //move on by one ahead of the found index to catch overlapping needles
               SizeOfHaystack -= (pFound+1)-pHayStackData;
               pHayStackData = pFound+1;
            }
         }
      }
      else
      {
         break;
      }
   }
   return NULL;
}
