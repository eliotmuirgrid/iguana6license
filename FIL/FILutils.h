#ifndef __FIL_UTILS_H__
#define __FIL_UTILS_H__
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

#include <COL/COLstring.h>
#include <COL/COLauto.h>
#include <COL/COLclosure.h>
#include <FIL/FILbinaryFile.h>
#include <time.h>

// standard file permissions (R=read, W=write, X=execute)

extern const COLuint32 FIL_READABLE;
extern const COLuint32 FIL_WRITABLE;

#ifndef WIN32
extern const COLuint32 FIL_EXECUTABLE;

#   ifndef DEFFILEMODE
#      define DEFFILEMODE S_IRUSR | S_IWUSR |S_IRGRP | S_IROTH
#   endif

#endif

#define FIL_USER_RWX 0x01C0
#define FIL_GROUP_RX 0x0028
#define FIL_OTHER_RX 0x0005

class COLstring;
class COLsink;

typedef COLauto< COLclosure2<const COLstring& /* In */, COLstring* /* pOut */> > FILnameFilter;

// This one throws exceptions.
void FILreadFileIntoString(const COLstring& FileName, COLstring* pDestination);

void FILwriteFile(const COLstring& FileName, const COLstring& Data);
void FILappendToFile(const COLstring& FileName, const COLstring& Data);

// Get the directory of Module
// For Chameleon the ModuleName parameter should be set to CHM_LIB3.dll.
// This is a deliberate break of backward compatibility I have made ... so
// read this message when the build breaks tommorrow :-)
// If ModuleName is empty (or not found), will return the directory of
// the current exe.  However, if this is what you're looking for, you
// should use FILgetExecutableFileDirectory() instead.
COLstring FILgetModuleDirectory(const COLstring& ModuleName);

// append '\' if necessary
void FILaddPathSeparator(COLstring& Path);
// remove '\' if necessary
void FILremovePathSeparator(COLstring& Path);
// remove '\' from start of path if necessary
void FILpathSepLeadingRemoveOneNative(COLstring& Path);

// change all separators in a string so that they conform to
// the platforms separator specifications (ex. convert / to \ on win)
void FILcorrectPathSeparators(COLstring& Path);

// Note that this function used to return a Boolean and now throws an exception when it
// fails.
void FILsetCurrentWorkingDir(const COLstring& Dir);

time_t FILmodified(const COLstring& FileName);

// Note that this function used to return a Boolean and now throws an exception when it
// fails.  Mode is only applicable under Posix - it's ignored under Windows.  It specifies
// the permissions to use when creating the directory.
// TODO - need better Mode argument.
#ifndef WIN32
void FILmakeDir(const COLstring& Dir, const COLint32 Mode, const char* pOwnerUserName = NULL);
#else
void FILmakeDir(const COLstring& Dir, const COLint32 Mode);
#endif

//Copy a SourceDir's contents (including sub directories and their files),
//into another folder. The DestDir doesn't have to exist for this to work.
void FILcopyDirectoryContents(const COLstring& SourceDirPath, const COLstring& DestDirPath, const COLint32 Mode, bool IncludeHidden=true);

// This function uses the FILfilePath object to parse the DirPath argument and will make
// sure that all of the directories in the path exist
// TODO - need to Test this function
#ifndef WIN32
void FILmakeFullDir(const COLstring& DirPath, const COLint32 Mode, const char* pOwnerUserName = NULL);
#else
void FILmakeFullDir(const COLstring& DirPath, const COLint32 Mode);
#endif

// Returns the current working directory - always with one directory separator
// appended to the end.  This is slightly different behaviour from earlier versions
COLstring FILcurrentWorkingDir();

// Returns true or false depending on whether the file is a directory
// WARNING: will throw an exception if the attribute check fails
// e.g. when the path doesn't exist
// - check for FILfileExists() first if this might be the case
bool FILisDirectory(const COLstring& FullPath);

bool FILisDirectoryEmpty(const COLstring& DirectoryName);

// if Overwrite == false and the file exists, an exception is thrown.
// destination directory must exist; if you want to copy to a dirtree that doesn't
// exist, use FILmakeFullDir before you call FILcopyFile.
void FILcopyFile(const COLstring& FromFile, const COLstring& ToFile, bool Overwrite = true);

// I removed FILmoveFile since it didn't follow the semantics of the rest
// of the FILutils library - i.e. throwing exceptions when errors occur - use
// this function instead  (FILrename has been removed too )
void FILrenameFile(const COLstring& FromFile, const COLstring& ToFile);

