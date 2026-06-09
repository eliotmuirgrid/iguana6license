#ifndef __FIL_FILE_PATH_H__
#define __FIL_FILE_PATH_H__
//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILfilePath
//
// Description:
// 
// NOTE: THIS CLASS HAS BEEN DEPRECATED. PLEASE USE FILPATHUTILS.H INSTEAD.
//
// This object allows the manipulation of a file name to
// extract things like the file extension, volume (drive or network drive)
// and convert to an absolute file name or relative file name.
//
// The object is intended to be efficiently implemented and work
// with multibyte file paths.  It does a copy of a file name being
// processed into an internal buffer which it then scans through
// assigning pointers to the various parts of the file name.
//
// KNOWN LIMITATIONS
//
// This class hasn't been throughly tested with incorrectly formatted
// file paths.  It also has not been profiled for performance.  It has not
// been verified whether the logic with multi-byte file paths actually
// works.  Generally speaking though, these are things which should be
// easy enough to do later and shouldn't affect the majority of our
// applications.
//
// Author: Eliot Muir
// Date:   Mon 01/26/2004 
//
//---------------------------------------------------------------------------


#include <COL/COLminimumInclude.h>

class FILfilePathPrivate;

class COL_DLL FILfilePath
{
public:
   FILfilePath();
   ~FILfilePath();  // assuming there are no children of this class

   // If this call is not made then FILfilePath will fetch the
   // current directory from the operating system if it needs it.  It
   // only needs it when a call is made to setFileName with a *relative*
   // file path - i.e. one that needs a current directory to resolve it.
   // Internally the FILfilePath will create another FILfilePath
   // object on demand to handle the current directory (it has to be done
   // this way to avoid a recursive loop - since the FILfilePath created
   // for the current directory must not create it's own child FILfilePath
   // objects (!)
   // If you set the argument to NULL then the current working directory will be
   // taken from the operating system.
   void setCurrentDirectory(const char* CurrentDir=NULL);  

   // This sets the file name that is to be parsed.  This method
   // may throw an exception if the file cannot be parsed.  If the path name
   // given is a relative path then this class will resolve it to an absolute
   // path based on the current directory (see setCurrentDirectory).
   void setFileName(const char* FileName);

   // Volume can be 'D:' or '\\Chameleon\build\'
   // obviously this doesn't apply to POSIX operating systems.
   const char* volume() const;

   // Count of nested directories that this file is in.
   COLuint32 countOfDir() const;

   // Array of nested directories that this file is in - see countOfDir
   const char* directory(COLuint32 DirectoryIndex) const;

   // This returns the filename without the extension
   // so 'life.txt' returns 'life'
   const char* nameWithoutLastExt() const;

   // This appends the filename with the extension to the string passed
   void fullFilename(COLstring& FileName) const;
   COLstring fullFilename() const; // functional equivalent

   // This returns the complete resolved file path
   void completeFilePath(COLstring& FileName) const;
   COLstring completeFilePath() const;


   // This returns the directory without the filename, extension
   // or volume
   const char* directory() const;

   // This returns the directory with the volume if it's windows
   void fullDirectory(COLstring& FullDirectory) const;
   COLstring fullDirectory() const;

   // The extension does not include the delimiter character - i.e. 
   // 'txt' instead of '.txt'
   const char* extension() const;

private:
   FILfilePathPrivate* pMember;

   FILfilePath(const FILfilePath& Orig); // not allowed
   FILfilePath& operator=(const FILfilePath& Orig); // not allowed
};

#endif // end of defensive include
