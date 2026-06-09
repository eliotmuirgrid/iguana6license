//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: DRGfilterEncrypt
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

#include "DRGprecomp.h"
#pragma hdrstop

#include "DRGfilterEncrypt.h"
#include <SFI/SFIaesEncryptor.h>
#include <SFI/SFImd5.h>
#include <COL/COLsimpleBuffer.h>

class DRGfilterEncryptPrivate
{
public:
   DRGfilterEncryptPrivate()
      : pEncryptor( NULL ),
        AmountInBuffer( 0 ),
        SizeOfEncryptionKey( 0 ),
        KeyWritten( false ) {}
   ~DRGfilterEncryptPrivate();

   SFIaesEncryptor* pEncryptor;

   COLuint8 EncryptionBuffer[ DRG_AES_SIZE_OF_BLOCK ];
   COLuint8 AmountInBuffer;

   COLuint8 EncryptionKey[ DRG_AES_SIZE_OF_KEY ];
   COLuint8 SizeOfEncryptionKey;
   bool KeyWritten;
};

DRGfilterEncryptPrivate::~DRGfilterEncryptPrivate()
{
   if ( pEncryptor != NULL )
   {
      delete pEncryptor;
   }
}

DRGfilterEncrypt::DRGfilterEncrypt( COLsink* pNext_ )
   : COLfilter( pNext_ )
{
   pMember = new DRGfilterEncryptPrivate();
}

DRGfilterEncrypt::~DRGfilterEncrypt()
{
   delete pMember;
}

void DRGfilterEncrypt::setEncryptionKey( const COLuint8* pEncryptionKey, COLuint8 SizeOfEncryptionKey )
{
   if ( SizeOfEncryptionKey > DRG_AES_SIZE_OF_KEY )
   {
      COL_ERROR_STREAM_PLAIN( "Encryption key is larger than maximum size of "
                              << DRG_AES_SIZE_OF_KEY << " bytes.", COLerror::PreCondition );
   }

   ::memset( pMember->EncryptionKey, '\0', DRG_AES_SIZE_OF_KEY );
   ::memcpy( pMember->EncryptionKey, pEncryptionKey, SizeOfEncryptionKey );

   pMember->SizeOfEncryptionKey = SizeOfEncryptionKey;

   if ( pMember->pEncryptor != NULL )
   {
      delete pMember->pEncryptor;
   }

   pMember->pEncryptor = new SFIaesEncryptor( pMember->EncryptionKey );
}

const COLuint8* DRGfilterEncrypt::encryptionKey() const
{
   COLPRECONDITION( pMember->pEncryptor != NULL )
   return pMember->EncryptionKey;
}

COLuint8 DRGfilterEncrypt::sizeOfEncryptionKey() const
{
   COLPRECONDITION( pMember->pEncryptor != NULL )
   return pMember->SizeOfEncryptionKey;
}

COLuint32 DRGfilterEncrypt::write( const void* pBuffer, COLuint32 SizeOfBuffer )
{
   const COLuint32 OriginalSizeOfBuffer = SizeOfBuffer;

   if ( pMember->pEncryptor == NULL )
   {
      COL_ERROR_STREAM( "Need to set encryption key for AES encryptor.", COLerror::PreCondition );
   }

   if ( !pMember->KeyWritten )
   {
      SFImd5Hasher Hasher;
      Hasher.write( pMember->EncryptionKey, DRG_AES_SIZE_OF_KEY );
      next().write( Hasher.hash().data(), DRG_AES_SIZE_OF_BLOCK );
      pMember->KeyWritten = true;
   }
   
   COLuint8 AmountToWrite;
   while ( SizeOfBuffer > 0 )
   {
      COLuint32 MaxSizeOfBlock = DRG_AES_SIZE_OF_BLOCK - 1; // First byte of block stores block size
      
      if ( SizeOfBuffer > (MaxSizeOfBlock - pMember->AmountInBuffer) )
      {
         AmountToWrite = MaxSizeOfBlock - pMember->AmountInBuffer;
      }
      else
      {
         AmountToWrite = SizeOfBuffer;
      }

      ::memcpy( pMember->EncryptionBuffer + 1 + pMember->AmountInBuffer, pBuffer, AmountToWrite );
      pMember->AmountInBuffer += AmountToWrite;
      
      if( pMember->AmountInBuffer == MaxSizeOfBlock )
      {
         pMember->EncryptionBuffer[0] = MaxSizeOfBlock;

         pMember->pEncryptor->ProcessBlock( pMember->EncryptionBuffer, pMember->EncryptionBuffer );
         
         next().write( pMember->EncryptionBuffer, DRG_AES_SIZE_OF_BLOCK );
         pMember->AmountInBuffer = 0;
      }

      if ( SizeOfBuffer > AmountToWrite )
      {
         pBuffer = (COLuint8*) pBuffer + AmountToWrite;
      }
      SizeOfBuffer -= AmountToWrite;
   }

   return OriginalSizeOfBuffer;
}

void DRGfilterEncrypt::flush()
{
   if ( pMember->pEncryptor == NULL )
   {
      COL_ERROR_STREAM( "Need to set encryption key for AES encryptor.", COLerror::PreCondition );
   }

   // Last block always contains the size of any overflow block, including 0 size.
   COLuint8 PaddedEncryptionBuffer[DRG_AES_SIZE_OF_BLOCK];
   ::memset( PaddedEncryptionBuffer, '\0', DRG_AES_SIZE_OF_BLOCK );
   
   ::memcpy( PaddedEncryptionBuffer + 1, pMember->EncryptionBuffer + 1, pMember->AmountInBuffer );
   PaddedEncryptionBuffer[0] = pMember->AmountInBuffer;
   
   pMember->pEncryptor->ProcessBlock( PaddedEncryptionBuffer, PaddedEncryptionBuffer );
   next().write( PaddedEncryptionBuffer, DRG_AES_SIZE_OF_BLOCK );
   pMember->AmountInBuffer = 0;

   next().flush();
}

void DRGfilterEncrypt::onEndStream()
{
   next().onEndStream(); 
   pMember->KeyWritten = false;
}
