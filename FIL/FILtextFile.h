#ifndef __FIL_TEXT_FILE_H_
#define __FIL_TEXT_FILE_H_
//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2005 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILtextFile
//
// Description:
//
// This object represents a text file.  It is implemented on top of
// FILbinaryFile
//
// LIMITATIONS
//
// This is not a multibyte aware implementation.
//
// Author: Eliot Muir
// Date:   Fri 22/01/2004 
//
//
//---------------------------------------------------------------------------

#include <FIL/FILbinaryFile.h>

class FILtextFilePrivate;

class COL_DLL FILtextFile : private FILbinaryFile
{
public:
   // Note that this class is actually doing some internal buffering already.
   FILtextFile(FILbinaryFile::FILtype Type=FILbinaryFile::Buffered);
   virtual ~FILtextFile();

   void open(const COLstring&  FileName, FILbinaryFile::FILmode Mode){
      FILbinaryFile::open(FileName, Mode);
   }
  
   virtual void close();
   
   bool isOpen() const { return FILbinaryFile::isOpen(); }

   COLindex position() const;
   void setPosition(COLindex NewPosition);

   // a pair of methods for positioning in files
   // with 64 bit addressing
   COLuint64 position64() const;
   void setPosition64(COLuint64 NewPosition);

   FILindex size() const { return FILbinaryFile::size(); }

   bool readCharacter(char& Character);
   void writeCharacter(char Character);

   bool readLine(COLstring& Line);
   void writeLine(const COLstring& Line);

   const COLstring& name() const { return FILbinaryFile::name(); }   

   virtual void flush();
private:
   FILtextFilePrivate* pMember;
   friend class FILtextFilePrivate;

   FILtextFile(const FILtextFile& Orig); // not allowed
   FILtextFile& operator=(const FILtextFile& Orig); // not allowed
};

#endif // end of defensive include
