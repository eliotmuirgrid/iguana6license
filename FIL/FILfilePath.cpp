   //---------------------------------------------------------------------------
//
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILfilePath
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Mon 01/26/2004 
//
//
//---------------------------------------------------------------------------
#include "FILprecomp.h"
#pragma hdrstop

#include "FILfilePath.h"
#include "FILutils.h"

#include <COL/COLsimpleBuffer.h>
#include <COL/COLerror.h>
#include <COL/COLvector.h>
#include <COL/COLmath.h>

#include <stdio.h>

static inline void AdvanceCharacterPointer(COLuint8*& pPointer)
{
   ++pPointer;
}

class FILfilePathPrivate
{
public:
   FILfilePathPrivate() 
    : FileNameBuffer(0),
      pCurrentDirExtractor(NULL)
   {}

   ~FILfilePathPrivate()
   {
      delete pCurrentDirExtractor;
   }

   COLsimpleBuffer FileNameBuffer;

   void scanFileName(const char* pFileName);
   void findVolume();
   void findDirs();

   void checkForBadStuff();

   void constructDirectory();

   void findFileAndExtension();

   void setCurrentDirectory(const char* pCurrentDir);

   COLstring Directory;

   COLstring Volume;

   COLstring Extension;

   COLvector<const char*> DirVector;

   COLuint8* pCursor;

   const char* pVolume;
   const char* pFileName;
   COLuint8* pExtension;

   FILfilePath* pCurrentDirExtractor;
};

void FILfilePathPrivate::setCurrentDirectory(const char* pCurrentDir)
{
   if (pCurrentDirExtractor == NULL)
   {
      pCurrentDirExtractor = new FILfilePath;
   }
   COLstring CurrentWorkingDir; 
   if (pCurrentDir == NULL)
   {
      CurrentWorkingDir = FILcurrentWorkingDir();
   }
   else
   {
      CurrentWorkingDir = pCurrentDir;
      FILaddPathSeparator(CurrentWorkingDir);
      FILcorrectPathSeparators(CurrentWorkingDir);
   }
   pCurrentDirExtractor->setFileName(CurrentWorkingDir.c_str());
}

void FILfilePath::setCurrentDirectory(const char* pCurrentDir)
{
   pMember->setCurrentDirectory(pCurrentDir);
}

void FILfilePathPrivate::checkForBadStuff()
{
#ifdef WIN32
   // TODO - there is other 'bad stuff' we could check for
   pCursor = FileNameBuffer.data();
   if (pCursor[0] == ':')
   {
      throw COLerror("Unable to resolve", COLerror::PreCondition);
   }
   if (FileNameBuffer.size() > 2)
   {
      AdvanceCharacterPointer(pCursor);
      AdvanceCharacterPointer(pCursor);
      while (pCursor < FileNameBuffer.end())
      {
         if (*pCursor == ':')
         {
            throw COLerror("Bad file path", COLerror::PreCondition);
         }
         AdvanceCharacterPointer(pCursor);
      }
   }
#endif
   pCursor = FileNameBuffer.data();
}

#ifdef WIN32
void FILfilePathPrivate::findVolume()
{
   if (FileNameBuffer.size() >= 2 
         && COLmath::toUpper(FileNameBuffer[0]) >= 'A' 
         && COLmath::toUpper(FileNameBuffer[0]) <= 'Z' 
         && FileNameBuffer[1] == ':')
   {
      pVolume = (const char*)FileNameBuffer.data();
      if (FileNameBuffer[2] == '\0')
      {
         return; // no need to null terminate - we're done
      }
      else
      {
         FileNameBuffer[2] = '\0'; // Null terminate         
         pCursor = FileNameBuffer.data() + 3;  // start of next part
         return;
      }
   }

   if (FileNameBuffer.size() >= 5 && FileNameBuffer[0] == '\\' && FileNameBuffer[1] == '\\')
   {
      pCursor = FileNameBuffer.data() + 2;
      while (pCursor < FileNameBuffer.end() && *pCursor != '\\')
      {
         AdvanceCharacterPointer(pCursor);
      }
      if (pCursor < FileNameBuffer.end())
      {
         AdvanceCharacterPointer(pCursor);
      }
      while (pCursor < FileNameBuffer.end() 
             && *pCursor != '\\' 
             && *pCursor != '/')
      {
         AdvanceCharacterPointer(pCursor);
      }
      if (pCursor < FileNameBuffer.end())
      {
         // we have a volume
         *pCursor = '\0'; 
         pVolume = (const char*)FileNameBuffer.data();
         pCursor++;
         return;
      }  
   }
   // we have no volume
   // so we must get the current directory
   if (pCurrentDirExtractor == NULL)
   {
      setCurrentDirectory(NULL);
   }
   COLPOSTCONDITION(pCurrentDirExtractor != NULL);
   Volume = pCurrentDirExtractor->volume();

   pVolume = Volume.c_str();
   pCursor = FileNameBuffer.data();
   if (*pCursor != FIL_DIR_SEPARATOR[0])
   {
      // we have a relative file name, to resolve it we will need to get
      // the current working directory and prepend it to this file path
      COLstring FullPath = pCurrentDirExtractor->directory();
      FullPath += (const char*)FileNameBuffer.data();
      FileNameBuffer = FullPath.c_str();     
      pCursor = FileNameBuffer.data();
   }
}
#else // if not WIN32
void FILfilePathPrivate::findVolume()
{
   // we have no volume under UNIX
   pVolume = "";
   if (*pCursor != FIL_DIR_SEPARATOR[0])
   {
      if (pCurrentDirExtractor == NULL)
      {
         setCurrentDirectory(NULL);
      }
      COLPOSTCONDITION(pCurrentDirExtractor != NULL);
      // We have a relative file name
      COLstring FullPath  = pCurrentDirExtractor->directory();
      FullPath += (const char*)FileNameBuffer.data();
      FileNameBuffer = FullPath.c_str();
      pCursor = FileNameBuffer.data();
   }
}
#endif

