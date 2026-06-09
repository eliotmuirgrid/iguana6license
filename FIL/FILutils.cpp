//---------------------------------------------------------------------------
// Copyright (C) 1997-2013 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILutils
//
// Description:
//
// Basic wrapping of operating system file functions
//
// Author: Eliot Muir
// Date:   Tue 09/01/1998 
//---------------------------------------------------------------------------
#include "FILprecomp.h"
#pragma hdrstop

#include "FILbinaryFile.h"
#include "FILdirEnumerator.h"
#include "FILfilePath.h"
#include "FILosDefs.h"
#include "FILpathUtils.h"
#include "FILutils.h"

#include <COL/COLsimpleBuffer.h>
#include <COL/COLerror.h>
#include <COL/COLsplit.h>
#include <COL/COLutils.h>
#include <COL/COLvector.h>
#include <COL/COLlog.h>
COL_LOG_MODULE;

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#ifdef WIN32
#   include <io.h>
#   include <direct.h>
#   include <winbase.h>
    const COLuint32 FIL_READABLE = 4;
    const COLuint32 FIL_WRITABLE = 2;
#   include <Shobjidl.h>
#   include <ShellAPI.h>
#   include <comdef.h>
#   include <objbase.h>

#else // POSIX
#   include <errno.h>
#   include <limits.h>
#   include <pwd.h>
#   include <unistd.h>
#   include <ftw.h>

#   ifdef CHM_MAC
#      include <mach-o/dyld.h>
#   endif

#   if defined(CHM_HPUX_ITANIUM) || defined(CHM_HPUX)
#      define _PSTAT64
#      include <sys/pstat.h>
#   endif

    const COLuint32 FIL_READABLE = R_OK;
    const COLuint32 FIL_WRITABLE = W_OK;
    const COLuint32 FIL_EXECUTABLE = X_OK;

    // Should be thread safe in this context #10052
    static COLstring s_ExecutableFileDirectory;
    static COLstring s_ExecutableFileName;
#endif // WIN32

// Warning! The destination COLstring can have, at most, a capacity of
// MAX_INT, while the files we could be reading into it could have a size
// of FILindex, which is a COLuint64 (long long or __int64). Coercing this
// to an int might have some surprising results.
void FILreadFileIntoString(const COLstring& FileName, COLstring* pDestination){
   COL_FUNCTION(FILreadFileIntoString);
   FILbinaryFile File(FileName, FILbinaryFile::Read);
   COL_VAR(File.size());
   pDestination->setCapacity(File.size()+1);
   COL_VAR(pDestination->capacity());
   pDestination->setSize(File.size());
   File.read((void*)pDestination->c_str(), File.size());
   File.close();
}

void FILwriteFile(const COLstring& FileName, const COLstring& Data){
   COL_FUNCTION(FILwriteFile);
   FILbinaryFile File(FileName, FILbinaryFile::Rewrite);
   File.write((void*)Data.c_str(), Data.size());
   File.close();
}

void FILappendToFile(const COLstring& FileName, const COLstring& Data){
   COL_FUNCTION(FILappendToFile);
   FILbinaryFile File(FileName, FILbinaryFile::Append);
   File.write((void*)Data.c_str(), Data.size());
   File.close();
}

void FILaddPathSeparator(COLstring& Directory){
   if (Directory.size() == 0){
      Directory += FIL_DIR_SEPARATOR;
      return;
   }
   if (Directory[Directory.size()-1] != FIL_DIR_SEPARATOR[0]){
      Directory += FIL_DIR_SEPARATOR;
   }
}

void FILremovePathSeparator(COLstring& Path){
   if (Path.is_null()){
      return;
   }
   if ( (Path.size() != 1) && (Path[Path.size()-1] == FIL_DIR_SEPARATOR[0]) ){
      Path = Path.substr(0, Path.size()-1);
   }
}

void FILpathSepLeadingRemoveOneNative(COLstring& Path){
   if (Path.is_null()){
      return;
   }
   if ( Path[0] == FIL_DIR_SEPARATOR[0] ){
      Path = Path.substr(1, Path.size());
   }
}

time_t FILmodified(const COLstring& FileName){
   struct FILstat StatBuffer;
   int Result = FILstat(FileName.c_str(), &StatBuffer);
   if (Result != 0){
      COL_THROW_STRERROR_PLAIN("FILmodified on " << FileName << " failed.");
   }
   return StatBuffer.st_mtime;
}

void FILcorrectPathSeparators(COLstring& Path) {
#ifdef WIN32
   const char FromChar = '/';
   const char ToChar = '\\';
#else
   const char FromChar = '\\';
   const char ToChar = '/';
#endif

   char* Pos = (char*)Path.c_str();
   while (char Ch = *Pos)
   {
      if (FromChar == Ch)
      {
         *Pos = ToChar;
      }
      ++Pos;
   }
}

#ifdef WIN32
bool FILfileExists(const COLstring& FileName) {
   COL_FUNCTION(FILfileExists);
   COLstring WindowsName;
   int Result = _waccess(FILmangleFileNameForWindows(FileName, &WindowsName), FILexistsPermissions);
   COL_VAR(Result);
   return Result == 0; 
}
#else
bool FILfileExists(const COLstring& FileName) {
   COL_FUNCTION(FILfileExists);
   int Result = FILaccess(FileName.c_str(), FILexistsPermissions);
   return Result == 0;
}
#endif

