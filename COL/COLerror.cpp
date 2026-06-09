//---------------------------------------------------------------------------
// Copyright (C) 1997-2014 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLerror
//
// Description: Implementation
//
// Error class used in project
//
// Author: Eliot Muir
// Date:   Wed 09/09/1998 
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include "COLerror.h"
#include "COLlog.h"
COL_LOG_MODULE;

#ifndef WIN32
   #include <errno.h>
   #include "COLmutex.h"
   #include "COLlocker.h"
#endif

#include <COL/COLdateTime.h>

#if defined(CHM_LINUX) || defined(CHM_MAC)
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif

#ifdef WIN32 
COLstring COLformatWin32Error(long ErrorCode)
{
   char* MessageBuffer;    
   DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                         FORMAT_MESSAGE_IGNORE_INSERTS | 
                         FORMAT_MESSAGE_FROM_SYSTEM;

   ::FormatMessage(dwFormatFlags,
      NULL, // module to get message from (NULL == system)
      ErrorCode,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
      (char*) &MessageBuffer,        
      0,        
      NULL);
   COLstring Result = MessageBuffer;
   LocalFree(MessageBuffer);
   return Result;
}

void COLthrowWin32Error
(
   long ErrorCode,
   const char* Description,
   COLuint32   LineNumber,
   const char* FileName
)
{
   COLsinkString ErrorSink;
   COLostream MessageStream(ErrorSink);
   if (Description)
   {
      MessageStream << Description;
   }
   if (LineNumber != COLuint32(-1) && (FileName != NULL))
   {
      MessageStream << " on line " << LineNumber << " in " << FileName << " - ";
   }  
   MessageStream << COLformatWin32Error(ErrorCode);
   throw COLerror(ErrorSink.string(), (COLuint32)ErrorCode);
}

long COLlastError()
{
   return ::GetLastError();
}

#endif // WIN32

COLstring COLbacktrace()
{
#if defined(CHM_LINUX) || defined(CHM_MAC)
   const int MaxFrames = 250;
   void* CallStack[MaxFrames];
   const int StackDepth = ::backtrace(CallStack, MaxFrames);
   COLostreamString bt;
   bt << "\nStack trace captured " << COLdateTime::currentTime() << '\n';

   // NOTE: This line is required for stripped relocated binaries
   // to sync stack addresses with addrline on Linux and atos on Mac.
   bt << "COLbacktrace: " << (void*)&COLbacktrace << '\n';

   if (char** SymbolArray = ::backtrace_symbols(CallStack, StackDepth)) {
      for (int i = 0; i < StackDepth; i++) {
         if (const char* Symbol = SymbolArray[i]) {
            bt << Symbol << '\n';
         } else {
            bt << i << ": " << CallStack[i] << '\n';
         }
      }
      ::free(SymbolArray);
   } else {
      for (int i = 0; i < StackDepth; i++) {
         bt << i << ": " << CallStack[i] << '\n';
      }
   }
   return bt.str();
#else
   return COLstring();
#endif
}

static COLstring COLerrorAsString(int ErrorCode)
{
   switch ((COLerror::COLerrorCode)ErrorCode) {
   case COLerror::PreCondition:       return "PreCondition";
   case COLerror::PostCondition:      return "PostCondition";
   case COLerror::FileError:          return "FileError";
   case COLerror::SerializationError: return "SerializationError";
   case COLerror::SystemError:        return "SystemError";
   case COLerror::InvalidDateTime:    return "InvalidDateTime";
   case COLerror::ParsingError:       return "ParsingError";
   case COLerror::Cancel:             return "Cancel";
   default:
      break;
   }
   COLostreamString oss;
   oss << "UnknownCOLerror " << ErrorCode;
   return oss.str();
}

COLerror::COLerror(const COLstring& Message, int Line, const char* File, int Code) :
   m_Description(Message),
   m_Code(Code),
   m_File(File),
   m_Line(Line)
{
   if (Code && Message.size()) {
      COL_ERR("Message: `" << Message << "`, Code: " << COLerrorAsString(Code)
         << ", File: " << File << ", Line " << Line << COLbacktrace());
   }
}

COLerror::COLerror(const COLstring& Message, int Code) :
   m_Description(Message),
   m_Code(Code),
   m_Line(0)
{
   if (Code && Message.size()) {
      COL_ERR("Message: `" << Message << "`, Code: " << COLerrorAsString(Code) << COLbacktrace());
   }
}

COLerror::COLerror(const COLerror& Error, int NewCode) :
   m_Description(Error.m_Description),
   m_File(Error.m_File),
   m_Line(Error.m_Line),
   m_Code(NewCode) // assign new code
{}

COLerror::COLerror(const COLerror& Error) :
   m_Description(Error.m_Description),
   m_Code(Error.m_Code),
   m_File(Error.m_File),
   m_Line(Error.m_Line)
{}

COLerror::~COLerror() {
}

void COLerror::setDescription(const COLstring& Description)
{
   m_Description = Description;
}

void COLerror::setCode(int NewCode)
{
   m_Code = NewCode;
}

COLerror& COLerror::operator=(const COLerror& Error)
{
   m_Description = Error.m_Description;
   m_Code        = Error.m_Code;
   m_File        = Error.m_File;
   m_Line        = Error.m_Line;

   return *this;
}

bool COLerror::operator==(const COLerror& Rhs) const
{
   return m_Code == Rhs.m_Code && m_Description == Rhs.m_Description;
}

COLostream& operator<<(COLostream& Stream, const COLerror& Error)
{
   Stream << "Error Code: 0x" << hex << Error.code() << decimal << newline
      << "Description: " << Error.description() << newline;
   if (Error.file().size() > 0)
   {
      Stream << "Line: " << Error.line() << newline
         << "File: " << Error.file() << newline;
   }

   return Stream;
}

COLstring COLstrerror(int ErrorNumber)
{
#ifdef WIN32
   return COLstring(strerror(ErrorNumber));  // WIN32 is thread safe since it's a lookup call.
#else // POSIX
   // For some POSIX platforms, errno does not seem to be set properly. We get error messages like
   // "Something failed: Error 0" or worse, "Something failed: Success!"
   static COLmutex Mutex;
   COLlocker Locker(Mutex);
   if (0 != ErrorNumber)
   {
      return COLstring(strerror(ErrorNumber));
   }
   return "No error info. (errno may not have been set - it was 0)";
#endif // NOT WIN32
}

void COLabort()
{
   abort();
}

void COLabortWithMessage(const COLstring& ErrorString)
{
   COL_ERR(ErrorString << COLbacktrace());
   COLcout << "Fatal Error. Abort with message: " << ErrorString << COLbacktrace() << flush;
#ifdef WIN32   
   ::OutputDebugString(ErrorString.c_str());
#endif
   abort();
}

void COLassertDefaultCallback(COLostream&) 
{ 
}

bool COLassertSettings::AbortOnAssert = false;
COLassertCallback COLassertSettings::Callback = COLassertDefaultCallback;

void COLassertSettings::setAbortOnAssert(bool Value)
{
   AbortOnAssert = Value;
}

bool COLassertSettings::abortOnAssert()
{
   return AbortOnAssert;
}

void COLassertSettings::setCallback(COLassertCallback Func) {
   Callback = Func;
}

COLassertCallback COLassertSettings::callback() {
   return Callback;
}
