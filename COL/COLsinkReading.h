#ifndef __COL_SINK_READING_H__
#define __COL_SINK_READING_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsinkReading
//
// Description:
//
// Author: Ryan Dewsbury
//
//---------------------------------------------------------------------------

#include "COLsink.h"

class COLsource;
class COLbinaryBuffer;

class COLsinkReading : public COLsink
{
public:
   COLsinkReading();
   virtual ~COLsinkReading();

   virtual bool readAll( COLsource& Source );

   void read(const COLint8* cpBuffer, 
             COLuint16      SizeOfBuffer );
   const char* readString();
   COLuint32 bytesRead() const{ return BytesRead; }

private:
   virtual COLuint32 write( const void* cpBuffer, 
                            COLuint32 SizeOfBuffer );

   COLsource* pSource;
   COLbinaryBuffer* pBuffer;
   COLuint32 BytesRead;
};

#endif // end of defensive include

