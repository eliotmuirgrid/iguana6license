//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2008 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILtextFile
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Fri 22/01/2004 
//
//---------------------------------------------------------------------------
#include "FILprecomp.h"
#pragma hdrstop

#include "FILtextFile.h"

#include <COL/COLstring.h>
#include <COL/COLsimpleBuffer.h>
#include <COL/COLlog.h>
COL_LOG_MODULE;

class FILtextFilePrivate
{
public:
   FILtextFilePrivate(FILtextFile* ipParent) 
    : ReadBuffer(1000),
      WriteBuffer(1000),
      NextReadCharIndex(0),
      LastReadCharIndex(0),
      LastWriteCharIndex(0),
      pParent(ipParent)
   {}

   ~FILtextFilePrivate() {}

   bool readCharacter(char& Character);

   void writeCharacter(char Character);
   void read();
   void write();

   bool readLine(COLstring& Line);
   void writeLine(const COLstring& Line);

   COLuint32 NextReadCharIndex;
   COLuint32 LastReadCharIndex;
   COLsimpleBuffer ReadBuffer;
   COLuint32 LastWriteCharIndex;
   COLsimpleBuffer WriteBuffer;
   FILtextFile* pParent;
};

FILtextFile::FILtextFile(FILbinaryFile::FILtype Type) 
: FILbinaryFile(Type) 
{
   pMember = new FILtextFilePrivate(this);
}

FILtextFile::~FILtextFile() {
   try {
      close();
      delete pMember;
   }

   catch(const COLerror& Error) {
      COL_ERR(Error);
   }
}

void FILtextFile::close() {
   if (FILbinaryFile::isOpen()) {
      flush();
      FILbinaryFile::close();
   }
}

bool FILtextFilePrivate::readLine(COLstring& Line){
   Line.clear();
   char Character;
   bool EndOfFile = true;
   // We assume that appending to COLstring is a fairly efficient operation.
   while (readCharacter(Character)) {
      EndOfFile = false;
      // We could use a lookup table here for better performance...
      if ('\n' == Character) {
         if (Line.size() > 0 && Line[Line.size()-1] == '\r') {
            // get rid of \r
            Line.remove(Line.size()-1, 1);            
         }
         break;
      }  
      Line += Character;
   }
   return !EndOfFile;
}

void FILtextFilePrivate::writeLine(const COLstring& Line){
   for (COLuint32 CharIndex = 0; CharIndex < Line.size(); CharIndex++) {
      writeCharacter(Line[CharIndex]);      
   }
#ifdef WIN32
   writeCharacter('\r');
#endif
   writeCharacter('\n');
}

bool FILtextFilePrivate::readCharacter(char& Character) {
   if (NextReadCharIndex == LastReadCharIndex) {
      read();
   }
   if (LastReadCharIndex == 0) {
      return false;
   }
   Character = ReadBuffer[NextReadCharIndex];
   NextReadCharIndex++;
   return true;
}

void FILtextFilePrivate::read() {
   LastReadCharIndex = pParent->FILbinaryFile::read(ReadBuffer.data(), ReadBuffer.size());
   NextReadCharIndex = 0;
}

bool FILtextFile::readCharacter(char& Character) {
   return pMember->readCharacter(Character);
}

void FILtextFilePrivate::writeCharacter(char Character) {
   if (LastWriteCharIndex == WriteBuffer.size()) {
      write();
   }

   WriteBuffer[LastWriteCharIndex] = Character;
   LastWriteCharIndex++;
}

void FILtextFilePrivate::write() {
   pParent->FILbinaryFile::write(WriteBuffer.data(), LastWriteCharIndex);
   LastWriteCharIndex = 0;
}

void FILtextFile::writeCharacter(char Character) {
   pMember->writeCharacter(Character);
}

bool FILtextFile::readLine(COLstring& Line) {
   return pMember->readLine(Line);
}

void FILtextFile::writeLine(const COLstring& Line) {
   pMember->writeLine(Line);
}

void FILtextFile::flush() {
   if (pMember->LastWriteCharIndex > 0) {
      pMember->write();
   }
   FILbinaryFile::flush(); 
}

COLindex FILtextFile::position() const {
   return FILbinaryFile::position() -  pMember->LastReadCharIndex + pMember->NextReadCharIndex;
}

void FILtextFile::setPosition(COLindex NewPosition) {
   flush();
   pMember->LastReadCharIndex = 0;
   pMember->NextReadCharIndex = 0;
   FILbinaryFile::setPosition(NewPosition);
}

COLuint64 FILtextFile::position64() const {
   return FILbinaryFile::position64() - pMember->NextReadCharIndex + pMember->LastWriteCharIndex;
}

void FILtextFile::setPosition64(COLuint64 NewPosition) {
   flush();
   pMember->LastReadCharIndex = 0;
   pMember->NextReadCharIndex = 0;
   FILbinaryFile::setPosition64(NewPosition);
}

