//---------------------------------------------------------------------------
// Copyright (C) 1997-2011 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: DRGinformation
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Tuesday, May 13th, 2008 @ 02:21:47 PM
//
//---------------------------------------------------------------------------
#include "DRGprecomp.h"
#pragma hdrstop

#include "DRGinformation.h"
#include <COL/COLdateTime.h>

const char* DRGinformation::DBD_LICENSE_FILE_NAME = "IguanaLicense";
const char* DRGinformation::DBD_LICENSE_PRODUCT_NAME = "iNTERFACEWARE Iguana";
const char* DRGinformation::DBD_LICENSE_VERSION = "3";
const COLuint32 DRGinformation::COUNT_OF_DAY_PER_MONTH = 31;

class DRGinformationPrivate
{
public:
   DRGinformationPrivate()
      : UnlimitedLlpConnections(false),
        MaxCountOfConnection(0),
        CountOfMonthToLicenseExpiry(0),
        CountOfMonthToMaintenanceExpiry(0)
   {}
   ~DRGinformationPrivate() {}
   
   COLint32 MaxCountOfConnection;
   COLdateTime LicenseTimeStamp;
   COLint32 CountOfMonthToLicenseExpiry;
   COLint32 CountOfMonthToMaintenanceExpiry;
   bool UnlimitedLlpConnections;

   //No longer need TREe dependency
};

DRGinformation::DRGinformation()
{
   
   pMember = new DRGinformationPrivate;
}

DRGinformation::~DRGinformation()
{
   delete pMember;
}

DRGinformation::DRGinformation( const DRGinformation& Orig ){
   pMember = new DRGinformationPrivate(*Orig.pMember);
}

DRGinformation& DRGinformation::operator=( const DRGinformation& Orig ){
   if (this != &Orig){
      *pMember = *Orig.pMember;
   }
   return *this;
}


void DRGinformation::onInitialized()
{
   pMember->LicenseTimeStamp = COLdateTime::currentTime();
}

void DRGinformation::setMaxCountOfConnection( COLuint32 MaxCountOfConnection )
{
   pMember->MaxCountOfConnection = MaxCountOfConnection;
}

COLuint32 DRGinformation::maxCountOfConnection() const
{
   return pMember->MaxCountOfConnection;
}

void DRGinformation::setLicenseTimeStamp( const COLdateTime& LicenseTimeStamp )
{
   pMember->LicenseTimeStamp = LicenseTimeStamp;
}

const COLdateTime& DRGinformation::licenseTimeStamp() const
{
   return pMember->LicenseTimeStamp;
}

void DRGinformation::setCountOfMonthToLicenseExpiry( COLuint32 CountOfMonthToLicenseExpiry )
{
   pMember->CountOfMonthToLicenseExpiry = CountOfMonthToLicenseExpiry;
}

COLdateTime DRGinformation::licenseExpiryDate() const
{
   COLdateTimeSpan LicenseExpiryDateTimeSpan(pMember->CountOfMonthToLicenseExpiry * COUNT_OF_DAY_PER_MONTH);
   return (pMember ->LicenseTimeStamp + LicenseExpiryDateTimeSpan);
}

COLuint32 DRGinformation::countOfMonthToLicenseExpiry() const
{
   return pMember->CountOfMonthToLicenseExpiry;
}

void DRGinformation::setCountOfMonthToMaintenanceExpiry( COLuint32 CountOfMonthToMaintenanceExpiry )
{
   pMember->CountOfMonthToMaintenanceExpiry = CountOfMonthToMaintenanceExpiry;
}

COLuint32 DRGinformation::countOfMonthToMaintenanceExpiry() const
{
   return pMember->CountOfMonthToMaintenanceExpiry;
}

COLdateTime DRGinformation::maintenanceExpiryDate() const
{
   COLdateTimeSpan MaintenanceExpiryDateTimeSpan( pMember->CountOfMonthToMaintenanceExpiry * COUNT_OF_DAY_PER_MONTH );
   return ( pMember->LicenseTimeStamp + MaintenanceExpiryDateTimeSpan );
}

bool DRGinformation::unlimitedLlpConnections() const
{
   return pMember->UnlimitedLlpConnections;
}

void DRGinformation::setUnlimitedLlpConnections(bool Var)
{
   pMember->UnlimitedLlpConnections = Var;
}
