#ifndef __FIL_BINARY_FILE_TRUNCATED_H__
#define __FIL_BINARY_FILE_TRUNCATED_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILbinaryFileTruncated
//
// Description:
//
// This is an extension of FILbinaryFile.  It's meant for use with
// applications that need to have truncated log files.  The write
// operation checks the size of the file and will truncate it if it gets
// too big.
//
// TODO - FILbinaryFileTruncated probably doesn't really deserve a place
// in the FIL library which is meant to be a lean building block library.  This
// is more of an application concept.  One day this file should be shifted out
// of FIL.
//
// Author: Eliot Muir
// Date:   Fri 01/16/2004 
//
//---------------------------------------------------------------------------

#include <FIL/FILbinaryFile.h>

class COL_DLL FILbinaryFileTruncated : public FILbinaryFile
{
public:
   FILbinaryFileTruncated(FILtype Type=Buffered);
   virtual ~FILbinaryFileTruncated();

   // we over-ride the the open method to ensure we store the full file name
   // to make it 
   virtual void open(const COLstring& FileName, FILbinaryFile::FILmode Mode);
   // This method will truncate the log file if it exceeds the maximum size.
   virtual COLuint32 write(const void* pBuffer, COLuint32 SizeOfBuffer);

   void setMaximumSize(FILindex iMaxSize) { MaxSize = iMaxSize; }
   FILindex maximumSize() const { return MaxSize; }
private:
   FILindex MaxSize;
   FILbinaryFileTruncated(const FILbinaryFileTruncated& Orig); // not allowed
   FILbinaryFileTruncated& operator=(const FILbinaryFileTruncated& Orig); // not allowed
};

#endif // end of defensive include
