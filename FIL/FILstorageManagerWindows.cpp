//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILstorageManager
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Fri 01/16/2004 
//
//
//---------------------------------------------------------------------------
#include "FILprecomp.h"
#pragma hdrstop

#include "FILstorageManager.h"

#include <COL/COLsimpleBuffer.h>
#include <COL/COLerror.h>
#include <COL/COLvector.h>

class FILstorageManagerPrivate
{
public:
   COLvector<COLstring> DeviceVector; 
};

FILstorageManager::FILstorageManager()
{
   pMember = new FILstorageManagerPrivate;
   try
   {
      setup();
   }
   catch(...)
   {
      delete pMember;
      throw;
   }
}

void FILstorageManager::setup()
{
   COLsimpleBuffer Buffer(0);
   COLuint32 RequiredLength = GetLogicalDriveStrings(Buffer.size(), (char*)Buffer.data());
   if (RequiredLength == 0)
   {
      COL_THROW_WIN32_ERROR("GetLogicalDriveStrings failed.");
   }
   Buffer.resize(RequiredLength);
   RequiredLength = GetLogicalDriveStrings(Buffer.size(), (char*)Buffer.data());
   if (RequiredLength == 0  || RequiredLength > Buffer.size())
   {
      COL_THROW_WIN32_ERROR("GetLogicalDriveStrings failed.");
   }
   const char* CharPtr = (const char*)Buffer.data();
   while (*CharPtr != '\0') 
   {
      pMember->DeviceVector.push_back(CharPtr);
      while (*CharPtr != '\0')
      {
         CharPtr++;
      }
      CharPtr++;
   } 
}
 
FILstorageManager::~FILstorageManager()
{
   delete pMember;
}

COLuint32 FILstorageManager::countOfDevice() const
{
   return pMember->DeviceVector.size();   
}

const COLstring& FILstorageManager::deviceName(COLuint32 DeviceIndex) const
{
   return pMember->DeviceVector[DeviceIndex];  
}