bool FILfileExists(const COLstring& FileName, FILnameFilter pFilter) {
   COL_FUNCTION(FILfileExists);
   if (pFilter.get()) {
      COLstring Tmp;
      pFilter->run(FileName, &Tmp);
      return FILfileExists(Tmp);
   }
   return FILfileExists(FileName);
}

bool FILexecutableExists(const COLstring& CommandLine) {
   COL_FUNCTION(FILexecutableExists);
   COLvector<COLstring> Pieces;
   COLsplit(&Pieces, CommandLine, " ");
   COLstring Trying;
   for (COLvector<COLstring>::const_iterator i = Pieces.begin(); i != Pieces.end(); ++i) {
      COL_TRC(i->data());
      Trying.append(i->data());
      if (FILfileExists(Trying)) {
         return true;
      }
      Trying.append(" ");
   }
   return false;
}

bool FILhasPermission(const COLstring& FileName, COLuint32 PermissionMode)
{
   // COLcout << "Exists? "     << F_OK << ' ' << FILaccess(FileName.c_str(), F_OK) << ::newline;
   // COLcout << "Executable? " << X_OK << ' ' << FILaccess(FileName.c_str(), X_OK) << ::newline;
   // COLcout << "Readable? "   << R_OK << ' ' << FILaccess(FileName.c_str(), R_OK) << ::newline;
   // COLcout << "Writable? "   << W_OK << ' ' << FILaccess(FileName.c_str(), W_OK) << ::newline;
   // COLcout << "Given... "    << PermissionMode << ' ' << (FILaccess(FileName.c_str(), PermissionMode) == 0) << ::newline;
   
    if(!FILfileExists(FileName))
    {
       COL_ERROR_STREAM_PLAIN
       (
     "File `" << FileName << "\' does not exist.",
     COLerror::FileError
       );
    }

    return (FILaccess(FileName.c_str(), PermissionMode) == 0);
}

#ifndef CHM_WIN
// PerimissionMode example    TODO : is it possible to change it to match chmod convention? e.g. 744 for rwxr--r--
// ls -l   : rwxr--r--
// Binary  : 111100100
// Decimal : 484
void FILsetPermission(const COLstring& FileName, COLuint32 PermissionMode)
{
   if (chmod(FileName.c_str(), PermissionMode) != 0)
   {
      COL_THROW_STRERROR_PLAIN("Attempt to change permissions on '" << FileName << "' failed.");
   }
}
#endif

COLstring FILcurrentWorkingDir()
{
   char Buffer[_MAX_PATH];
   if (NULL == getcwd(Buffer, _MAX_PATH))
   {
      throw COLerror("Unable to get the current working directory", COLerror::PreCondition);
   }
   COLstring Directory = Buffer;
   FILaddPathSeparator(Directory);
   return Directory;
}

COLstring FILgetModuleDirectory(const COLstring& ModuleName)
{
#ifdef WIN32
   HMODULE Handle;
   Handle = ::GetModuleHandle(ModuleName.c_str());
   // this may return NULL if the ModuleName has not been loaded.

   // The GetModuleFileName(NULL,...) will return the name of the calling process
   char Buffer[_MAX_PATH + 1];
   DWORD CountReturned = ::GetModuleFileName(Handle, Buffer, _MAX_PATH);
   if (0 == CountReturned)
   {
      COL_THROW_WIN32_ERROR("GetModuleFileName failed");
   }
   while ((CountReturned > 0) && Buffer[CountReturned] != '\\')
   {
      CountReturned--;
   }
   Buffer[CountReturned] = 0;
   COLstring Directory = Buffer;
#else  // not windows
   COLstring Directory = ".";
#endif
   FILaddPathSeparator(Directory);
   return Directory;
}


#ifdef WIN32
void FILsetCurrentWorkingDir(const COLstring& Dir) {
   COL_FUNCTION(FILsetCurrentWorkingDir);
   COL_INF("FILsetCurrentWorkingDir: " << Dir);
   COLstring WinDir;
   if (0 != _wchdir(FILmangleFileNameForWindows(Dir, &WinDir))) {
      COL_ERR("FILsetCurrentWorkingDir failed: " << Dir);
      COL_THROW_STRERROR_PLAIN("Attempt to change the working directory to '" << Dir << "' failed.");
   }
}

void FILmakeDir(const COLstring& Dir, const COLint32 Mode) {
   COL_FUNCTION(FILmakeDir);
   COLstring WindowsDir;
   if (0 != _wmkdir(FILmangleFileNameForWindows(Dir, &WindowsDir))) {
      COL_THROW_STRERROR_PLAIN("Attempt to make new directory '" << Dir << "' failed.");
   }
}
#else
void FILsetCurrentWorkingDir(const COLstring& Dir) {
   COL_FUNCTION(FILsetCurrentWorkingDir);
   COL_INF("FILsetCurrentWorkingDir: " << Dir);
   if (0 != FILchdir(Dir.c_str())) {
      COL_ERR("FILsetCurrentWorkingDir failed: " << Dir);
      COL_THROW_STRERROR_PLAIN("Attempt to change the working directory to '" << Dir << "' failed.");
   }
}

