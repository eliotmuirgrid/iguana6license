//---------------------------------------------------------------------------
//
// Copyright (C) 1997-2011 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: IguanaLicense
//
// Description:
//
// A command-line utility to generate license keys for Iguana.
//
// Author: Henry Tran
// Date: July 27, 2004 
//---------------------------------------------------------------------------
#include "Precomp.h"
#pragma hdrstop

#include <stdio.h>
#include <stdlib.h>

#include <FIL/FILbinaryFile.h>
#include <FIL/FILutils.h>

#include <COL/COLdateTime.h>
#include <COL/COLerror.h>

#include <CMD/CMDlineParser.h>

#include <DRG/DRGiguanaId.h>
#include <DRG/DRGinformation.h>
#include <DRG/DRGserialization.h>

static const COLuint32 BUFFER_SIZE = 1024;

static void loadLicense(const COLstring& FileName, const COLstring& IguanaId, DRGinformation& RegistrationInformation){
   COLstring InputString;
   FILreadFileIntoString(FileName, &InputString);

   DRGlicenseFromString(InputString, 
      RegistrationInformation,
      (const COLuint8*) IguanaId.c_str(), 
      IguanaId.size(),
      DRGinformation::DBD_LICENSE_PRODUCT_NAME,
      strlen(DRGinformation::DBD_LICENSE_PRODUCT_NAME),
      DRGinformation::DBD_LICENSE_VERSION,
      strlen(DRGinformation::DBD_LICENSE_VERSION) );

}

static COLstring readLine(){
   char InputBuffer[BUFFER_SIZE];
   return ::fgets(InputBuffer,sizeof(InputBuffer),stdin);
}

static void regSetInt
(
   const char* pHelp, 
   COLuint32 (DRGinformation::*getPtr)() const,
   void (DRGinformation::*setPtr)( COLuint32 ), 
   DRGinformation& RegistrationInformation
){
   COLint32 Current = (RegistrationInformation.*getPtr)();
   COLcout << pHelp << " (0 for infinite) [" << Current << "]: ";
   COLstring Str = readLine();
   if (!Str.isWhitespace()){
        (RegistrationInformation.*setPtr)( ::atoi(Str.c_str()) );
   }
}

static void printLicense(const DRGinformation& RegistrationInformation){
   COLcout << "Count of Connection: " << RegistrationInformation.maxCountOfConnection() << newline 
           << "License Time Stamp : " << RegistrationInformation.licenseTimeStamp()  << newline
           << "License Expiry     : " << RegistrationInformation.licenseExpiryDate() << " (" << RegistrationInformation.countOfMonthToLicenseExpiry() << " months)" << newline
           << "Maintenance Expiry : " << RegistrationInformation.maintenanceExpiryDate() << " (" << RegistrationInformation.countOfMonthToMaintenanceExpiry() << " months)" << newline
           << "Unlimited Llp Conns: " << RegistrationInformation.unlimitedLlpConnections() << newline;
}

void DRGoutput(const COLstring& LicenseString){
   //print it in col width to make life easier for
   //cut and paste
   const char* pStart = LicenseString.c_str();
   const char* pEnd = pStart + LicenseString.size();
   const COLuint32 Width = 40;

   while (pStart < pEnd) {
      COLuint32 WriteCount = COL_MIN(pEnd - pStart, Width);
      COLcout.sink()->write(pStart, WriteCount);
      COLcout << newline;
      pStart += WriteCount;
   }
}

int main( int argc, const char** argv )
{
   try
   {
         DRGinformation RegistrationInformation;
	 RegistrationInformation.onInitialized();

        // COLcout << argc << "args." << newline;

	 if (argc < 2) {
            COLcout << "IguanaLicense <id> <version>" << newline;
	    return 0;
         }
         const char* IguanaVersion = DRGinformation::DBD_LICENSE_VERSION;
         if (argc == 3) {
            COLcout << "Activate version control." << newline;
            IguanaVersion = argv[2];
         }
         COLcout << "Version:" << IguanaVersion << newline;
        

	 COLstring IguanaId = argv[1];
	 RegistrationInformation.setMaxCountOfConnection(0);
	 RegistrationInformation.setCountOfMonthToLicenseExpiry(0);
	 RegistrationInformation.setCountOfMonthToMaintenanceExpiry(0);
         RegistrationInformation.setUnlimitedLlpConnections(true);

         COLstring LicenseString;
         DRGlicenseToString( RegistrationInformation, 
                             LicenseString, 
                             (const COLuint8*) IguanaId.c_str(), 
                             IguanaId.size(),
                             DRGinformation::DBD_LICENSE_PRODUCT_NAME,
                             strlen(DRGinformation::DBD_LICENSE_PRODUCT_NAME),
                             IguanaVersion, 
                             strlen(DRGinformation::DBD_LICENSE_VERSION) );

         //COLcout << newline << "-----" << newline;
         //printLicense(RegistrationInformation);
         //COLcout << "-----" << newline << newline;
	 DRGoutput(LicenseString);
      return EXIT_SUCCESS;
   }
   catch ( COLerror& Error )
   {
      COLcerr << Error.description() << newline;
      return EXIT_FAILURE;
   }
   catch ( ... )
   {
      COLcerr << "Unhandled exception." << newline;
      return EXIT_FAILURE;
   }
}
