#ifndef __CMD_EXTRA_PARAM_DESCRIPTION_H__
#define __CMD_EXTRA_PARAM_DESCRIPTION_H__
//------------------------------------------------------------------------
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  CMDextraParamDescription
// 
// Description
//
// Represents an extra parameter
//
// Author: Roman Leifer
// Date:   19/08/2004 
//
//---------------------------------------------------------------------------

class CMDextraParamDescription
{
public:
   CMDextraParamDescription() 
    : ParamRequired(false){}
   CMDextraParamDescription& operator=(const CMDextraParamDescription& Orig)
   {
      ParamName = Orig.ParamName;
      ParamDescription = Orig.ParamDescription;
      ParamRequired = Orig.ParamRequired;
      return *this;
   }
   CMDextraParamDescription(const CMDextraParamDescription& Orig)
   {
      operator=(Orig);
   }
   CMDextraParamDescription(const COLstring& Name, 
                            const COLstring& Description,
                            bool       IsRequired) :
      ParamName(Name), 
      ParamDescription(Description),
      ParamRequired(IsRequired) {}
 
   COLstring ParamName;
   COLstring ParamDescription;
   bool ParamRequired;
};

#endif // end of defensive include
