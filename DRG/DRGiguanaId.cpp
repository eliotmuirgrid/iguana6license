//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: DRGiguanaId
//
// Description:
//
// Implementation
//
// Author: Kevin Senn
// Date:   Wednesday, October 22nd, 2008 @ 02:59:29 PM
//
//---------------------------------------------------------------------------
#include "DRGprecomp.h"
#pragma hdrstop

#include "DRGiguanaId.h"
#include "DRGfilterEncrypt.h"

#include <COL/COLsimpleBuffer.h>
#include <FIL/FILutils.h>

#include <RGN/RGNmachineId.h>

#include <SFI/SFIhexEncodeFilter.h>
#include <SFI/SFImd5.h>


const char* DRG_IGUANA_ID_CHARACTER_SET = "23456789ABCDEFGHJKLMNPQRSTUVWXYZ";

// No default directory for posix.  Oh well.
#define DRG_DEFAULT_WORKING_DIRECTORY "C:\\PROGRAM FILES\\INTERFACEWARE\\IGUANA\\"

COLstring DRGiguanaId()
{
   // Eliot note to maintainers of the this code least they make the same mistake I did and forget
   // that by using FILcurrentWorkingDir() one is in effect normalizing the working directory.  I changed
   // the code to pass through the configuration directory as an argument and ran into the difficulty that d:\build\DBD
   // is different from D:\build\DBD

   COLstring WorkingDirectory = FILcurrentWorkingDir();
   return DRGiguanaIdImplementation(RGNmachineId(), WorkingDirectory);
}

COLuint32 DRGparseHexDigit(const char Character)
{
   if (Character >= '0' && Character <= '9')
   {
      return Character - '0';
   }
   else if (Character >= 'A' && Character <= 'F')
   {
      return 10 + (Character - 'A');
   }
   else
   {
      COL_ERROR_STREAM("Non-hex character encountered: '" << Character << '\'', COLerror::PreCondition);
   }
}

COLuint32 DRGparseTwoDigitHexNumber(const char FirstCharacter, const char SecondCharacter)
{
   return DRGparseHexDigit(FirstCharacter)*16 + DRGparseHexDigit(SecondCharacter);
}

// TODO - looks like this is HEX de-encoding code - it could probably be replaced with SFI library code?
COLstring DRGconvertFromHexToIguanaId(const COLstring& HexString)
{
   COLstring IguanaId(HexString.size()/2, ' ');
   COLuint32 CurrentIguanaIdChar;
   const COLuint32 CountOfPossibleIguanaIdCharacters = strlen(DRG_IGUANA_ID_CHARACTER_SET);

   for (COLuint32 CurrentIguanaIdCharIndex = 0; CurrentIguanaIdCharIndex < IguanaId.size(); ++CurrentIguanaIdCharIndex)
   {
      CurrentIguanaIdChar = DRGparseTwoDigitHexNumber
         (HexString[CurrentIguanaIdCharIndex*2], HexString[CurrentIguanaIdCharIndex*2+1])
         % CountOfPossibleIguanaIdCharacters;
      IguanaId[CurrentIguanaIdCharIndex] = DRG_IGUANA_ID_CHARACTER_SET[CurrentIguanaIdChar];
   }

   return IguanaId;
}

COLstring DRGiguanaIdImplementation(const COLstring& MachineId, const COLstring& WorkingDirectoryRaw)
{
   // Eliot note to maintainers of the this code least they make the same mistake I did and forget
   // that by using FILcurrentWorkingDir() one is in effect normalizing the working directory.  I changed
   // the code to pass through the configuration directory as an argument and ran into the difficulty that d:\build\DBD
   // is different from D:\build\DBD
   COLstring WorkingDirectory(WorkingDirectoryRaw);
   FILcorrectPathSeparators(WorkingDirectory);
#ifdef WIN32  // mea coupa - see #10826
   WorkingDirectory.toUpperCase();
   if (WorkingDirectory == DRG_DEFAULT_WORKING_DIRECTORY)
   {
      return MachineId;
   }
#endif

   COLstring UnencryptedIguanaId = MachineId + WorkingDirectory;
   COLstring HexString = SFImd5(UnencryptedIguanaId);
   // TODO - huh - we get the MD5 hash then unencrypt it???
   return DRGconvertFromHexToIguanaId(HexString);
}
