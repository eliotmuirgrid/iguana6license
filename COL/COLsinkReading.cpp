//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsinkReading
//
// Description:
//
// Implementation
//
// Author: Ryan Dewsbury
//
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include <COL/COLsinkReading.h>
#include <COL/COLsource.h>
#include <COL/COLbinaryBuffer.h>

COLsinkReading::COLsinkReading()
: pSource(NULL),
  BytesRead(0)
{
   pBuffer = new COLbinaryBuffer();
}

COLsinkReading::~COLsinkReading()
{
   delete pBuffer;
}

bool COLsinkReading::readAll( COLsource& Source )
{
   pSource = &Source;
   return true;
}

void COLsinkReading::read
( 
   const COLint8* cpBuffer, 
   COLuint16 SizeOfBuffer 
)
{
   while( SizeOfBuffer > pBuffer->size() )
   {
      COLPRECONDITION( pSource != NULL );
      pSource->writeChunk();
   }
   memcpy( (void*)cpBuffer, pBuffer->start(), SizeOfBuffer );
   pBuffer->removeChunkFromStart( SizeOfBuffer );
   BytesRead += SizeOfBuffer;
}

const char* COLsinkReading::readString()
{
   // TODO - looking for NULL characters can be quite expensive?  We could consider more of a Pascal
   // type string approach of prepending the length of the string before the data?  Penalty is that we
   // end up with more overhead?

   //look for a null character
   const char* pStringEnd = (const char*)memchr( pBuffer->start(), 0, pBuffer->size() );

   /*
   if (pStringEnd == NULL)
   {
   \   bool TEMP = pSource->isEndOfSource();
   }
   */

   while( pStringEnd == NULL && !pSource->isEndOfSource()) //not found, so request another chunk
   {
      COLPRECONDITION( pSource != NULL );
      pSource->writeChunk();
      pStringEnd = (const char*)memchr( pBuffer->start(), 0, pBuffer->size() );
   }
   
   COLPRECONDITION( pStringEnd != NULL );
   const char* pString = pBuffer->start();
   pBuffer->removeChunkFromStart( pStringEnd - pString + 1 );
   BytesRead += pStringEnd - pString + 1;
   return pString;
}

COLuint32 COLsinkReading::write
( 
   const void* cpBuffer,
   COLuint32 SizeOfBuffer
)
{
   pBuffer->addChunk( (COLint8*)cpBuffer, SizeOfBuffer );

   return SizeOfBuffer;
}
