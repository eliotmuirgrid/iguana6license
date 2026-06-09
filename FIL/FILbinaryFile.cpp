//---------------------------------------------------------------------------
// Copyright (C) 1997-2013 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILbinaryFile
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Fri 01/16/2004 
//---------------------------------------------------------------------------
#include "FILprecomp.h"
#pragma hdrstop

#include "FILbinaryFile.h"
#include "FILfilePath.h"
#include "FILutils.h"

#include <COL/COLerror.h>
#include <COL/COLsimpleBuffer.h>
#include <COL/COLlog.h>
COL_LOG_MODULE;

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "FILosDefs.h"
#ifdef WIN32
#   include <io.h>
#else
#   include <unistd.h>
#endif

#define FILnpos FILindex(-1)

static void FILdisableFileHandleInheritance(int FileDescriptor, const COLstring& FileName)
{
#ifdef WIN32
   // now try to prohibit inheritance of this handle by a child process
   HANDLE WindowsFileHandle = (HANDLE)_get_osfhandle(FileDescriptor);
   if ((long)WindowsFileHandle == -1)
   {
      COL_ERROR_STREAM_PLAIN("Cannot get windows handle for file " << FileName,COLerror::PreCondition);
   }
   DWORD Flags;
   if (!GetHandleInformation(WindowsFileHandle, &Flags))
   {
      COL_THROW_WIN32_ERROR("GetHandleInformation failed");
   }
   Flags &= (~HANDLE_FLAG_INHERIT);
   if (!SetHandleInformation(WindowsFileHandle, HANDLE_FLAG_INHERIT, Flags))
   {
      COL_THROW_WIN32_ERROR("SetHandleInformation failed");
   }
#endif
}

class FILbinaryFilePrivate
{
public:
   FILbinaryFilePrivate() {}
   virtual ~FILbinaryFilePrivate() {}
   virtual void open(const COLstring& FileName, FILbinaryFile::FILmode Mode)=0;
  
   virtual void close()=0;
   
   virtual bool isOpen() const=0;

   virtual FILindex position() const=0;
   virtual void setPosition(FILindex NewPosition)=0;

   virtual COLuint32 read(void* pBuffer, COLuint32 SizeOfBuffer)=0;
   virtual COLuint32 write(const void* pBuffer, COLuint32 SizeOfBuffer)=0;

   virtual FILindex size() const=0;

   virtual void flush()=0;

   virtual bool isEndOfSource() const=0;


   bool tryToRecover(const COLstring& iFileName, FILbinaryFile::FILmode Mode);
   COLstring m_FileName;

#ifdef WIN32
   COLstring m_WindowsFileNameString;
   virtual LPWSTR winFileName() = 0;
   virtual HANDLE win32Handle() = 0;
#endif
};

bool FILbinaryFilePrivate::tryToRecover(const COLstring& iFileName, FILbinaryFile::FILmode Mode){
   // We have failed to open/write/create a file.
   // This routine is designed to recover from some
   // common error conditions such as when the directory does not
   // exist for a log file.
   int SaveErrorNumber = errno;
   if (FILbinaryFile::Read != Mode) {
      // We are writing a file - lets see if the directory existed
      // if it didn't then we'll try and create it. 
      FILfilePath FilePath;
      FilePath.setFileName(iFileName.c_str());
      COLstring Directory;
      FilePath.fullDirectory(Directory);
      if (!FILfileExists(Directory)) {
         // The directory doesn't exist...
#ifdef WIN32
         FILmakeFullDir(Directory, 0x1777);
#else
         FILmakeFullDir(Directory, DEFFILEMODE | S_IXUSR); // need execute bit for directory regardless
#endif      

         return true;
      } else if (!FILisDirectory(Directory.c_str())) {
         COL_ERROR_STREAM_PLAIN("Could not create " << iFileName 
            << " because '" << Directory << "' is not a directory.", COLerror::PreCondition);
      }
   }
   // We didn't manage to recover
   errno = SaveErrorNumber;
   return false;
}


