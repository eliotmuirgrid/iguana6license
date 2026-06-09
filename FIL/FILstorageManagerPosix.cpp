//---------------------------------------------------------------------------
// Copyright (C) 1997-2010 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILstorageManager
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Fri 01/16/2004 
//
//---------------------------------------------------------------------------

#include <FIL/FILprecomp.h>
#pragma hdrstop

#include <FIL/FILstorageManager.h>
#include <COL/COLsimpleBuffer.h>
#include <COL/COLerror.h>
#include <COL/COLvector.h>
#include <COL/COLlog.h>
#include <stdio.h>
#ifdef CHM_BSD 
   #include <fstab.h>
#endif
#ifdef CHM_LINUX
   #include <mntent.h>
#endif
#ifdef CHM_SOLARIS
   #include <string.h>
   #include <errno.h>
   #include <sys/mnttab.h>
#endif

COL_LOG_MODULE;

class FILstorageManagerPrivate
{
public:
   COLvector<COLstring> DeviceVector; 
};

FILstorageManager::FILstorageManager()
{
   pMember = new FILstorageManagerPrivate;
   try
   {
      setup();
   }
   catch(...)
   {
      delete pMember;
      throw;
   }
}

void FILstorageManager::setup()
{
   COL_METHOD(FILstorageManager::setup);
#ifdef CHM_BSD
   if (!setfsent())
   {
      COL_ERROR_STREAM_PLAIN("setfsent failed to open /etc/fstab", COLerror::PreCondition);
   }  

   struct fstab* pFileSystemEntry = NULL;
   while ((pFileSystemEntry=getfsent()) != NULL)
   {
      pMember->DeviceVector.push_back(pFileSystemEntry->fs_file);
   }
   endfsent();
#endif

#ifdef CHM_LINUX
   FILE* FileHandle = setmntent("/etc/mtab", "r");
   if (NULL == FileHandle)
   {
      COL_ERROR_STREAM_PLAIN("setmntent failed to open /etc/mtab", COLerror::PreCondition);
   }

   struct mntent* pMountEntry = getmntent(FileHandle);

   while(pMountEntry != NULL)
   {
      COL_TRC(  "mnt_fsname = " << pMountEntry->mnt_fsname     /* name of mounted file system */
            << " mnt_dir = " << pMountEntry->mnt_dir           /* file system path prefix */
            << " mnt_type = " << pMountEntry->mnt_type         /* mount type (see mntent.h) */
            << " mnt_opts = " << pMountEntry->mnt_opts         /* mount options (see mntent.h) */
            << " mnt_freq = " << pMountEntry->mnt_freq         /* dump frequency in days */
            << " mnt_passno = " << pMountEntry->mnt_passno);   /* pass number on parallel fsck */

      if ( (strcmp(pMountEntry->mnt_type, "debugfs") != 0) && (strcmp(pMountEntry->mnt_type, "securityfs") != 0) )
      {
         pMember->DeviceVector.push_back(pMountEntry->mnt_dir);
      }

      pMountEntry = getmntent(FileHandle);
   }
   
   // no error codes on endmntent - whoopee!
   endmntent(FileHandle);
#endif

#ifdef CHM_SOLARIS
   FILE* FileHandle = NULL;
   FileHandle = fopen("/etc/mnttab","r");
   if (NULL == FileHandle)
   {
      COL_THROW_STRERROR("fopen failed to open /etc/mtab.");
   }
   
   try
   {
      struct mnttab MountEntry; 
      while (true)
      {
         int ReturnCode = getmntent(FileHandle, &MountEntry);
         
         if (ReturnCode < 0)
         {
            break;
         }
         
         if (ReturnCode > 0)
         {
            COL_ERROR_STREAM_PLAIN("getmntent failed",COLerror::PostCondition);
            break;
         }
         
         pMember->DeviceVector.push_back(MountEntry.mnt_mountp);
      }
      ::fclose(FileHandle);
   }
   catch(const COLerror&)
   {
      ::fclose(FileHandle);
      throw;
   }
#endif
}
 
FILstorageManager::~FILstorageManager()
{
   delete pMember;
}

COLuint32 FILstorageManager::countOfDevice() const
{
   return pMember->DeviceVector.size();   
}

const COLstring& FILstorageManager::deviceName(COLuint32 DeviceIndex) const
{
   return pMember->DeviceVector[DeviceIndex];  
}