void FILfilePathPrivate::findDirs()
{
   DirVector.clear();
   if (*pCursor != FIL_DIR_SEPARATOR[0])
   {
      DirVector.push_back((char*)pCursor);
   }
   while (pCursor < FileNameBuffer.end())
   {
      if (*pCursor == FIL_DIR_SEPARATOR[0])
      {
         *pCursor = '\0';
         AdvanceCharacterPointer(pCursor);
         DirVector.push_back((char*)pCursor);
      }
      else
      {
         AdvanceCharacterPointer(pCursor);
      }     
   }
   pCursor = (COLuint8*)DirVector.back();
   DirVector.pop_back(); 
   for (COLuint8 DirIndex = 0; DirIndex < DirVector.size(); DirIndex++)
   {
      if (DirVector[DirIndex][0] == '.')
      {
         if (DirVector[DirIndex][1] == '.')
         {
            // we have a '..' directory
            if ((DirIndex < 1) || (DirVector.size() < 2))
            {
               throw COLerror("Unable to resolve", COLerror::PreCondition);
            }
            DirVector.remove(DirIndex);
            DirVector.remove(DirIndex-1);
            DirIndex -= 3;
         }
         else if(DirVector[DirIndex][1]==0)
         {
            // we have a '.' directory
            DirVector.remove(DirIndex);
            DirIndex -= 2;
         }
         // else it's a directory that begins with a dot
      }
      else if (DirVector[DirIndex][0] == '\0')
      {
         // we have a double directory separator so we treat it the same
         DirVector.remove(DirIndex);
         DirIndex -= 2;
      }
   }
}

void FILfilePathPrivate::constructDirectory()
{
   Directory = COLstring(FIL_DIR_SEPARATOR);
   for (COLuint32 DirIndex = 0; DirIndex < DirVector.size(); DirIndex++)
   {
      Directory +=  DirVector[DirIndex];
      Directory += COLstring(FIL_DIR_SEPARATOR);
   }
}

void FILfilePathPrivate::findFileAndExtension()
{
   pFileName = (const char*)pCursor;
   // now we have to figure out the two cases of
   // zero length file - i.e. no file name
   AdvanceCharacterPointer(pCursor);
   if (pCursor == FileNameBuffer.end())
   {
      pExtension = (COLuint8*)"";
      return;  // we have no file.
   }
   // else let's see if we have an extension
   pExtension = NULL;
   while (pCursor < FileNameBuffer.end())
   {
      if (*pCursor == '.')
      {
         pExtension = pCursor;
      }
      AdvanceCharacterPointer(pCursor);
   }
   if (pExtension != NULL)
   {
      *(pExtension) = '\0'; // null terminate the file name
      AdvanceCharacterPointer(pExtension);
   }
   else
   {
      pExtension = (COLuint8*)"";  // we have no extension
   }
}


void FILfilePathPrivate::scanFileName
(
   const char* ipFileName
)
{
   try
   {
      COLstring FileName = ipFileName;
      FILcorrectPathSeparators(FileName);
      FileNameBuffer = FileName.c_str();

      checkForBadStuff();
      findVolume();
      findDirs();
      Directory.clear();
      findFileAndExtension();
   }
   catch (const COLerror& Error)
   {
      COL_ERROR_STREAM_PLAIN(Error.description() << " '" << ipFileName << '\'', COLerror::PreCondition);
   }
}

////////////////////////////////////////////////////////////////////////////////

FILfilePath::FILfilePath()
{
   pMember = new FILfilePathPrivate();
}

FILfilePath::~FILfilePath()
{
   delete pMember;
}

void FILfilePath::setFileName
(
   const char* FileName
)
{
   pMember->scanFileName(FileName);
}

const char* FILfilePath::extension() const
{
   COLPRECONDITION(pMember->FileNameBuffer.size() > 0);
   return (const char*)pMember->pExtension;
}

const char* FILfilePath::volume() const
{
   COLPRECONDITION(pMember->FileNameBuffer.size() > 0);  // i.e. have we actually assigned a file name to work on.
   return pMember->pVolume;
}

COLuint32 FILfilePath::countOfDir() const
{
   return pMember->DirVector.size();
}

const char* FILfilePath::directory(COLuint32 DirectoryIndex) const
{
   return pMember->DirVector[DirectoryIndex];
}



const char* FILfilePath::nameWithoutLastExt() const
{
   return pMember->pFileName;
}

const char* FILfilePath::directory() const
{
   if (pMember->Directory.is_null())
   {
      pMember->constructDirectory();
   }
   return pMember->Directory.c_str();
}

void FILfilePath::fullDirectory(COLstring& FullDirectory) const
{
   FullDirectory = COLstring(volume()) + directory();
}
COLstring FILfilePath::fullDirectory() const
{
   COLstring Result;
   fullDirectory(Result);
   return Result;
}

void FILfilePath::fullFilename(COLstring& FileName) const
{
   FileName += nameWithoutLastExt();
   if (strlen(extension()) > 0)
   {
      FileName += '.';
      FileName += extension();
   }
}
COLstring FILfilePath::fullFilename() const
{
   COLstring Result;
   fullFilename(Result);
   return Result;
}

void FILfilePath::completeFilePath(COLstring& FileName) const
{
   FileName = volume();
   FileName += directory();
   fullFilename(FileName);
}
COLstring FILfilePath::completeFilePath() const
{
   COLstring Result;
   completeFilePath(Result);
   return Result;
}