// This function is only implemented under POSIX
bool FILisSymbolicLink(const COLstring& FileName);

// Set the executable file directory of the current application
// Under POSIX, pass argv[0] to this function before calling FILgetExecutableFileDirectory
// Under WIN32, this function does nothing
void FILsetExecutableFileDirectory(const COLstring& ExecutableFilePath);

// Get the directory of the current application
void FILgetExecutableFileDirectory(COLstring& Directory);  

// Get only the file name of the current application for POSIX and Windows
// WithEx is ignored in POSIX
COLstring FILgetExecutableFileName(bool WithExt = true);

#ifdef WIN32
// Get the system directory
void FILgetSystemDirectory(COLstring& SystemDir);

// Get the windows directory
void FILgetWindowsDirectory(COLstring& SystemDir);

wchar_t* FILmangleFileNameForWindows(const COLstring& FileName, COLstring* pMangled);
void FILunMangleWindowsFileName(const wchar_t* pMangled, COLstring* pFixed);
#endif // !WIN32

// Get the full file name of the current application
void FILfullExecutableFileName(COLstring& Filename);

bool FILfileExists(const COLstring& FileName);
bool FILfileExists(const COLstring& FileName, FILnameFilter pFilter);
bool FILexecutableExists(const COLstring& CommandLine);
bool FILhasPermission(const COLstring& FileName, COLuint32 PermissionMode);

#ifndef CHM_WIN
// POSIX only
void FILsetPermission(const COLstring& FileName, COLuint32 PermissionMode);
#endif

// Warning - don't use this on an Open file - it is not reliable
// in that situation - use the .size() method on FILbinaryFile instead
// in those cases.
COLuint64 FILfileSize(const COLstring& Filename);

void FILgetTempDirectory(COLstring& TemporaryDirectory);

COLstring FILgetTempFileName();


#ifdef WIN32
#  define FIL_DIR_SEPARATOR "\\"
#  define FIL_PATH_SEPARATOR ";"
#else
#  define FIL_DIR_SEPARATOR "/"
#  define FIL_PATH_SEPARATOR ":"
#endif

#define FIL_POSIX_DIR_SEPARATOR "/"
#define FIL_POSIX_PATH_SEPARATOR ":"


// wrapper for the remove() library call
// FIXME WARNING RED FLAG - it has inconsistent behavior between windows and posix
// Windows: This function will fail to remove a directory - use FILremoveDir instead
//    ref http://msdn.microsoft.com/en-us/library/2da4hk1d(v=vs.90).aspx
//    > ENOENT to indicate .. or that the path specifies a directory.
// Posix: remove() deletes a name from the filesystem.  It calls unlink(2) for files, and rmdir(2) for directories.
//    ref man remove
//
// On Windows passing true to the RemoveReadOnly flag will delete files/dirs even if they 
// have the read-only attribute set.
void FILremove(const COLstring& FileName, bool RemoveReadOnly = false);

// This function will throw an exception if it fails.
// DEPRECATED, see FILremoveFullDirNew
void FILremoveDir(const COLstring& DirectoryName, bool RemoveReadOnly = false);

// This function will remove a complete directory.
// DEPRECATED, see FILremoveFullDirNew
void FILremoveFullDir(const COLstring& DirectoryName, bool RemoveReadOnly = false);

// This function will remove a complete directory (including read only/hidden files).
// Done entirely with native system implementations to avoid recursive delete errors (see #26846)
void FILremoveFullDirNew(const COLstring& DirectoryName);

void FILdeleteFileIfExists(const COLstring& FilePath, bool RemoveReadOnly = false);

// This function will try and remove a complete directory.  If it
// runs into problems - i.e. someone has a process using a file located
// in a sub-directory it will continue on without error, but it will log
// the result in the COLostream argument.
void FILcleanFullDir(const COLstring& DirectoryName, COLostream& Stream, bool RemoveReadOnly = false);

// Changes the size of a file.  If the new size is smaller, the file is truncated.
// If the new size is larger, the file is extended and padded with zeros.
void FILchangeFileSize(const COLstring& FileName, COLuint64 NewFileSize);

// Return an expanded path of the form ~/some/dir to $HOME/some/dir
// Does not support paths of the form ~username/some/dir
// Returns the same string if cannot expand.
COLstring FILexpand(const COLstring& Path);

#endif // end of defensive include