class FILbinaryFilePrivateBuffered 
: public FILbinaryFilePrivate
{
public:
   FILbinaryFilePrivateBuffered(); 
    
   virtual ~FILbinaryFilePrivateBuffered();
   virtual void open(const COLstring& FileName, FILbinaryFile::FILmode Mode);
  
   virtual void close();
   
   virtual bool isOpen() const;

   virtual FILindex position() const;
   virtual void setPosition(FILindex NewPosition);

   virtual COLuint32 read(void* pBuffer, COLuint32 SizeOfBuffer);
   virtual COLuint32 write(const void* pBuffer, COLuint32 SizeOfBuffer);

   virtual FILindex size() const;

   virtual void flush();

   virtual bool isEndOfSource() const;

   FILE* m_pFileHandle;
   COLstring m_Option;
   mutable FILindex m_FileSize;

   enum FILlastOperation
   {
      FLUSH,
      READ,
      WRITE
   };
   
#ifdef WIN32
   virtual LPWSTR winFileName(); 
   virtual HANDLE win32Handle();
#endif

   FILlastOperation LastOperation;
   
};

FILbinaryFilePrivateBuffered::FILbinaryFilePrivateBuffered()
: FILbinaryFilePrivate(),
  m_pFileHandle(NULL),
  m_FileSize(FILnpos),
  LastOperation(FLUSH)
{

}

FILbinaryFilePrivateBuffered::~FILbinaryFilePrivateBuffered(){
   try {
      close();
   } catch(const COLerror& Error) {
      COL_ERR(Error);
   }
}

bool FILbinaryFilePrivateBuffered::isEndOfSource() const{
   COLPRECONDITION(m_pFileHandle != NULL);
   return (0 != feof(m_pFileHandle));
}

#ifdef WIN32
LPWSTR FILbinaryFilePrivateBuffered::winFileName() {
   return (LPWSTR)m_WindowsFileNameString.get_buffer();
}
#endif

static void FILoptionToString(FILbinaryFile::FILmode Mode, COLstring* pOption) {
   COL_FUNCTION(FILoptionToString);
   COLstring& Option = *pOption;
   switch(Mode) {
   case FILbinaryFile::Read:     // open for reading only
      Option = "rb"; 
      break;   
      case FILbinaryFile::Write: // open for reading and writing
      Option = "rb+";
      break;
   case FILbinaryFile::Append:   // open and seek to the end
      Option = "ab+";
      break;
   case FILbinaryFile::Rewrite:  // open, truncate any exisitng file
      Option = "wb+";
      break;
   default:
      COL_ERROR_STREAM("Unknown File Parameter", COLerror::PreCondition);
      break;
   };
}

#ifdef WIN32
void FILbinaryFilePrivateBuffered::open(const COLstring& iFileName, FILbinaryFile::FILmode Mode){
   COL_METHOD(FILbinaryFilePrivateBuffered::open);
   COL_TRC("Opening " + iFileName);
   close();
   LastOperation = FLUSH;
   m_FileName = iFileName;
   FILmangleFileNameForWindows(m_FileName, &m_WindowsFileNameString);
   m_FileSize = FILnpos;
   FILoptionToString(Mode, &m_Option);
   COLstring WinOption;
   LPWSTR pWinOption = FILmangleFileNameForWindows(m_Option, &WinOption);
   m_pFileHandle = _wfopen(winFileName(), pWinOption);
   if (NULL == m_pFileHandle) {
      // tryToRecover will preserve the errno 
      if (tryToRecover(m_FileName, Mode)) {
         m_pFileHandle = _wfopen(winFileName(), pWinOption);
      }
   }
   if (NULL == m_pFileHandle) {   
      // we have a file error
      COL_THROW_STRERROR_PLAIN("Open file '" << m_FileName << "' failed.");
   }
   FILdisableFileHandleInheritance(FILfileno(m_pFileHandle), m_FileName);
   COL_TRC("Success.");
}

# else

// POSIX implementation
void FILbinaryFilePrivateBuffered::open(const COLstring& iFileName, FILbinaryFile::FILmode Mode){
   COL_METHOD(FILbinaryFilePrivateBuffered::open);
   COL_TRC("Opening " + iFileName);
   close();
   LastOperation = FLUSH;
   m_FileName = iFileName;
   m_FileSize = FILnpos;
   FILoptionToString(Mode, &m_Option);
   m_pFileHandle = ::fopen(m_FileName.c_str(), m_Option.c_str());
   if (NULL == m_pFileHandle) {
      // tryToRecover will preserve the errno 
      if (tryToRecover(m_FileName, Mode)) {
         m_pFileHandle = ::fopen(m_FileName.c_str(), m_Option.c_str());
      }
   }
   if (NULL == m_pFileHandle) {   
      // we have a file error
      COL_THROW_STRERROR_PLAIN("Open file '" << m_FileName << "' failed.");
   }
   FILdisableFileHandleInheritance(FILfileno(m_pFileHandle), m_FileName);
   COL_TRC("Success.");
}
#endif

