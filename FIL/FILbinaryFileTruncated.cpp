//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILbinaryFileTruncated
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Fri 01/16/2004 
//
//---------------------------------------------------------------------------
#include "FILprecomp.h"
#pragma hdrstop

#include "FILbinaryFileTruncated.h"
#include "FILfilePath.h"
#include "FILutils.h"


FILbinaryFileTruncated::FILbinaryFileTruncated(FILtype Type)
: FILbinaryFile(Type),
  MaxSize((FILindex)-1) 
{
   // do nothing
}

FILbinaryFileTruncated::~FILbinaryFileTruncated()
{
   // do nothing
}

void FILbinaryFileTruncated::open(const COLstring& FileName, FILbinaryFile::FILmode Mode)
{
   // Because the current working directory might be changed it's important that we get the absolute
   // file path rather than just storing the current file path.
   FILfilePath FilePath;
   FilePath.setFileName(FileName.c_str());
   COLstring CompleteFilePath;
   FilePath.completeFilePath(CompleteFilePath);
   FILbinaryFile::open(CompleteFilePath, Mode);
}  

COLuint32 FILbinaryFileTruncated::write
(
   const void* pBuffer, 
   COLuint32 SizeOfBuffer
)
{
   FILbinaryFile::write(pBuffer, SizeOfBuffer);

   // TODO ... haven't really checked to see what the performance hit is like doing
   // a check of the size() of the file all the time.
   if (MaxSize != FILindex(-1))
   {
      if (size() > MaxSize)
      {
         // we need to truncate...
         close();
         COLstring DumpFile = name() + ".old";
         if (FILfileExists(DumpFile))
         {
            FILremove(DumpFile);
         }
         FILrenameFile(name().c_str(), DumpFile.c_str());
         open(name(), FILbinaryFile::Rewrite);
      }
   }

   return SizeOfBuffer;
}
