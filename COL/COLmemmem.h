#ifndef __COL_MEMMEM_H__
#define __COL_MEMMEM_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLmemmem
//
// Description:
//
// A memory searching function, to look for patterns in a given region of memory
// This is an implementation of the gnu ::memmem function, but we do it here
// for platforms that don't have it.
//
// Author: Nasron Cheong
// Date:   Fri 01/16/2004 
//---------------------------------------------------------------------------

// WHY DID WE IMPLEMENT OUR OWN VERSION OF MEMMEM?
//COLmemmem is a function that finds the first position of a byte array within a byte array (a byte array substring finder, except it deals with byte arrays and not strings).  
//
//COLmemmem is a GNU C library extension method (not part of the core library).  Therefore it's unsupported on many system and even on GNU Linux it 
//was implemented with the parameters interchanged (so the interface was incompatible with itself for a few years).  
//
//It's been implemented in our core library, and is used by: 
//
//Dependencies: (as of time of writing)
//
//DQUmessageheader1dot1.cpp
//DQUmessageheader1dot2.cpp
//LLPfullparser.cpp
//Recommendations:
//
//Feel free to use this function if you need it.  If you are dealing with strings just use strstr.  This function is a useful one and we should plan to always support it in our code base.  
//*It would be nice to organize functions like this better.
//
// Finds the pNeedle in the pHayStack.
// Returns NULL if not found, or if SizeOfNeedle or SizeOfHaystack is 0.
//
void* COLmemmem(const void* pHayStack, COLuint32 SizeOfHaystack, const void* pNeedle, COLuint32 SizeOfNeedle);

#endif // end of defensive include
