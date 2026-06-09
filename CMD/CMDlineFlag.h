#ifndef __CMD_LINE_FLAG_H__
#define __CMD_LINE_FLAG_H__
//------------------------------------------------------------------------
// Copyright (C) 1997-2012 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  CMDlineFlag
// 
// Description
//
// Represents the command line flag itself
//
// Author: Eliot Muir
// Date:   Thu 10/03/2002 
//
//---------------------------------------------------------------------------

#include <COL/COLvector.h>

class CMDlineFlag : public COLrefCounted
{
public:
   CMDlineFlag(const COLstring& iName)
    : Name(iName),
      IsHidden(false),
      IsPresent(false),
      WasExpected(false),
      HasArgument(false),
      ArgumentName(""),
      WasRepeated(false),
      RepeatOk(false)
   {
   }

   COLstring   Name;
   COLstring   Description;
   bool  HasArgument;
   bool  IsPresent;
   bool  WasExpected;
   COLstring   Argument;
   COLstring   ArgumentName;
   bool  IsHidden;
   bool  WasRepeated;
   COLvector<COLstring> ValueList;
   bool  RepeatOk;

private:
   CMDlineFlag(const CMDlineParser& Orig);
   CMDlineFlag operator=(const CMDlineParser& Orig);
};

#endif // end of defensive include