void FILbinaryFilePrivateBuffered::close(){
   COL_METHOD(FILbinaryFilePrivateBuffered::close);
   if (NULL != m_pFileHandle) {
      flush();
      if (EOF == fclose(m_pFileHandle)) {
         m_pFileHandle = NULL;
         COL_THROW_STRERROR_PLAIN("Closing file '" << m_FileName << "' failed.");
      }
   }
   m_pFileHandle = NULL;
   m_FileSize = FILnpos;
}
   
bool FILbinaryFilePrivateBuffered::isOpen() const{
   return m_pFileHandle != NULL;
}

FILindex FILbinaryFilePrivateBuffered::position() const{
   COLPRECONDITION(m_pFileHandle != NULL);
   long Position = ftell(m_pFileHandle);
   if (Position == FILnpos) {
      COL_THROW_STRERROR_PLAIN("ftell failed on '" << m_FileName << "'.");
   }
   return FILindex(Position);
}

void FILbinaryFilePrivateBuffered::setPosition(FILindex NewPosition){
   COLPRECONDITION(m_pFileHandle != NULL);
   if (0 != fseek(m_pFileHandle, (long)NewPosition, SEEK_SET)) {
      COL_THROW_STRERROR_PLAIN("fseek failed to move to " << NewPosition << " on '" << m_FileName << "'.");
   }
}

COLuint32 FILbinaryFilePrivateBuffered::read(void* pBuffer, COLuint32 SizeOfBuffer){
   COLPRECONDITION(m_pFileHandle != NULL);
   if (LastOperation == WRITE) {
      flush();
   }
   LastOperation = READ;

   COLuint32 AmountRead = fread(pBuffer, 1, SizeOfBuffer, m_pFileHandle);
   if (AmountRead < SizeOfBuffer) {
      // we need to check to see if we have an error or if we simply have reached the
      // end of the file
      if (ferror(m_pFileHandle)) {
         COL_THROW_STRERROR_PLAIN("fread failed on '" << m_FileName << "'.");
      }
   }
   return AmountRead;
}

COLuint32 FILbinaryFilePrivateBuffered::write(const void* pBuffer, COLuint32 SizeOfBuffer){
   COLPRECONDITION(m_pFileHandle != NULL);
   if (SizeOfBuffer == 0)   {
      return SizeOfBuffer;
   }
   if (LastOperation == READ)   {
      flush();
   }
   LastOperation = WRITE;

   m_FileSize = FILnpos;
   // initialize the value of errno before calling fwrite
   errno = 0;
   size_t CountWritten = fwrite(pBuffer, 1, SizeOfBuffer, m_pFileHandle );
   if (CountWritten == SizeOfBuffer) {
      return SizeOfBuffer; // we are good
   }
   // We may have got to the end of the file.  Our desired behaviour
   // in this case is to move the file position to before the end of
   // the file and quietly recover
   if (errno == 0) {  // fwrite has not changed the value of errno
      setPosition(size());
      m_FileSize = FILnpos;
      CountWritten = fwrite(pBuffer, 1, SizeOfBuffer, m_pFileHandle);
      if (CountWritten == SizeOfBuffer)  {
         return SizeOfBuffer;
      }
   }
   COL_THROW_STRERROR_PLAIN("fwrite failed on '" << m_FileName << "'.");
}

FILindex FILbinaryFilePrivateBuffered::size() const{
   COLPRECONDITION(m_pFileHandle != NULL);
   if (m_FileSize == FILnpos)   {
      // We haven't got a cached value of the file size
      FILindex OldPosition = position();

      if (0 != fseek(m_pFileHandle, 0, SEEK_END)) {
         COL_THROW_STRERROR_PLAIN("fseek during size() call failed on '" << m_FileName << "'.");
      }
      m_FileSize = position();
      ((FILbinaryFilePrivateBuffered*)this)->setPosition(OldPosition);
   }
   return m_FileSize;
}