void FILmakeDir(const COLstring& Dir, const COLint32 Mode, const char* pOwnerUserName) {
   COL_FUNCTION(FILmakeDir);
   if (0 != mkdir(Dir.c_str(), Mode)) { // Mode definitions are in FILutils.h
      COL_THROW_STRERROR_PLAIN("Attempt to make new directory '" << Dir << "' failed.");
   }

   if (pOwnerUserName) {
      passwd* pUserInfo = ::getpwnam(pOwnerUserName);
      if (!pUserInfo) {
         COL_ERROR_STREAM_PLAIN("Invalid owner username `" << pOwnerUserName
            << "', or other error from ::getpwnam().", COLerror::PreCondition);
      }

      if (::chown(Dir.c_str(), pUserInfo->pw_uid, pUserInfo->pw_gid) == -1) {
         COL_ERROR_STREAM_PLAIN("The chown failed trying to change ownership of path `" << Dir << "' to user `" << pOwnerUserName << "'.", COLerror::SystemError);
      }
   }

}
#endif



#ifndef WIN32
void FILmakeFullDir(const COLstring& DirPath, const COLint32 Mode /* =0 */, const char* pOwnerUserName)
#else
void FILmakeFullDir(const COLstring& DirPath, const COLint32 Mode /* =0 */)
#endif
{
   COL_FUNCTION(FILmakeFullDir);
   FILfilePath FilePath;

   // FilePath.countOfDir() below will get the wrong value if DirPath does
   // not end with a path separator, so we must ensure that it has one.
   COLstring TempDir(DirPath);
   FILaddPathSeparator(TempDir); 
   FilePath.setFileName(TempDir.c_str());
   
   COLuint32 DirIndex = 0;
   COLstring BaseDir = FilePath.volume();
   FILaddPathSeparator(BaseDir); 
   
   while (DirIndex < FilePath.countOfDir()) {
      BaseDir += FilePath.directory(DirIndex);
      if (FILfileExists(BaseDir)) {
         COL_TRC(BaseDir << " exists!");
         if (!FILisDirectory(BaseDir.c_str())) {
            COL_ERROR_STREAM_PLAIN("Error creating directory for '" << DirPath << "', '" << BaseDir << " is a file, not a directory.", COLerror::PreCondition);
         } else {
            FILaddPathSeparator(BaseDir); 
         }
      } else {
         FILaddPathSeparator(BaseDir);     

         try {
            COL_TRC("making " << BaseDir);
#ifndef WIN32 
            FILmakeDir(BaseDir, Mode, pOwnerUserName);
#else
            FILmakeDir(BaseDir, Mode);
#endif
         } catch (...) {
            if (!FILfileExists(BaseDir) || !FILisDirectory(BaseDir.c_str()) ) {
               throw;
            }
         }
      }      

      DirIndex++;
   }
}

void FILcopyDirectoryContents(const COLstring& SourceDirPath, const COLstring& DestDirPath, const COLint32 Mode, bool IncludeHidden){
   COL_FUNCTION(FILcopyDirectoryContents);

   if( ! FILfileExists(DestDirPath) ){
      FILmakeFullDir(DestDirPath, Mode);
   }

   COLvector<COLstring> Patterns;
   #ifdef WIN32
      Patterns.push_back("*.*");
   #else
      Patterns.push_back("*");

      if(IncludeHidden){
         Patterns.push_back(".*");
      }
   #endif
   
   const bool WantSort        = false; // unsorted is faster
   const bool DirectoriesOnly = false;
   COLstring Pattern;

   for(int i = 0; i < Patterns.size(); i++){
      Pattern = Patterns[i];
      FILdirEnumerator FileWalker(SourceDirPath + FIL_DIR_SEPARATOR + Pattern, DirectoriesOnly, WantSort);

      COLstring CurrentFileName;
      COLstring Source;
      COLstring Dest;

      COL_TRC("Iterating over files in directory with file detection pattern: " << Pattern);
      while( FileWalker.getFile(CurrentFileName) ){
         COL_VAR(CurrentFileName);
         Source = SourceDirPath + FIL_DIR_SEPARATOR + CurrentFileName;
         Dest   = DestDirPath   + FIL_DIR_SEPARATOR + CurrentFileName;
         COL_VAR2(Source, Dest);
         if( FileWalker.isDirectory() ){
            COL_TRC("Found a sub directory, recursing into it...");
            FILcopyDirectoryContents(Source, Dest, Mode, IncludeHidden);
         }
         else{
            COL_TRC("Found a file...");
            FILcopyFile(Source, Dest, true);
         }
      }
   }
}

COLuint64 FILfileSize(const COLstring& FileName){
   struct FILstat StatBuffer;
   if (0 != FILstat(FileName.c_str(),&StatBuffer)){
     COL_THROW_STRERROR_PLAIN("Could not get file size for file '" << FileName << "'.");
   }
   return StatBuffer.st_size;
}

