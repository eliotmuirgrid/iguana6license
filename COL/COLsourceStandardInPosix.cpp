//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsourceStandardInPosix
//
// Description:
//
//     The Posix version of COLsourceStandIn. Unix doesn't have the same
// problems that Windows does with the translations and the method for reading
// from stdin is quite different
//
// Author: Chris Yong-Set
// Date:   Tue 05/03/2005
//
//
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include "COLsink.h"
#include "COLsourceStandardIn.h"
#include "COLsimpleBuffer.h"
#include "COLlog.h"
COL_LOG_MODULE;

#include <stdio.h>

// Not really needed for Unix
COLuint32 COLsourceStandardIn::STDIN = 0;

void COLsourceStandardIn::writeChunk(COLuint32 ChunkSize)
{
   COLsimpleBuffer Buffer(ChunkSize);
   COLuint32 AmountRead = fread(Buffer.data(), 1 , ChunkSize, stdin);
   COL_VAR(AmountRead);
   if (feof(stdin))
   {
      setEndOfSource(true);
   }

   next().write(Buffer.data(), AmountRead);
}

void COLsourceStandardIn::resetSourcePosition()
{
   // stdin has no position
}

COLuint32 COLsourceStandardIn::read(void* pBuffer, COLuint32 SizeOfBuffer)
{
   return fread(pBuffer, 1 , SizeOfBuffer, stdin);
}

void COLsourceStandardIn::setBinaryMode(bool Set)
{
   // Irrelevent under Unix. Unix already doesn't translate CRLFs
}
