//---------------------------------------------------------------------------
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: DRGfilterDecrypt
//
// Description:
//
// Implementation.
//
// Author: Henry Tran
// Date: July 6, 2004
//
//---------------------------------------------------------------------------
#include "DRGprecomp.h"
#pragma hdrstop

#include "DRGfilterDecrypt.h"

#include <SFI/SFIaesDecryptor.h>
#include <SFI/SFImd5.h>
#include <COL/COLsimpleBuffer.h>

class DRGfilterDecryptPrivate
{
public:
   DRGfilterDecryptPrivate()
      : pDecryptor( NULL ),
        AmountInBuffer( 0 ),
        SizeOfDecryptionKey( 0 ),
        KeyVerified( false ) {}
   ~DRGfilterDecryptPrivate();

   SFIaesDecryptor* pDecryptor;

   COLuint8 DecryptionBuffer[ DRG_AES_SIZE_OF_BLOCK ];
   COLuint8 AmountInBuffer;

   COLuint8 DecryptionKey[ DRG_AES_SIZE_OF_KEY ];
   COLuint8 SizeOfDecryptionKey;
   bool KeyVerified;
};

DRGfilterDecryptPrivate::~DRGfilterDecryptPrivate()
{
   if ( pDecryptor != NULL )
   {
      delete pDecryptor;
   }
}

DRGfilterDecrypt::DRGfilterDecrypt( COLsink* pNext_ )
   : COLfilter( pNext_ )
{
   pMember = new DRGfilterDecryptPrivate();
}

DRGfilterDecrypt::~DRGfilterDecrypt()
{
   delete pMember;
}

void DRGfilterDecrypt::setDecryptionKey( const COLuint8* pDecryptionKey, COLuint8 SizeOfDecryptionKey )
{
   if ( SizeOfDecryptionKey > DRG_AES_SIZE_OF_KEY )
   {
      COL_ERROR_STREAM_PLAIN( "Decryption key is larger than maximum size of "
                              << DRG_AES_SIZE_OF_KEY << " bytes.", DRG_KEY_TOO_LARGE );
   }

   ::memset( pMember->DecryptionKey, '\0', DRG_AES_SIZE_OF_KEY );
   ::memcpy( pMember->DecryptionKey, pDecryptionKey, SizeOfDecryptionKey );

   pMember->SizeOfDecryptionKey = SizeOfDecryptionKey;

   if ( pMember->pDecryptor != NULL )
   {
      delete pMember->pDecryptor;
   }

   pMember->pDecryptor = new SFIaesDecryptor( pMember->DecryptionKey );
}

const COLuint8* DRGfilterDecrypt::decryptionKey() const
{
   COLPRECONDITION( pMember->pDecryptor != NULL )
   return pMember->DecryptionKey;
}

COLuint8 DRGfilterDecrypt::sizeOfDecryptionKey() const
{
   COLPRECONDITION( pMember->pDecryptor != NULL )
   return pMember->SizeOfDecryptionKey;
}

COLuint32 DRGfilterDecrypt::write( const void* pBuffer, COLuint32 SizeOfBuffer )
{
   const COLuint32 OriginalSizeOfBuffer = SizeOfBuffer;

   if ( pMember->pDecryptor == NULL )
   {
      COL_ERROR_STREAM( "Need to set decryption key for AES decryptor.", DRG_KEY_NO_DECRYPTION_KEY );
   }

   COLuint8 AmountToWrite;
   while ( SizeOfBuffer > 0 )
   {
      if ( SizeOfBuffer > (DRG_AES_SIZE_OF_BLOCK - pMember->AmountInBuffer) )
      {
         AmountToWrite = DRG_AES_SIZE_OF_BLOCK - pMember->AmountInBuffer;
      }
      else
      {
         AmountToWrite = SizeOfBuffer;
      }

      ::memcpy( pMember->DecryptionBuffer + pMember->AmountInBuffer, pBuffer, AmountToWrite );
      pMember->AmountInBuffer += AmountToWrite;

      if( pMember->AmountInBuffer == DRG_AES_SIZE_OF_BLOCK )
      {
         if ( !pMember->KeyVerified )
         {
            SFImd5Hasher Hasher;
            Hasher.write( pMember->DecryptionKey, DRG_AES_SIZE_OF_KEY );

            if ( ::memcmp( Hasher.hash().data(), pMember->DecryptionBuffer, DRG_AES_SIZE_OF_BLOCK ) != 0 )
            {
               // #8844
               // This is changed to reflect something more user friendly. 
               // because this is the message that gets bubbled up to users
               // of Iguana with a license key with different Machine ID
               COL_ERROR_STREAM( "Encryption key does not match decryption key.", DRG_KEY_MISMATCH);
            }

            pMember->KeyVerified = true;
         }
         else
         {
            pMember->pDecryptor->ProcessBlock( pMember->DecryptionBuffer, pMember->DecryptionBuffer );
            COLuint8 SizeOfBlock = pMember->DecryptionBuffer[0];
            next().write( pMember->DecryptionBuffer + 1, SizeOfBlock );
         }
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

void DRGfilterDecrypt::onEndStream()
{
   next().onEndStream(); 
   pMember->KeyVerified = false;
};