void FILremoveDir(const COLstring& Dir, FILdirEnumerator& DirEnumerator, bool RemoveReadOnly) {
   COLstring FileName;
   while(DirEnumerator.getFile(FileName)) {
      if (DirEnumerator.isDirectory()) {
         COL_TRC("Found directory " << FileName);
         FILremoveFullDir(Dir + FileName, RemoveReadOnly);
      } else {
         COL_TRC("Removing file " << FileName);
         FILremove(Dir + FileName, RemoveReadOnly);
      }
   }
}

void FILremoveFullDir(const COLstring& DirectoryName, bool RemoveReadOnly)
{
   COLstring Dir(DirectoryName);
   FILaddPathSeparator(Dir);

   // we put in the scope here so that FILdirEnumerator is destroyed
   // before we delete the directory.
   {
#ifdef WIN32
      COL_TRC("Trying to remove full directory " << Dir + "*.*");
      FILdirEnumerator DirEnumerator(Dir + "*.*");
      FILremoveDir(Dir, DirEnumerator, RemoveReadOnly);
#else // POSIX
      COL_TRC("Trying to remove full directory " << Dir + "*");
      FILdirEnumerator DirEnumerator(Dir + "*");
      FILremoveDir(Dir, DirEnumerator, RemoveReadOnly);
      // Now we have to do the hidden files and directories separately
      FILdirEnumerator DirHiddenEnumerator(Dir + ".*");
      FILremoveDir(Dir, DirHiddenEnumerator, RemoveReadOnly);
#endif
   }
   COL_TRC("Finally, removing directory " << DirectoryName);
   FILremoveDir(DirectoryName, RemoveReadOnly);
}


#ifdef WIN32

static void normalizeWindowsPath(COLstring& Path){
   COL_FUNCTION(normalizePath);
   COL_TRC("Normalizing path " << Path);
   Path.replace('/', '\\');
   COL_TRC("Path normalized to " << Path);
}

//Only works >= Windows Vista
static int removeFullDirNative(const COLstring& DirectoryName){
   COL_FUNCTION(removeFullDirNative);
   COLstring DirectoryNameNormalized = FILpathResolve(DirectoryName, FILcurrentWorkingDir());
   normalizeWindowsPath(DirectoryNameNormalized);
   COL_VAR(DirectoryNameNormalized);
   HRESULT OperationResult = CoInitialize(NULL);
   IFileOperation* FileOperation = NULL;
   IShellItem*     DirectoryObj  = NULL;

   COLstring WinDirName;
   wchar_t* pWinDir = FILmangleFileNameForWindows(DirectoryNameNormalized, &WinDirName);
   if( SUCCEEDED(OperationResult) ){
      OperationResult = SHCreateItemFromParsingName(pWinDir, NULL, IID_PPV_ARGS(&DirectoryObj));
   }
   if( SUCCEEDED(OperationResult) ){
      OperationResult = CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&FileOperation));
   }
   if( SUCCEEDED(OperationResult) ){
       FileOperation->SetOperationFlags(FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI | FOFX_EARLYFAILURE);
       COL_TRC("set file operation flags.");
       COL_TRC("About to mark directory for delete: " + DirectoryNameNormalized);
       OperationResult = FileOperation->DeleteItem(DirectoryObj, NULL);
       if( SUCCEEDED(OperationResult) ){
          COL_TRC("Performing delete operation on marked directory: " + DirectoryNameNormalized);
          OperationResult = FileOperation->PerformOperations();
       }
       DirectoryObj->Release();
       FileOperation->Release();
       DirectoryObj  = NULL;
       FileOperation = NULL;
   }
   if( ! SUCCEEDED(OperationResult) ){
      COL_TRC("Error!");
      _com_error ErrorObj(OperationResult);
      COLstring ErrorMessage( ErrorObj.ErrorMessage() );
      COL_TRC("Error trying to remove " + DirectoryNameNormalized + ". " + ErrorMessage + ".");
   }
   CoUninitialize();
   return OperationResult;
}

#else

static int removeFullDirPosixDirTreeWalkCallback(const char* FilePath, const struct stat* Stats, int TypeFlag, struct FTW* OptionFlags){
   COL_FUNCTION(removeFullDirPosixDirTreeWalkCallback);
   COL_TRC("About to remove file/directory: " << FilePath);
   int RemoveResult = remove(FilePath);
   if(RemoveResult == -1){
      COL_TRC("Error trying to remove " + COLstring(FilePath) + ". " + COLstring(strerror(errno)) + ".");
   }
   return RemoveResult;
}
static int removeFullDirNative(const COLstring& DirectoryName){
   COL_FUNCTION(removeFullDirNative);
   COL_TRC("About to recursively remove directory: " + DirectoryName);
   return nftw(DirectoryName.c_str(), removeFullDirPosixDirTreeWalkCallback, 50, FTW_DEPTH | FTW_PHYS);
}

#endif

static void FILremoveFullDirNewImpl(const COLstring& DirectoryName){
   COL_FUNCTION(FILremoveFullDirNewImpl);
   int RemoveResult;
   COL_TRC("Calling relevant OS implementation code..");
   RemoveResult = removeFullDirNative(DirectoryName);
   if(RemoveResult < 0){
      throw COLerror("Attempt to remove '" + DirectoryName + "' failed. Error " + COLint32ToString(RemoveResult));
   }
   COL_TRC("Successfully removed entire directory: " + DirectoryName);
}

