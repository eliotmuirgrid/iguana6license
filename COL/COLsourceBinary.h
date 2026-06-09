#ifndef __COL_SOURCE_BINARY_H__
#define __COL_SOURCE_BINARY_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsourceBinary
//
// Description:
//
// This is the base class for a binary COLsource.  This is
// basically just a COLsource that supports the notion of 
// a byte index position.  
//
// Author: Rob Moyse
// Date:   Dec 8, 2004
//
//
//---------------------------------------------------------------------------
#include <COL/COLminimumInclude.h>

#include <COL/COLsource.h>

class COL_DLL COLsourceBinary:
   public COLsource
{
public:

   COLsourceBinary( COLsink* ipNext = NULL ) : COLsource(ipNext) {};
   virtual ~COLsourceBinary(){};

   /**
    * Sets the position of the source (written out in writeChunk)
    * to be the given offset.
    */
   virtual void setSourcePosition(COLuint32 ByteOffset) = 0;

   /**
    * Sets the position of the end of the source (written out in writeChunk)
    * to be the given offset.
    *
    * If the ByteOffset is set to 0 then this should mean that the
    * end position is actually the end of the source.. (ie. the
    * source's capacity).
    */
   virtual void setSourceEndPosition(COLuint32 ByteOffset) = 0;

};

#endif // end of defensive include

