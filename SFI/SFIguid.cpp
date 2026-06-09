//---------------------------------------------------------------------------
// Copyright (C) 1997-2013 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: SFIguid
//
// Description:
//
// Implementation
//
// Author: Nasron Cheong
// Date:   Thursday, December 20th, 2007 @ 11:25:16 AM
//
//---------------------------------------------------------------------------
#include "SFIprecomp.h"
#pragma hdrstop

#include "SFIguid.h"
#include "SFIhexEncodeFilter.h"
#include "SFIrandom.h"

#include <COL/COLvector.h>
#include <COL/COLsinkString.h>

#include <sys/timeb.h>

#ifdef WIN32
   #define SFIftime _ftime
   typedef _timeb SFItimeb;
#else
   #define SFIftime ftime
   typedef timeb SFItimeb;
#endif

COLstring SFIgenerateGUID( long BitStrength ) 
{
   long GuidLength = BitStrength/8;

   SFItimeb NowTime;
   SFIftime(&NowTime);

   COLvector<COLuint8> GuidData;
   //append time and ms
   GuidData.resize(GuidLength);

   //at most half of the generated data can be the time
   //half of 128 is 64, which is 8 bytes
   COLuint32 TimeSize = 0;
   if ((sizeof(NowTime.millitm) + sizeof(NowTime.time)) <= 8)
   {
      TimeSize = (sizeof(NowTime.millitm) + sizeof(NowTime.time));
      ::memcpy(GuidData.getArray(),&NowTime.time,sizeof(NowTime.time));
      ::memcpy(GuidData.getArray()+sizeof(NowTime.time),&NowTime.millitm,sizeof(NowTime.millitm));
   }
   else
   {         
      //this branch is usually for 64 bit systems.
      COLuint32 TruncatedTime = (COLuint32)NowTime.time;
      COLuint16 TruncatedMs = (COLuint16)NowTime.millitm;
      TimeSize = (sizeof(TruncatedTime) + sizeof(TruncatedMs));
      ::memcpy(GuidData.getArray(),&TruncatedTime,sizeof(TruncatedTime));
      ::memcpy(GuidData.getArray()+sizeof(TruncatedTime),&TruncatedMs,sizeof(TruncatedMs));
   }
   COLPRECONDITION(TimeSize > 0)

   long AmountToGenerate = GuidLength -  TimeSize;
   COLPRECONDITION(AmountToGenerate > 0)

   SFIrandom Rng;
   if (!Rng.isStrongRng())
   {
      COLstring ErrorString;
      COLostream ErrorStream(ErrorString);
      ErrorStream << "This platform does not support strong random number generation.";
#ifndef WIN32
      ErrorStream << " On Unix, this is required through the device '/dev/urandom'. Check your Unix documentation on how to add support for strong random number generation.";                        
#endif
      throw COLerror(ErrorString,COLerror::SystemError);
   }
   Rng.seed(GuidData.getArray(),TimeSize);
   Rng.generate( GuidData.getArray()+TimeSize, AmountToGenerate);

   //now hex encode the result
   COLstring ResultString;
   COLsinkString ResultSink(ResultString);
   SFIhexEncodeFilter HexEncoder(&ResultSink);
   HexEncoder.write(GuidData.getArray(),GuidData.size());
   HexEncoder.onEndStream();
   return ResultString;
}