void FILremoveFullDirNew(const COLstring& DirectoryName){
   COL_FUNCTION(FILremoveFullDirNew);
   // We allow 2 retries to overcome failures due to hidden file auto-regeneration,
   // or other unexpected file system errors.
   int MaxRetries = 2;
   int RetryCount = 0;
   while(true){
      try{
         FILremoveFullDirNewImpl(DirectoryName);
         break;
      }
      catch(COLerror& Error){
         if(RetryCount == MaxRetries){
            COL_TRC("Remove failed and we reached our retry limit.");
            throw Error;
         }
         COL_TRC("Remove failed and still under retry limit. Will try to remove dir again...");
         COL_VAR2(RetryCount, MaxRetries);
         RetryCount++;
      }
   }
}

bool FILisDirectoryEmpty(const COLstring& DirectoryName){
   COL_FUNCTION(FILisDirectoryEmpty);
   bool Result = true;

   COLstring Dir(DirectoryName);
   FILaddPathSeparator(Dir);

#ifdef WIN32
   FILdirEnumerator DirEnumerator(Dir + "*.*");
#else // POSIX
   FILdirEnumerator DirEnumerator(Dir + "*");
#endif

   COLstring Dummy;
   while( DirEnumerator.getFile(Dummy) ){
      COL_VAR(Dummy);
      Result = false;
   }
   return Result;
}

void FILcleanDir(const COLstring& Dir, FILdirEnumerator& DirEnumerator, COLostream& Stream, bool RemoveReadOnly)
{
   COLstring FileName;
   while(DirEnumerator.getFile(FileName))
   {
      if (DirEnumerator.isDirectory())
      {
         COL_TRC("Found directory " << FileName);
         FILcleanFullDir(Dir + FileName, Stream, RemoveReadOnly);
      }
      else
      {
         COL_TRC("Removing file " << FileName);
         try
         {
            FILremove(Dir + FileName, RemoveReadOnly);
         }
         catch(const COLerror& Error)
         {
            Stream << "Failed to delete " << FileName << " because of " << Error.description() << newline;
         }         
      }
   }
}

void FILcleanFullDir(const COLstring& DirectoryName, COLostream& Stream, bool RemoveReadOnly)
{
   COL_FUNCTION(FILcleanFullDir);
   COLstring Dir(DirectoryName);
   FILaddPathSeparator(Dir);

   // we put in the scope here so that FILdirEnumerator is destroyed
   // before we delete the directory.
   {
#ifdef WIN32
      FILdirEnumerator DirEnumerator(Dir + "*.*");
      FILcleanDir(Dir, DirEnumerator, Stream, RemoveReadOnly);
#else // POSIX
      FILdirEnumerator DirEnumerator(Dir + "*");
      FILcleanDir(Dir, DirEnumerator, Stream, RemoveReadOnly);
      // Now we have to do the hidden files and directories separately
      FILdirEnumerator DirHiddenEnumerator(Dir + ".*");
      FILcleanDir(Dir, DirHiddenEnumerator, Stream, RemoveReadOnly);
#endif
   }
   COL_TRC("Finally, removing directory " << DirectoryName);
   try
   {
      FILremoveDir(DirectoryName, RemoveReadOnly);
   }
   catch(const COLerror& Error)
   {
      Stream << "Unable to delete " << DirectoryName << " because of " << Error.description() << newline;
   }
}

