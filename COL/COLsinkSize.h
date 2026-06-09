#ifndef __COL_SINK_SIZE_H__
#define __COL_SINK_SIZE_H__
//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsinkSize
//
// Description:
//
// Calculates the size of the data written to it.
//
// Author: Ryan Dewsbury
//

//---------------------------------------------------------------------------

#include "COLsink.h"

class COLsinkSize : public COLsink  
{
public:
   COLsinkSize();
   virtual ~COLsinkSize();

   virtual COLuint32 write( const void* cpBuffer, 
                            COLuint32 SizeOfBuffer );

   COLuint32 size(){ return Size; }
private:
   COLuint32 Size;
};

#endif // end of defensive include
