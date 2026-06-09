//------------------------------------------------------------------------
//
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  CMDprogressMeter
// 
// Description
//
// Implementation of a utility class which makes it easy to output
// a progress meter for a command line application.
//
// Author: Eliot Muir
// Date:   Fri 04/25/2003 
//
//---------------------------------------------------------------------------
#include "CMDprecomp.h"
#pragma hdrstop

#include <CMD/CMDprogressMeter.h>
#include <COL/COLostream.h>


void CMDprogressMeter::setProgress
(
   COLuint32   Percentage, 
   const COLstring&  Description,   
   COLostream& Stream
)
{
   Stream << '\r' << Percentage << Description << "    ";
}
