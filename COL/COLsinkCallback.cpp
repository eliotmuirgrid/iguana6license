//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsinkCallback
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

#include <COL/COLsinkCallback.h>


COLsinkCallback::COLsinkCallback(flushCallback iCallback)
: Callback(iCallback)
{
   COLPRECONDITION(Callback != NULL);
}


void COLsinkCallback::flush()
{
   COLPRECONDITION(Callback != NULL);
   (*Callback)(LineBuffer.c_str());
   LineBuffer.clear();
}

COLuint32 COLsinkCallback::write(const void* pData, COLuint32 SizeOfBuffer)
{
   const char* pMessageText = (const char*)pData;
   const char* pLineEnd = strpbrk(pMessageText, "\r\n");

   if( pLineEnd == NULL || pLineEnd >= pMessageText + SizeOfBuffer )
   {
      LineBuffer.append(pMessageText, SizeOfBuffer);
   }
   else
   {
      const size_t SizeOfLine = pLineEnd - pMessageText;

      LineBuffer.append(pMessageText, SizeOfLine);
      flush();

      if( *pLineEnd == '\r' && *(pLineEnd+1) == '\n' )
      {
         write(pLineEnd + 2, SizeOfBuffer - SizeOfLine - 2);
      }
      else
      {
         write(pLineEnd + 1, SizeOfBuffer - SizeOfLine - 1);
      }
   }

   return SizeOfBuffer;
}
