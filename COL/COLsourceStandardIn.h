#ifndef __COL_SOURCE_STANDARD_IN_H__
#define __COL_SOURCE_STANDARD_IN_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLsourceStandardIn
//
// Description:
//
//   A COLsource which wraps up stdin.
//
// TODO: Currently this source just read all of the data from stdin until
// it can't read anymore. This is NOT good for user input because _read will
// never return 0 and so isEndOfSource is never set. We would have to 
// add COLreadable (?) functionality to this so we could:
//   MyVar = MyStandardIn.read(); // or
//   MyStandardIn.read(Buffer, BufferSize); // or
//   MyStandardIn >> MyVar;
//   
// Author: Chris Yong-Set
// Date:   Tue 05/03/2005
//
//
//---------------------------------------------------------------------------
#include <COL/COLminimumInclude.h>

#include <COL/COLsource.h>

class COLsourceStandardIn:
   public COLsource
{
public:

   // The file descriptor for stdin
   static COLuint32 STDIN;

   COLsourceStandardIn(COLsink* pNext=NULL) : COLsource(pNext) {};
   virtual ~COLsourceStandardIn() {};

   /* COLsource interface */
   virtual void writeChunk(COLuint32 ChunkSize); 
   virtual void resetSourcePosition();

   // Probably something better we could do...
   COLuint32 read(void* pBuffer, COLuint32 SizeOfBuffer);
   
   static void setBinaryMode(bool Set=true);

private:
   COLsourceStandardIn(const COLsource& Source);
   COLsourceStandardIn& operator=(const COLsource& Source);
};

#endif // end of defensive include

