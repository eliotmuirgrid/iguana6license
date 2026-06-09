#ifndef __DRG_INFORMATION_H__
#define __DRG_INFORMATION_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2011 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: DRGinformation
//
// Description:
//
// DRGinformation - license information for Iguana.
//
// Author: Eliot Muir
// Date:   Tuesday, May 13th, 2008 @ 02:21:47 PM
//
//---------------------------------------------------------------------------


class COLdateTime;
class DRGinformationPrivate;

class DRGinformation
{
public:
   static const char* DBD_LICENSE_FILE_NAME;
   static const char* DBD_LICENSE_PRODUCT_NAME;
   static const char* DBD_LICENSE_VERSION;

   DRGinformation();
   virtual ~DRGinformation();
   DRGinformation( const DRGinformation& Orig );
   DRGinformation& operator=( const DRGinformation& Orig );


   virtual void onInitialized();

   // Set to 0 for infinite number of connections.
   void setMaxCountOfConnection( COLuint32 MaxCountOfConnection );
   COLuint32 maxCountOfConnection() const;

   // This is automatically set to the current time in onInitialized().
   void setLicenseTimeStamp( const COLdateTime& LicenseTimeStamp );
   const COLdateTime& licenseTimeStamp() const;

   // Number of months is in reference to the license time stamp.
   // Set to 0 for no license expiry.
   void setCountOfMonthToLicenseExpiry( COLuint32 CountOfMonthToLicenseExpiry );
   COLuint32 countOfMonthToLicenseExpiry() const;
   // Not a member, constructed during runtime.
   COLdateTime licenseExpiryDate() const;

   // Number of months is in reference to the license time stamp.
   // Set to 0 for no maintenance expiry.
   void setCountOfMonthToMaintenanceExpiry( COLuint32 CountOfMonthToMaintenanceExpiry );
   COLuint32 countOfMonthToMaintenanceExpiry() const;
   // Not a member, constructed during runtime.
   COLdateTime maintenanceExpiryDate() const;

   // Determines if llp connections should be unlimited
   bool unlimitedLlpConnections() const;
   void setUnlimitedLlpConnections(bool Var);   

private:
   static const COLuint32 COUNT_OF_DAY_PER_MONTH;

   DRGinformationPrivate* pMember;
};

#endif // end of defensive include
