//---------------------------------------------------------------------------
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: SFIhexDecodeFilter
//
// Description:
//
// Implementation.
//
// Author: Henry Tran
// Date: July 6, 2004
//
//
//---------------------------------------------------------------------------
#include "SFIprecomp.h"
#pragma hdrstop

#include "SFIhexDecodeFilter.h"

#include <COL/COLsinkString.h>
#include <COL/COLlog.h>
COL_LOG_MODULE;

#include <string.h>

class SFIhexDecodeFilterPrivate
{
public:
   SFIhexDecodeFilterPrivate() : HexBufferCount(0) {}
   ~SFIhexDecodeFilterPrivate() {}

   COLuint8 HexBufferCount;
   COLuint8 HexBuffer[2];
};

// TODO - we should unify this library with a single HEX library that will do fast HEX
// conversions - see work that Yunqi is doing on FMT library.
static inline COLuint8 valueOfHexByte(char HexCharacter)
{
   if (HexCharacter >= '0' && HexCharacter <= '9')
   {
      return HexCharacter - '0';
   }
   else if (HexCharacter >= 'A' && HexCharacter <= 'F')
   {
      return HexCharacter - 'A' + 10;
   }
   else if (HexCharacter >= 'a' && HexCharacter <= 'f')
   {
      return HexCharacter - 'a' + 10;
   }
   else
   {
      COL_ERROR_STREAM_PLAIN("Invalid character in hex decoding: '" << HexCharacter << '\'', COLerror::PreCondition);
   }
   return 0;
}

SFIhexDecodeFilter::SFIhexDecodeFilter(COLsink* ipNext)
: COLfilter(ipNext)
{
   pMember = new SFIhexDecodeFilterPrivate();
}

SFIhexDecodeFilter::~SFIhexDecodeFilter()
{
   delete pMember;
}

void SFIhexDecodeFilter::resetFilter()
{
   pMember->HexBufferCount = 0;
}

COLuint32 SFIhexDecodeFilter::unprocessedCharacterCount() const
{
   return pMember->HexBufferCount;
}

COLuint32 SFIhexDecodeFilter::write(const void* pBuffer, COLuint32 SizeOfBuffer)
{
   COLPRECONDITION(pMember->HexBufferCount < 2);

   const COLuint8* pBufferCharacters = (const COLuint8*)pBuffer;
   COLuint8* HexBuffer = pMember->HexBuffer;
   const int OutBufferSize = 128;
   COLuint8 OutBuffer[OutBufferSize]; // minimize expensive calls to next().write()
   int OutBufferCount = 0;
   COLsink& Output = next();

   for (int i = 0; i < (int)SizeOfBuffer; ++i)
   {
      HexBuffer[pMember->HexBufferCount++] = pBufferCharacters[i];
      if (pMember->HexBufferCount == 2)
      {
         COLuint8 DecodedByte = (valueOfHexByte(HexBuffer[0]) << 4) | valueOfHexByte(HexBuffer[1]);
         OutBuffer[OutBufferCount++] = DecodedByte;
         pMember->HexBufferCount = 0;

         if (sizeof(OutBuffer) == OutBufferCount)
         {
            Output.write(OutBuffer, OutBufferCount);
            OutBufferCount = 0;
         }
      }
   }

   if (OutBufferCount > 0)
   {
      Output.write(OutBuffer, OutBufferCount);
   }

   return SizeOfBuffer;
}

// TODO could be made more efficient one day
COLstring SFIhexDecode(void* pBuffer, int Size){
   COL_FUNCTION(SFIhexDecode);
   COLstring Result;
   COLsinkString Sink(Result);
   SFIhexDecodeFilter Filter(&Sink);
   Filter.write(pBuffer, Size);
   return Result;
}

COLstring SFIhexDecode(const COLstring& Input){
   return SFIhexDecode((void*)Input.c_str(),Input.size());
}