void FILbinaryFilePrivateBuffered::flush(){
   COLPRECONDITION(m_pFileHandle != NULL);
   if (m_Option == "rb" ) {
       // this is read only file - so we cannot flush
       return;
   }
   if (fflush(m_pFileHandle) != 0) {
      COL_THROW_STRERROR_PLAIN("fflush failed on '" << m_FileName << "'.");
   }
   LastOperation = FLUSH;
}

#ifdef WIN32
HANDLE FILbinaryFilePrivateBuffered::win32Handle(){
   if (!m_pFileHandle) {
      return 0;
   }
   return (HANDLE)_get_osfhandle( FILfileno(m_pFileHandle) );
}
#endif

class FILbinaryFilePrivateUnbuffered 
: public FILbinaryFilePrivate
{
public:
   FILbinaryFilePrivateUnbuffered();
   virtual ~FILbinaryFilePrivateUnbuffered();
   virtual void open(const COLstring& FileName, FILbinaryFile::FILmode Mode);
  
   virtual void close();
   
   virtual bool isOpen() const;

   virtual FILindex position() const;
   virtual void setPosition(FILindex NewPosition);

   virtual COLuint32 read(void* pBuffer, COLuint32 SizeOfBuffer);
   virtual COLuint32 write(const void* pBuffer, COLuint32 SizeOfBuffer);

   virtual FILindex size() const;

   virtual void flush();

   virtual bool isEndOfSource() const;

#ifdef WIN32
   virtual HANDLE win32Handle();
   virtual LPWSTR winFileName();
#endif

   int m_FileHandle;
   int m_OptionFlag;
   mutable FILindex m_FileSize;
};

FILbinaryFilePrivateUnbuffered::FILbinaryFilePrivateUnbuffered()
: FILbinaryFilePrivate(),
  m_FileSize(FILnpos),
  m_OptionFlag(-1),
  m_FileHandle(-1){}

FILbinaryFilePrivateUnbuffered::~FILbinaryFilePrivateUnbuffered(){
   try {
      close();
   }
   catch(const COLerror& Error) {
      COL_ERR(Error);
   }
}

#ifdef WIN32
wchar_t* FILbinaryFilePrivateUnbuffered::winFileName() {
   return (wchar_t*)m_WindowsFileNameString.get_buffer();
}
#endif

void FILbinaryFilePrivateUnbuffered::open(const COLstring& iFileName, FILbinaryFile::FILmode Mode){
   COL_METHOD(FILbinaryFilePrivateBuffered::open);
   COL_TRC("Opening " + iFileName);
   close();
   m_FileName = iFileName;

#ifdef WIN32
   FILmangleFileNameForWindows(m_FileName, &m_WindowsFileNameString);
#endif

   m_FileSize = FILnpos;

   switch(Mode) {
   case FILbinaryFile::Read:
      m_OptionFlag = O_BINARY | O_RDONLY; 
      break;    // open for reading only
   case FILbinaryFile::Write:   // open for reading and writing
      m_OptionFlag = O_BINARY | O_RDWR;
      break;
   case FILbinaryFile::Append:  // open and seek to the end
      m_OptionFlag = O_BINARY | O_RDWR | O_CREAT | O_APPEND;
      break;
   case FILbinaryFile::Rewrite: // open, truncate any exisitng file
      m_OptionFlag = O_BINARY | O_RDWR | O_CREAT | O_TRUNC;
      break;
   default:
      COL_ERROR_STREAM("Unknown File Parameter", COLerror::PreCondition);
      break;
   };
   if (FILbinaryFile::Read == m_OptionFlag) {

#ifdef WIN32
      m_FileHandle = _wopen(winFileName(), m_OptionFlag);
#else
      m_FileHandle = ::open(m_FileName.c_str(), m_OptionFlag);
#endif

   } else {

#ifdef WIN32
      m_FileHandle = _wopen(winFileName(), m_OptionFlag, S_IREAD | S_IWRITE);
#else
      m_FileHandle = ::open(m_FileName.c_str(), m_OptionFlag, S_IREAD | S_IWRITE);
#endif

      if (-1 == m_FileHandle) {
         // tryToRecover will preserve the errno - currently this assumption works
         // on the basis that the only kind of error we can recover from is when the
         // directory does not exist - tryToRecover will check for this and attempt
         // to create the directory when possible.
         if (tryToRecover(m_FileName, Mode)) {

#ifdef WIN32
            m_FileHandle = _wopen(winFileName(), m_OptionFlag, S_IREAD | S_IWRITE);
#else
            m_FileHandle = ::open(m_FileName.c_str(), m_OptionFlag, S_IREAD | S_IWRITE);
#endif

         }
      }
   }
   if (-1 == m_FileHandle) {
      // we have a file error
      COL_THROW_STRERROR_PLAIN("Open file '" << m_FileName << "' failed.");
   }  
   FILdisableFileHandleInheritance(m_FileHandle, m_FileName);
   COL_TRC("Success.");
}

