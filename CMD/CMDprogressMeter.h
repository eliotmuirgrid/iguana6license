#ifndef _CMD_PROGRESS_METER_H__
#define _CMD_PROGRESS_METER_H__
//------------------------------------------------------------------------
//
// Copyright (C) 1997-2005 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  CMDprogressMeter
// 
// Description
//
// This is utility class which makes it easy to out
// put a percentage progress meter for a command line process.
// It makes use of the fact that outputing a \r returns to the
// start of the line but does not go down a line.
//
// Author: Eliot Muir
// Date:   Fri 04/25/2003 
//
//---------------------------------------------------------------------------
class COLstring;

class CMDprogressMeter
{
public:
   CMDprogressMeter() {}
   virtual ~CMDprogressMeter() {}

   // set progress - should be a number from 0 to 100
   void setProgress(COLuint32 Percentage, const COLstring& Description, COLostream& Stream);
private:
   // disallowed operations
   CMDprogressMeter(const CMDprogressMeter& Orig);
   CMDprogressMeter& operator=(const CMDprogressMeter& Orig);
};

#endif // end of defensive include
