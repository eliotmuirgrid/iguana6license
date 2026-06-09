//---------------------------------------------------------------------------
// Copyright (C) 1997-2011 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: DRGserialization
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Tuesday, May 13th, 2008 @ 02:21:53 PM
//
//---------------------------------------------------------------------------
#include "DRGprecomp.h"
#pragma hdrstop

#include "DRGserialization.h"
#include "DRGinformation.h"
#include "DRGfilterDecrypt.h"
#include "DRGfilterEncrypt.h"

#include <COL/COLsimpleBuffer.h>
#include <COL/COLdateTime.h>

#include <SFI/SFIhexDecodeFilter.h>
#include <SFI/SFIhexEncodeFilter.h>
#include <SFI/SFIhexValidFilter.h>
#include <SFI/SFImd5.h>

#include <stdio.h>
#include <stdlib.h>

// ':' and ';' are escaped with '\'. The '\' itself is represented by '\\'.
COLstring DRGescapeString( const COLstring& UnescapedString );
COLstring DRGdeEscapeString( const COLstring& EscapedString );

// Returns a pointer to the next (unescaped) instance of Separator in 
// pBuffer. Returns NULL if not found.
const char* DRGnextSeparator( const char* pBuffer, char Separator );

void DRGwriteLicenseString(COLostream& OutStream, const COLstring& KeyName, const COLstring& Value)
{
   OutStream << KeyName << ':' << DRGescapeString(Value) << ';';
}

COLstring DRGdateToStringTreStyle(const COLdateTime& DateTime)
{
   COLstring Result;
   COLostream ResultStream(Result);

   try
   {
      if (DateTime.isNull())
      {
         ResultStream << "<null>";
      }
      else if (DateTime.status() == COLdateTime::invalid)
      {
         ResultStream << "(invalid)";
      }
      else
      {
         char Buffer[200];
         sprintf(Buffer, "%02i/%02i/%04i %02i:%02i:%02i",          
                 DateTime.day(),
                 DateTime.month(),
                 DateTime.year(),
                 DateTime.hour(),
                 DateTime.minute(),
                 DateTime.second()
                 );
         ResultStream << Buffer;
      }
   }
   catch(COLerror& Error)
   {
      ResultStream << "Invalid datetime (" << Error << ')';
   }

   return Result;
}

void DRGwriteLicenseDateTime(COLostream& OutStream, const COLstring& KeyName, const COLdateTime& Value)
{
   OutStream << KeyName << ':' << DRGescapeString(DRGdateToStringTreStyle(Value)) << ';';
}

void DRGwriteLicenseInt32(COLostream& OutStream, const COLstring& KeyName, COLint32 Value)
{
   OutStream << KeyName << ':' << Value << ';';
}

void DRGlicenseToString
( 
   const DRGinformation& InputLicense,
   COLstring& OutputString, 
   const COLuint8* pEncryptionKey, 
   COLuint8 SizeOfEncryptionKey,
   const char* pProductName,
   COLuint32 SizeOfProductName,
   const char* pLicenseVersion,
   COLuint32 SizeOfLicenseVersion
)
{  
   COLstring SerializedTreeString;
   COLostream SerializedTreeStream( SerializedTreeString );
   
   //Write Member:Value; straight into the COLostream, rather than using TREe
   DRGwriteLicenseInt32(SerializedTreeStream, "MaxCountOfConnection", InputLicense.maxCountOfConnection());
   DRGwriteLicenseDateTime(SerializedTreeStream,"LicenseTimeStamp",InputLicense.licenseTimeStamp());
   DRGwriteLicenseInt32(SerializedTreeStream, "CountOfMonthToLicenseExpiry", InputLicense.countOfMonthToLicenseExpiry());
   DRGwriteLicenseInt32(SerializedTreeStream, "CountOfMonthToMaintenanceExpiry", InputLicense.countOfMonthToMaintenanceExpiry());
   DRGwriteLicenseInt32(SerializedTreeStream, "UnlimitedLlpConnections", (InputLicense.unlimitedLlpConnections() ? 1 : 0));

   //TREe Serialization no longer needed
   //Instead a brute force method is used. If there is ever a need to add a member:value pair
   //to the license key... You should add it here

   // Encrypt the serialized license instance, and convert to HEX so that it may be easily copied/pasted.
   COLsimpleBuffer EncryptedLicenseSink;
   SFIhexEncodeFilter ToHexFilter( &EncryptedLicenseSink );
   DRGfilterEncrypt EncryptFilter( &ToHexFilter );
   EncryptFilter.setEncryptionKey( pEncryptionKey, SizeOfEncryptionKey );   
   EncryptFilter.write(SerializedTreeString.c_str(), (COLuint32)SerializedTreeString.size());
   EncryptFilter.flush();
   
   // hash of the product name to ensure that this license is used only for the specified product.
   COLstring ProductNameHash = SFImd5((void*)pProductName, SizeOfProductName);  
   // hash of the license version for the version check during decryption.
   COLstring LicenseVersionHash = SFImd5((void*)pLicenseVersion, SizeOfLicenseVersion);
  
   // hash of the entire license to detect possible data corruption.
   COLstring LicenseData = ProductNameHash + LicenseVersionHash;
   LicenseData.write(EncryptedLicenseSink.data(), EncryptedLicenseSink.size());
   COLstring LicenseDataHash = SFImd5(LicenseData);
   
   OutputString = LicenseDataHash + ProductNameHash + LicenseVersionHash;
   OutputString.append((const char*) EncryptedLicenseSink.data(),EncryptedLicenseSink.size());
}