void FILbinaryFilePrivateUnbuffered::close(){
   COL_METHOD(FILbinaryFilePrivateUnbuffered::close);
   if (-1 != m_FileHandle) {
      if (-1 == ::FILclose(m_FileHandle)) {
         m_FileHandle = -1;
         COL_THROW_STRERROR_PLAIN("Closing file '" << m_FileName << "' failed.");
      }
   }
   m_FileHandle = -1;
   m_FileSize = FILnpos;
}

bool FILbinaryFilePrivateUnbuffered::isEndOfSource() const {
   COLPRECONDITION(m_FileHandle != -1);

#ifdef WIN32
   int ReturnCode = FILeof(m_FileHandle);
   if (ReturnCode == -1)
   {
      COL_THROW_STRERROR_PLAIN("_eof failed.");
   }
   return (ReturnCode == 1);
#else
   off_t CurrentPosition = lseek(m_FileHandle, 0, SEEK_CUR);
   off_t EndOfFilePosition = lseek(m_FileHandle, 0, SEEK_END);
   lseek(m_FileHandle, CurrentPosition, SEEK_SET);

   return (CurrentPosition == EndOfFilePosition);
#endif
}
   
bool FILbinaryFilePrivateUnbuffered::isOpen() const{
   return m_FileHandle != -1;
}

FILindex FILbinaryFilePrivateUnbuffered::position() const{
   COLPRECONDITION(m_FileHandle != -1);
   // permit files of up to (2^64) - 1 bytes
   COLuint64 Position = FILlseek64(m_FileHandle, 0, SEEK_CUR);
   if (Position == -1) {
      COL_THROW_STRERROR_PLAIN("lseek failed on '" << m_FileName << "'.");
   }
   return FILindex(Position);
}

void FILbinaryFilePrivateUnbuffered::setPosition(FILindex NewPosition){
   COLPRECONDITION(m_FileHandle != -1);
   
   if (-1 == FILlseek64(m_FileHandle, NewPosition, SEEK_SET)) {
      COL_THROW_STRERROR_PLAIN("lseek failed to move to " << NewPosition << " on '" << m_FileName << "'.");
   }
}

COLuint32 FILbinaryFilePrivateUnbuffered::read(void* pBuffer, COLuint32 SizeOfBuffer) {
   COLPRECONDITION(m_FileHandle != -1);
   COLuint32 AmountRead = ::FILread(m_FileHandle, pBuffer, SizeOfBuffer);
   
   if (AmountRead == -1 ) {
      COL_THROW_STRERROR_PLAIN("read failed on '" << m_FileName << "'.");
   }
   return AmountRead;
}

COLuint32 FILbinaryFilePrivateUnbuffered::write(const void* pBuffer, COLuint32 SizeOfBuffer){
   COLPRECONDITION(m_FileHandle != -1);
   COLuint32 AmountWritten = ::FILwrite(m_FileHandle, pBuffer, SizeOfBuffer);

   if (AmountWritten == -1 || AmountWritten < SizeOfBuffer) {
      COL_THROW_STRERROR_PLAIN("write failed on '" << m_FileName << "'.");
   }
   m_FileSize = FILnpos;

   return SizeOfBuffer;
}

FILindex FILbinaryFilePrivateUnbuffered::size() const{
   COLPRECONDITION(m_FileHandle != -1);
   if (m_FileSize == FILnpos){
      // We haven't got a cached value of the file size
      FILindex OldPosition = position();

      m_FileSize = FILlseek64(m_FileHandle, 0, SEEK_END);

      if (FILnpos == m_FileSize) {
         COL_THROW_STRERROR_PLAIN("lseek during size() call failed on '" << m_FileName << "'.");
      }
      ((FILbinaryFilePrivateUnbuffered*)this)->setPosition(OldPosition);
   }
   return m_FileSize;
}