#ifdef WIN32
bool FILisDirectory(const COLstring& FullPath) {
   COL_FUNCTION(FILisDirectory);
   COLstring Path = FullPath;
   FILcorrectPathSeparators(Path);
   FILremovePathSeparator(Path);
   if(Path.size() == 2 && isalpha(Path[0]) && Path[1]==':') {
      // a drive-root path, e.g. C:\ 
      FILaddPathSeparator(Path);
   }
   COLstring WindowsPath;
   const DWORD Attrib = GetFileAttributesW(FILmangleFileNameForWindows(Path, &WindowsPath));
   if( Attrib == 0xFFFFFFFF ) {
      COL_THROW_WIN32_ERROR_PLAIN("Failed to resolve if '" << FullPath << "' is a directory. ");
   }
   return (bool)((Attrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}
#else
bool FILisDirectory(const COLstring& FullPath) {
   COL_FUNCTION(FILisDirectory);
   COLstring Path = FullPath;
   FILcorrectPathSeparators(Path);
   FILremovePathSeparator(Path);
   struct FILstat statbuf;
   if (FILstat(Path.c_str(), &statbuf) < 0)
   {
      COL_THROW_STRERROR_PLAIN("Failed to resolve if '" << FullPath << "' is a directory.");
   }
   if (!(S_ISDIR(statbuf.st_mode) == 0))
   {
      return true;
   } else
   {
      return false;
   }
}
#endif

void FILcopyFile(const COLstring& FromFile, const COLstring& ToFile, bool Overwrite)
{
   COL_FUNCTION(FILcopyFile);
   COL_VAR2(FromFile, ToFile);

   if (!Overwrite && FILfileExists(ToFile)) {
      COL_ERROR_STREAM_PLAIN("Destination file `" << ToFile << "' exists, but Overwrite is unset.", COLerror::PreCondition);
   }

   FILbinaryFile SourceFile;
   SourceFile.open(FromFile, FILbinaryFile::Read);

   FILbinaryFile DestinationFile;
   DestinationFile.open(ToFile, FILbinaryFile::Rewrite);

   COLstring Buffer;
   FILreadFileIntoString(FromFile, &Buffer);
   FILwriteFile(ToFile, Buffer);
}   


#ifdef WIN32
void FILrenameFile(const COLstring& FromFile, const COLstring& ToFile){
   COL_FUNCTION(FILrenameFile);
   COL_VAR2(FromFile, ToFile);
   COLstring WinFrom, WinTo;
   if ( 0 != _wrename(FILmangleFileNameForWindows(FromFile, &WinFrom), FILmangleFileNameForWindows(ToFile, &WinTo))) {
      COL_THROW_STRERROR_PLAIN("Attempt to rename '"  << FromFile << "' to '" << ToFile << "' failed.");
   }
}
#else
void FILrenameFile(const COLstring& FromFile, const COLstring& ToFile){
   COL_FUNCTION(FILrenameFile);
   COL_VAR2(FromFile, ToFile);
   if (0 != ::rename(FromFile.c_str(), ToFile.c_str())) {
      COL_THROW_STRERROR_PLAIN("Attempt to rename '" << FromFile << "' to '" << ToFile << "' failed.");
   }
}
#endif


void FILsetExecutableFileDirectory(const COLstring& OriginalExecutableFilePath)
{
   COL_FUNCTION(FILsetExecutableFileDirectory);
#ifdef WIN32
   //do nothing
#else
   // POSIX
   COL_VAR(OriginalExecutableFilePath);
   COLstring ExecutableFilePath = OriginalExecutableFilePath;

   char Buffer[2048] = {0}; // do not use any path size macros here
#if defined(CHM_LINUX)
   {
      size_t BufferSize = sizeof(Buffer)-1;
      int Length = readlink("/proc/self/exe", Buffer, BufferSize);
      if (Length > 0 && Length < sizeof(Buffer) && Buffer[0])
      {
         Buffer[Length] = 0;
         ExecutableFilePath = Buffer;
      }
   }
#elif defined(CHM_MAC)
   {
      uint32_t BufferSize = sizeof(Buffer)-1;
      if (_NSGetExecutablePath(Buffer, &BufferSize) != -1
       && BufferSize > 0 && BufferSize < sizeof(Buffer) && Buffer[0])
      {
         Buffer[BufferSize] = 0;
         ExecutableFilePath = Buffer;
      }
   }
#elif defined(CHM_HPUX_ITANIUM) || defined(CHM_HPUX)
   {
      size_t BufferSize = sizeof(Buffer)-1;
      struct pst_status s;
      if (pstat_getproc(&s, sizeof(s), 0, ::getpid()) != -1
       && pstat_getpathname(Buffer, BufferSize, &s.pst_fid_text) != -1
       && Buffer[0])
      {
         ExecutableFilePath = Buffer;
      }
   }
#elif defined(CHM_SOLARIS)
   {
      const char* Path = 0;
      if ((Path = getexecname()) && Path[0])
      {
         ExecutableFilePath = Path;
      }
   }
#endif
   s_ExecutableFileName = FILpathNameWithoutLastExt(OriginalExecutableFilePath);
   COL_INF("ExecutableFilePath = " << ExecutableFilePath);
   FILfilePath FilePath;
   FilePath.setFileName(ExecutableFilePath.c_str());
   FilePath.fullDirectory(s_ExecutableFileDirectory);
   COL_INF("s_ExecutableFileDirectory = " << s_ExecutableFileDirectory);
#endif
}

void FILgetExecutableFileDirectory(COLstring& Directory){
#ifdef WIN32
   HMODULE Handle = ::GetModuleHandle(NULL);
   char Buffer[_MAX_PATH + 1];
   DWORD CountReturned = ::GetModuleFileName(Handle, Buffer, _MAX_PATH);
   if (0 == CountReturned){
      COL_THROW_WIN32_ERROR("GetModuleFileName failed.");
   }
   while ((CountReturned > 0) && Buffer[CountReturned] != '\\'){
      CountReturned--;
   }
   Buffer[CountReturned] = 0;
   Directory = Buffer;
#else // POSIX
   if(s_ExecutableFileDirectory.is_null()) { // This is a precondition really.
      COL_ERROR_STREAM("Error: Must call FILsetExecutableFileDirectory under POSIX first", COLerror::PreCondition);
   } else {
      Directory = s_ExecutableFileDirectory;
   }
#endif
}

COLstring FILgetExecutableFileName(bool WithExt) {
#ifdef WIN32
   COLstring FileName;
   FILfullExecutableFileName(FileName);
   if (WithExt) {
      return FILpathName(FileName);
   }
   return FILpathNameWithoutLastExt(FileName);
#else // POSIX
   if (s_ExecutableFileName.is_null()) { // This is a precondition really.
      COL_ERROR_STREAM("Error: Must call FILgetExecutableFileDirectory under POSIX first", COLerror::PreCondition);
   } else {
      return s_ExecutableFileName;
   }
#endif
}

void FILfullExecutableFileName(COLstring& FileName){
#ifdef WIN32
   FileName.assign(_MAX_FNAME, '\0');
   HMODULE FileHandle = ::GetModuleHandle(NULL);
   if (NULL == FileHandle){  // This really is a precondition - don't think we'll see this in production
      COL_THROW_WIN32_ERROR("GetModuleHandle failed.");
   }
   DWORD CountOfChar = ::GetModuleFileName(FileHandle, FileName.get_buffer(), FileName.size());
   if (0 == CountOfChar){ // pre condition.
      COL_THROW_WIN32_ERROR("GetModuleFileName failed.");
   }
   FileName = FileName.substr(0, CountOfChar);
#else
   COLstring ExeDir;
   FILgetExecutableFileDirectory(ExeDir);
   FileName = ExeDir + FILgetExecutableFileName();
#endif
}


#ifdef WIN32
// Get the system directory
void FILgetSystemDirectory(COLstring& SystemDir)
{
   SystemDir.assign(MAX_PATH, '\0');
   UINT Result = GetSystemDirectory(SystemDir.get_buffer(), SystemDir.size());
   if (Result > SystemDir.size())
   {
      SystemDir.assign(Result, '\0');
      Result = GetSystemDirectory(SystemDir.get_buffer(), SystemDir.size());
   }
   if (0 == Result) // precondition
   {
      COL_THROW_WIN32_ERROR("GetSystemDirectory failed.");
   }
   SystemDir = SystemDir.substr(0, Result);
   FILaddPathSeparator(SystemDir);
}

// Get the windows directory
void FILgetWindowsDirectory(COLstring& WindowDir)
{
   WindowDir = "";
   UINT Result = GetWindowsDirectory(WindowDir.get_buffer(), WindowDir.size());
   if (Result > WindowDir.size())
   {
      WindowDir.assign(Result, '\0');
      Result = GetWindowsDirectory(WindowDir.get_buffer(), WindowDir.size());
   }
   if (0 == Result)
   {
      COL_THROW_WIN32_ERROR("GetWindowsDirectory failed.");
   }
   WindowDir = WindowDir.substr(0, Result);
   FILaddPathSeparator(WindowDir);
}
#endif // WIN32

void FILgetTempDirectory(COLstring& TemporaryDirectory)
{
   TemporaryDirectory="";
#ifdef WIN32
   TemporaryDirectory.assign(MAX_PATH, '\0');
   DWORD CountOfChar = ::GetTempPath(TemporaryDirectory.size(), TemporaryDirectory.get_buffer());
   if (0 == CountOfChar)
   {
      COL_THROW_WIN32_ERROR("GetTempPath failed.");
   }
   TemporaryDirectory = TemporaryDirectory.substr(0, CountOfChar);
#else // not WIN32      
   if (NULL != getenv("TMPDIR"))
   {
      TemporaryDirectory = getenv("TMPDIR");
   }
   if (NULL != getenv("HOME"))
   {
      TemporaryDirectory = getenv("HOME");
      FILaddPathSeparator(TemporaryDirectory);
      TemporaryDirectory += COL_T("tmp");      
   }      
#endif 
   FILaddPathSeparator(TemporaryDirectory); 
   if (!FILfileExists(TemporaryDirectory))
   {
      FILmakeFullDir(TemporaryDirectory, FIL_USER_RWX);
   }
}

COLstring FILgetTempFileName()
{
   char FileName[L_tmpnam+1];
   char* pFileName;
   pFileName = tmpnam(FileName);
   if (pFileName != NULL)
   {
      return FileName;
   }
   else
   {
      COL_THROW_STRERROR_PLAIN("Unable to create a temporary file name.");
   }
}

#ifdef WIN32
wchar_t* FILmangleFileNameForWindows(const COLstring& FileName, COLstring* pMangled) {
   COL_FUNCTION(FILmangleFileNameForWindows);
   COLstring& Mangled = *pMangled;
   int WCharCount = ::MultiByteToWideChar(CP_UTF8, 0, FileName.c_str(), -1, NULL, 0);
   int Size = sizeof(wchar_t) * WCharCount;
   Mangled.assign(Size, '\0');
   wchar_t* pMangledWide = (wchar_t*)Mangled.get_buffer();
   ::MultiByteToWideChar(CP_UTF8, 0, FileName.c_str(), FileName.size(), pMangledWide, WCharCount);
   COLvar Check = Mangled;
   COL_VAR(FileName);
   COL_VAR(Check.json(true)); // For looking at the bytes
   return pMangledWide;
}

void FILunMangleWindowsFileName(const wchar_t* pMangled, COLstring* pFixed) {
   COL_FUNCTION(FILunMangleWindowsFileName);
   COLstring& Fixed = *pFixed;
   int ByteCount = ::WideCharToMultiByte(CP_UTF8, 0, pMangled, -1, NULL, 0, NULL, NULL);
   COL_VAR(ByteCount);
   Fixed.assign(ByteCount, '\0');
   ::WideCharToMultiByte(CP_UTF8, 0, pMangled, -1, Fixed.get_buffer(), ByteCount, NULL, NULL);
   Fixed.setSize(ByteCount - 1); // Makes the null byte included in the string a terminator instead.
   COLvar Check = Fixed;
   COL_VAR(Check.json(true)); // Byte view
   COL_VAR(Check.json(true, COLvar::StrEncJs)); // Friendly view
}

void FILremove(const COLstring& FileName, bool RemoveReadOnly) {
   COL_FUNCTION(FILremove);
   COLstring WindowsName;
   wchar_t* pReadyToUse = FILmangleFileNameForWindows(FileName, &WindowsName);
   if(RemoveReadOnly && (GetFileAttributesW(pReadyToUse) & FILE_ATTRIBUTE_READONLY)){
      COL_TRC("Removing the READONLY attribute so file can be deleted.");
      SetFileAttributesW(pReadyToUse, FILE_ATTRIBUTE_NORMAL);
   }
   if(GetFileAttributesW(pReadyToUse) & FILE_ATTRIBUTE_HIDDEN){
      COL_TRC("Removing the HIDDEN attribute so file can be deleted.");
      SetFileAttributesW(pReadyToUse, FILE_ATTRIBUTE_NORMAL);
   }
   if (-1 == _wremove(pReadyToUse)) {
      COL_THROW_STRERROR_PLAIN("Attempt to remove '" << FileName << "' failed.");
   }
}

void FILremoveDir(const COLstring& DirectoryName, bool RemoveReadOnly) {
   COL_FUNCTION(FILremoveDir);
   COLstring WindowsDirName;
   wchar_t* pWindowsDir = FILmangleFileNameForWindows(DirectoryName, &WindowsDirName);
   if (RemoveReadOnly && (GetFileAttributesW(pWindowsDir) & FILE_ATTRIBUTE_READONLY)){
      COL_TRC("Removing the READONLY attribute so dir can be deleted.");
      SetFileAttributesW(pWindowsDir, FILE_ATTRIBUTE_NORMAL);
   }
   if (GetFileAttributesW(pWindowsDir) & FILE_ATTRIBUTE_HIDDEN){
      COL_TRC("Removing the HIDDEN attribute so dir can be deleted.");
      SetFileAttributesW(pWindowsDir, FILE_ATTRIBUTE_NORMAL);
   }
   if (-1 == _wrmdir(pWindowsDir)) {
      COL_THROW_STRERROR_PLAIN("Attempt to remove directory '" << DirectoryName << "' failed.");
   }
}
#else
// POSIX implementation
void FILremove(const COLstring& FileName, bool RemoveReadOnly) {
   COL_FUNCTION(FILremove);
   if (-1 == remove(FileName.c_str())) {
      COL_THROW_STRERROR_PLAIN("Attempt to remove '" << FileName << "' failed.");
   }
}

void FILremoveDir(const COLstring& DirectoryName, bool RemoveReadOnly) {
   COL_FUNCTION(FILremoveDir);
   if (-1 == FILrmdir(DirectoryName.c_str())) {
      COL_THROW_STRERROR_PLAIN("Attempt to remove directory '" << DirectoryName << "' failed.");
   }
}
#endif


void FILdeleteFileIfExists(const COLstring& FilePath, bool RemoveReadOnly) {
   COL_FUNCTION(FILdeleteFileIfExists);
   if (!FilePath.is_null() && FILfileExists(FilePath)){
      FILremove(FilePath, RemoveReadOnly);
   }
}

bool FILisSymbolicLink(const COLstring& FileName)
{
#ifdef WIN32
   return false;
#else
   struct stat64 FileStat;
   if (-1 == lstat64(FileName.c_str(), &FileStat))
   {
      COL_THROW_STRERROR_PLAIN("Failed to resolve if  " << FileName << " is a symbolic link.");
   }

   return ((FileStat.st_mode & S_IFLNK) == S_IFLNK);
#endif
}

void FILchangeFileSize(const COLstring& FileName, COLuint64 NewFileSize) {
#ifdef WIN32
   FILbinaryFile TruncateFile(FileName, FILbinaryFile::Write);
   TruncateFile.setPosition64(NewFileSize);

   if (0 == SetEndOfFile(TruncateFile.win32Handle()))
   {
      COL_THROW_WIN32_ERROR("Attempt to truncate " << FileName << " failed.");
   }
#else
   if (-1 == truncate(FileName.c_str(), NewFileSize))
   {
      COL_THROW_STRERROR_PLAIN("Attempt to truncate " << FileName << " failed.");
   }
#endif
}

COLstring FILexpand(const COLstring& OriginalPath)
{
   COLstring Path = COLexpandEnvironmentVariables(OriginalPath);
   if (Path == "~" || (Path.size() >= 2 && Path[0] == '~' && (Path[1] == '/' || Path[1] == '\\')))
   {
#ifdef CHM_WIN
      const char* Home = ::getenv("UserProfile");
#else
      const char* Home = ::getenv("HOME");
#endif
      if (Home && Home[0])
      {
         return Home + Path.substr(1);
      }
   }
   return Path;
}