COLint32 DRGint32FromString(const COLstring& String)
{
   COLint32 Number = atoi(String.c_str());  
   return Number;
}

void DRGdateTimeFromString(const COLstring& String, COLdateTime& DateTime)
{
   if (String == "<null>")
   {
      DateTime.setNull();
   }
   else if (String == "(invalid)")
   {
      DateTime.setStatus(COLdateTime::invalid);
   }
   else
   {      
      // TODO - This functionality has little error checking against 
      // badly formed input strings. - Ron
      COLint32 CountOfYear   = 0;
      COLint32 CountOfMonth  = 0;
      COLint32 CountOfDay    = 0;
      COLint32 CountOfHour   = 0;
      COLint32 CountOfMinute = 0;
      COLint32 CountOfSecond = 0;

      sscanf(String.c_str(), "%02d/%02d/%04d %02d:%02d:%02d",          
            &CountOfDay,
            &CountOfMonth,
            &CountOfYear, 
            &CountOfHour,
            &CountOfMinute,
            &CountOfSecond);

      DateTime.setDateTime(CountOfYear, CountOfMonth, CountOfDay,
                            CountOfHour, CountOfMinute, CountOfSecond);
   }  
}

//This helper function is used to replace the need of TREe serialization used in 
//the previous implementation. This, however, is pretty brute force and depends on
//the fact that we know what the member:value pairs are. If ever we need to add another
//member:value pair, we must add another else if statement that checks for the MemberName
//explicitly and handles the Value properly.
void DRGpopulateOutputLicense(DRGinformation& OutputLicense, COLstring MemberName, COLstring ValueString)
{
   if (MemberName == "MaxCountOfConnection")
   {  
      COLint32 MaxCountOfConnection = DRGint32FromString(ValueString);
      OutputLicense.setMaxCountOfConnection(MaxCountOfConnection);
   }
   else if (MemberName == "LicenseTimeStamp")
   {
      COLdateTime LicenseTimeStamp;
      DRGdateTimeFromString(ValueString,LicenseTimeStamp);
      OutputLicense.setLicenseTimeStamp(LicenseTimeStamp);
   }
   else if (MemberName == "CountOfMonthToLicenseExpiry")
   {
      //cast ValueString to int32
      COLint32 CountOfMonthToLicenseExpiry = DRGint32FromString(ValueString);
      OutputLicense.setCountOfMonthToLicenseExpiry(CountOfMonthToLicenseExpiry);
   }
   else if (MemberName == "CountOfMonthToMaintenanceExpiry")
   {
      //cast ValueString to int32
      COLint32 CountOfMonthToMaintenanceExpiry = DRGint32FromString(ValueString);
      OutputLicense.setCountOfMonthToMaintenanceExpiry(CountOfMonthToMaintenanceExpiry);
   }
   else if (MemberName == "UnlimitedLlpConnections")
   {
      COLint32 Val = DRGint32FromString(ValueString);
      OutputLicense.setUnlimitedLlpConnections(Val == 0 ? false : true);
   }
}