void FILbinaryFilePrivateUnbuffered::flush() {
   if (m_OptionFlag == O_BINARY) { //in read only mode
      // this is read only file - so we cannot flush
      return;
   }
   if (-1 == ::FILcommit(m_FileHandle)) {
      // This often happens if the file has not be opened for writing to it.
      COL_THROW_STRERROR_PLAIN("commit flush called failed on '" << m_FileName << "'.");
   }
}

#ifdef WIN32
HANDLE FILbinaryFilePrivateUnbuffered::win32Handle(){
   if (m_FileHandle == -1) {
      return 0;
   }
   return (HANDLE)_get_osfhandle(m_FileHandle);
}
#endif

FILbinaryFile::FILbinaryFile(const COLstring& FileName, FILmode Mode, FILtype Type): pMember(NULL){
   COL_METHOD(FILbinaryFile::FILbinaryFile);
   if (Buffered == Type){
      pMember = new FILbinaryFilePrivateBuffered();      
   } else {
      pMember = new FILbinaryFilePrivateUnbuffered();      
   }
   try {
      COL_TRC("About to open " << FileName);
      open(FileName, Mode);
   }
   catch(...)
   {
      COL_ERR("Exception while constructing file.");
      delete pMember;
      COL_TRC("Cleaned up after exception - about to re-throw error for application");
      throw;
   }
}  

FILbinaryFile::FILbinaryFile(FILtype Type)
 : pMember(NULL)
{
   if (Buffered == Type) {
      pMember = new FILbinaryFilePrivateBuffered();      
   } else {
      pMember = new FILbinaryFilePrivateUnbuffered();      
   }
}

FILbinaryFile::~FILbinaryFile(){
   delete pMember;
}

void FILbinaryFile::open(const COLstring& FileName, FILmode Mode){
   COL_METHOD(FILbinaryFile::open);
   pMember->open(FileName, Mode);
}
  
void FILbinaryFile::close(){
   pMember->close();
}
   
bool FILbinaryFile::isOpen() const{
   return pMember->isOpen();
}

COLindex FILbinaryFile::position() const{
   COLPRECONDITION(pMember->position() <= COL_UINT_32_MAX);
   return (COLindex)pMember->position();
}

void FILbinaryFile::setPosition(COLindex NewPosition){
   pMember->setPosition(NewPosition);
}

COLuint64 FILbinaryFile::position64() const{
   return pMember->position();
}

void FILbinaryFile::setPosition64(COLuint64 NewPosition){
   pMember->setPosition(NewPosition);
}

COLuint32 FILbinaryFile::read(void* pBuffer, COLuint32 SizeOfBuffer){
   return pMember->read(pBuffer, SizeOfBuffer);
}

COLuint32 FILbinaryFile::write(const void* pBuffer, COLuint32 SizeOfBuffer){
   return pMember->write(pBuffer, SizeOfBuffer);
}

void FILbinaryFile::readAll(COLsink* pDest){
   COLPRECONDITION(pDest);

   COLsimpleBuffer Buffer(BUFSIZ);
   COLuint32 AmountRead;

   // Keep reading BUFSIZ buffers until we run out (or throw),
   // writing them into the destination.
   bool EndOfFile = false;
   while (!EndOfFile) {
      AmountRead = read(Buffer.data(), BUFSIZ);

      if (AmountRead < BUFSIZ) {
         EndOfFile = true;
      }

      pDest->write(Buffer.data(), AmountRead);
   }

   pDest->flush();
   pDest->onEndStream();   // Is this necessary?
   resetSourcePosition();
}

void FILbinaryFile::resetSourcePosition(){
   setPosition(0);
}

void FILbinaryFile::setSourcePosition(COLuint32 ByteOffset){
   setPosition(ByteOffset);   
}

COLindex FILbinaryFile::readPosition() const{
   return position();
}

void FILbinaryFile::setReadPosition(COLindex Position){
   setPosition(Position);
}

FILindex FILbinaryFile::size() const{
   return pMember->size();
}

const COLstring& FILbinaryFile::name() const{
   return pMember->m_FileName;
}

void FILbinaryFile::flush(){
   pMember->flush();
}

void FILbinaryFile::setName(const COLstring& NewName){
   pMember->m_FileName = NewName;
}

bool FILbinaryFile::isEndOfReadable() const{
   return pMember->isEndOfSource();
}

#ifdef WIN32
HANDLE FILbinaryFile::win32Handle()
{
   return pMember->win32Handle();
}
#endif
