#ifndef __COL_FILE_H__
#define __COL_FILE_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2013 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLfile
//
// Description:
//
// Simple set of File primitives which we are replacing FIL with for
// Tuatara.  General idea here is to not use C++ exceptions and put in
// the smallest amount of abstraction possible.
//
// Also we want to avoid putting in features that in practice we find
// we don't use.  No bothering with COLsink/COLsource etc.
//
// This isn't really finished.
//
// Author: Eliot Muir
// Date:   Mon  4 Nov 2013 09:44:22 EST
//---------------------------------------------------------------------------

class COLfile{
public:
   static bool fileToString(const COLstring& FileName, COLstring* pDestination, int* pErr);
   static COLstring err(int Err);
};

#endif // end of defensive include