void DRGlicenseFromString
( 
   const COLstring& InputString,
   DRGinformation& OutputLicense,
   const COLuint8* pDecryptionKey, 
   COLuint8 SizeOfDecryptionKey,
   const char* pProductName,
   COLuint32 SizeOfProductName,
   const char* pLicenseVersion,
   COLuint32 SizeOfLicenseVersion
)
{
   
   // Store entire license file data into a buffer and filtering out
   // invalid HEX characters such as whitespace.

   COLstring InputStringCopy = InputString;

   COLsimpleBuffer CleanedBuffer;
   SFIhexValidFilter ValidHexFilter( &CleanedBuffer );
   ValidHexFilter.write(InputString.c_str(), (COLuint32)InputString.size());

   const COLuint32 SizeOfHashBlockBinary = DRG_AES_SIZE_OF_BLOCK;
   const COLuint32 SizeOfHashBlockHex = 2 * SizeOfHashBlockBinary; // 2 HEX characters per binary byte.
   const COLuint32 CountOfHashBlock = 3; // 1 block for entire license data hash, 1 block for product name hash, 
                                         // and 1 block for license version hash. Encryption/decryption key hash 
                                         // block is embedded in the encrypted license.

   if ( CleanedBuffer.size() < (CountOfHashBlock * SizeOfHashBlockHex) )
   {
      COL_ERROR_STREAM_PLAIN( "License string is too short.", COLerror::PreCondition );
   }
 
   // Converting both hash blocks back to binary in order to compare hash values
   // before decryption.
   COLsimpleBuffer CombinedHashSink;
   SFIhexDecodeFilter CombinedHashFromHexFilter( &CombinedHashSink );
   CleanedBuffer.setSourceEndPosition(CountOfHashBlock * SizeOfHashBlockHex);
   CleanedBuffer.setNext(&CombinedHashFromHexFilter);
   CleanedBuffer.writeAll();

   // First block is the hash of the entire license data.
   SFImd5Hasher LicenseDataHasher;
   LicenseDataHasher.write( CleanedBuffer.data() + SizeOfHashBlockHex, CleanedBuffer.size() - SizeOfHashBlockHex );
   if ( ::memcmp( LicenseDataHasher.hash().data(), CombinedHashSink.data(), SizeOfHashBlockBinary ) != 0 )
   {
      COL_ERROR_STREAM_PLAIN( "Your " << pProductName << " license is corrupted." << newline
                               << "Contact the iNTERFACEWARE Sales Office to obtain a new license." << newline,
                               COLerror::PreCondition );
   }

   // Second block is the hash of the product name.
   SFImd5Hasher ProductNameHasher;
   ProductNameHasher.write( pProductName, SizeOfProductName );
   if ( ::memcmp(ProductNameHasher.hash().data(), 
                 CombinedHashSink.data() + SizeOfHashBlockBinary, 
                 SizeOfHashBlockBinary) != 0 )
   {
      COL_ERROR_STREAM_PLAIN( "This license is not supported by " << pProductName << '.' << newline
                               << "Contact the iNTERFACEWARE Sales Office to obtain a license for " << pProductName << '.' << newline,
                               COLerror::PreCondition );
   }

   // Third block is the hash of the license version.
   SFImd5Hasher LicenseVersionHasher;
   LicenseVersionHasher.write( pLicenseVersion, SizeOfLicenseVersion );
   if ( ::memcmp(LicenseVersionHasher.hash().data(), 
                 CombinedHashSink.data() + SizeOfHashBlockBinary + SizeOfHashBlockBinary, 
                 SizeOfHashBlockBinary) != 0 )
   {
      COL_ERROR_STREAM_PLAIN( "The version of your " << pProductName << " license is no longer supported." << newline
                               << "Contact the iNTERFACEWARE Sales Office to obtain a new license version." << newline,
                               COLerror::PreCondition );
   }

   // Decrypt the actual license data into the serialized license string format
   // used during encryption.
   COLsimpleBuffer SerializedLicenseSink;
   DRGfilterDecrypt DecryptFilter( &SerializedLicenseSink );
   DecryptFilter.setDecryptionKey( pDecryptionKey, SizeOfDecryptionKey );
   SFIhexDecodeFilter FromHexFilter( &DecryptFilter );
   CleanedBuffer.setSourcePosition(CountOfHashBlock * SizeOfHashBlockHex);
   CleanedBuffer.setSourceEndPosition(0);
   CleanedBuffer.setNext(&FromHexFilter);

   try
   {
      CleanedBuffer.writeAll();
   }
   catch(const COLerror& Error)
   {
      if (Error.code() == DRG_KEY_MISMATCH)
      {
         //throw the correct message, this means Iguana ID mismatch
         COL_ERROR_STREAM( "The license code was created for another Iguana instance and cannot be used here.", DRG_IGUANA_ID_MISMATCH );
      }
      else
      {
         throw;
      }
   }

   if ( SerializedLicenseSink.size() == 0 )
   {
      COL_ERROR_STREAM_PLAIN( "License string did not contain any actual license data.", COLerror::PreCondition );
   }

   // Fill up license instance from the serialized license string.
   const char* pNextMemberName = (const char*) SerializedLicenseSink.data();
   const char* pNextMemberValue = NULL;

   do
   {
      pNextMemberValue = DRGnextSeparator( pNextMemberName, ':' );
      if ( pNextMemberValue == NULL )
      {
         break;
      }
      pNextMemberValue++;
      COLstring MemberName( pNextMemberName, pNextMemberValue - pNextMemberName - 1 );

      pNextMemberName = DRGnextSeparator( pNextMemberValue, ';' );
      if ( pNextMemberName == NULL )
      {
         break;
      }
      pNextMemberName++;
      COLstring ValueString( pNextMemberValue, pNextMemberName - pNextMemberValue - 1 );
      ValueString = DRGdeEscapeString(ValueString);

      //This function call replaces the need for TREe de-serialization.
      DRGpopulateOutputLicense(OutputLicense, MemberName, ValueString);
 
   } while ( (pNextMemberName - ((const char*)SerializedLicenseSink.data()) + 1) < SerializedLicenseSink.size() );
}


COLstring DRGescapeString( const COLstring& UnescapedString )
{
   COLstring EscapedString;
   for ( COLuint32 CharIndex = 0; CharIndex < UnescapedString.size(); CharIndex++ )
   {
      switch ( UnescapedString[ CharIndex ] )
      {
         case ':':
            EscapedString += "\\:";
            break;
         case ';':
            EscapedString += "\\:";
            break;
         case '\\':
            EscapedString += "\\\\";
            break;
         default:
            EscapedString += UnescapedString[ CharIndex ];
      };
   }

   return EscapedString;
}

COLstring DRGdeEscapeString( const COLstring& EscapedString )
{
   COLstring DeEscapedString;
   for ( COLuint32 CharIndex = 0; CharIndex < EscapedString.size(); CharIndex++ )
   {
      if ( EscapedString[CharIndex] == '\\' )
      {
         CharIndex++;
         if ( CharIndex >= EscapedString.size() )
         {
            break;
         }
      }
      DeEscapedString += EscapedString[CharIndex];
   }

   return DeEscapedString;
}

const char* DRGnextSeparator( const char* pBuffer, char Separator )
{
   COLuint32 BufferIndex = 0;
   while ( true )
   {
      const char* pSeparator = ::strchr( pBuffer + BufferIndex, Separator );
      if ( pSeparator == NULL )
      {
         return NULL;
      }
      else
      {
         if ( pSeparator == pBuffer )
         {
            return pSeparator;
         }

         BufferIndex = pSeparator - pBuffer;
         if ( *(pSeparator - 1) != '\\' )
         {
            return pSeparator;
         }
      }
   }
}

void DRGiguanaLicenseToString(COLstring* pLicenseStringOut, const DRGinformation& License, const COLstring& IguanaId)
{
   DRGlicenseToString
   (
      License,
      *pLicenseStringOut,
      (const COLuint8*) IguanaId.c_str(),
      IguanaId.size(),
      DRGinformation::DBD_LICENSE_PRODUCT_NAME,
      strlen(DRGinformation::DBD_LICENSE_PRODUCT_NAME),
      DRGinformation::DBD_LICENSE_VERSION,
      strlen(DRGinformation::DBD_LICENSE_VERSION)
   );
}

void DRGiguanaLicenseFromString
(
   DRGinformation* pLicenseInformationOut,
   const COLstring& IguanaLicenseCode,
   const COLstring& IguanaId
)
{
   DRGlicenseFromString(IguanaLicenseCode, *pLicenseInformationOut, (const COLuint8*)IguanaId.c_str(), (COLuint8)IguanaId.size(), 
      DRGinformation::DBD_LICENSE_PRODUCT_NAME, (COLuint32)strlen(DRGinformation::DBD_LICENSE_PRODUCT_NAME),
      DRGinformation::DBD_LICENSE_VERSION, (COLuint32)strlen(DRGinformation::DBD_LICENSE_VERSION));
}
