# Microsoft Developer Studio Project File - Name="COL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=COL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "COL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "COL.mak" CFG="COL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "COL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "COL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "COL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
F90=df.exe
LINK32=link.exe
CFG=rc.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /vmg /GR /GX /Zi /O2 /Ob2 /I "..\\" /D "NDEBUG" /D "_LIB" /D COL_DEBUG_LEVEL=0 /D "WIN32" /D "COL_DLL_EXPORTS" /D "COL_ASSERTS_ON" /Yu"COLprecomp.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "COL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
F90=df.exe
LINK32=link.exe
CFG=rc.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /Gi /vmg /GR /GX /Zi /Ob2 /Gf /I "..\\" /D "_MT" /D "_DLL" /D "COL_ASSERTS_ON" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "COL_DLL_EXPORTS" /Yu"COLprecomp.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "COL - Win32 Release"
# Name "COL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\COLatomic.cpp
# End Source File
# Begin Source File

SOURCE=.\COLattachedSink.cpp
# End Source File
# Begin Source File

SOURCE=.\COLbinaryBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\COLcompileTimeStamp.cpp
# End Source File
# Begin Source File

SOURCE=.\COLdateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\COLdebugNewDelete.cpp
# End Source File
# Begin Source File

SOURCE=.\COLdll.cpp
# End Source File
# Begin Source File

SOURCE=.\COLerror.cpp
# End Source File
# Begin Source File

SOURCE=.\COLfifoBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\COLlist.cpp
# End Source File
# Begin Source File

SOURCE=.\COLlog.cpp
# End Source File
# Begin Source File

SOURCE=.\COLhash.cpp
# End Source File
# Begin Source File

SOURCE=.\COLhashmap.cpp
# End Source File
# Begin Source File

SOURCE=.\COLmath.cpp
# End Source File
# Begin Source File

SOURCE=.\COLmemmem.cpp
# End Source File
# Begin Source File

SOURCE=.\COLmutex.cpp
# End Source File
# Begin Source File

SOURCE=.\COLonce.cpp
# End Source File
# Begin Source File

SOURCE=.\COLostream.cpp
# End Source File
# Begin Source File

SOURCE=.\COLprecomp.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\COLreadable.cpp
# End Source File
# Begin Source File

SOURCE=.\COLreference.cpp
# End Source File
# Begin Source File

SOURCE=.\COLrefHashTable.cpp
# End Source File
# Begin Source File

SOURCE=.\COLrefVect.cpp
# End Source File
# Begin Source File

SOURCE=.\COLrefVectFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsimpleBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsink.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsinkCallback.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsinkComposite.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsinkNull.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsinkReading.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsinkSize.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsinkStandard.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsinkWindowsDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsortedList.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsource.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsourceStandardIn.cpp
# End Source File
# Begin Source File

SOURCE=.\COLsplit.cpp
# End Source File
# Begin Source File

SOURCE=.\COLstring.cpp
# End Source File
# Begin Source File

SOURCE=.\COLthreadLocal.cpp
# End Source File
# Begin Source File

SOURCE=.\COLtrackable.cpp
# End Source File
# Begin Source File

SOURCE=.\COLutils.cpp
# End Source File
# Begin Source File

SOURCE=.\COLvector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\COLatomic.h
# End Source File
# Begin Source File

SOURCE=.\COLattachedSink.h
# End Source File
# Begin Source File

SOURCE=.\COLbinaryBuffer.h
# End Source File
# Begin Source File

SOURCE=.\COLcommaList.h
# End Source File
# Begin Source File

SOURCE=.\COLcompileTimeStamp.h
# End Source File
# Begin Source File

SOURCE=.\COLcrtInit.h
# End Source File
# Begin Source File

SOURCE=.\COLdataType.h
# End Source File
# Begin Source File

SOURCE=.\COLdateTime.h
# End Source File
# Begin Source File

SOURCE=.\COLdebugLog.h
# End Source File
# Begin Source File

SOURCE=.\COLdebugNewDelete.h
# End Source File
# Begin Source File

SOURCE=.\COLdll.h
# End Source File
# Begin Source File

SOURCE=.\COLdllExport.h
# End Source File
# Begin Source File

SOURCE=.\COLendian.h
# End Source File
# Begin Source File

SOURCE=.\COLerror.h
# End Source File
# Begin Source File

SOURCE=.\COLfifoBuffer.h
# End Source File
# Begin Source File

SOURCE=.\COLfilter.h
# End Source File
# Begin Source File

SOURCE=.\COLflagSet.h
# End Source File
# Begin Source File

SOURCE=.\COLgenericLocker.h
# End Source File
# Begin Source File

SOURCE=.\COLindexIterator.h
# End Source File
# Begin Source File

SOURCE=.\COLiterator.h
# End Source File
# Begin Source File

SOURCE=.\COLlist.h
# End Source File
# Begin Source File

SOURCE=.\COLlocker.h
# End Source File
# Begin Source File

SOURCE=.\COLlog.h
# End Source File
# Begin Source File

SOURCE=.\COLlogDevice.h
# End Source File
# Begin Source File

SOURCE=.\COLhash.h
# End Source File
# Begin Source File

SOURCE=.\COLhashmap.h
# End Source File
# Begin Source File

SOURCE=.\COLmath.h
# End Source File
# Begin Source File

SOURCE=.\COLmemmem.h
# End Source File
# Begin Source File

SOURCE=.\COLminimumInclude.h
# End Source File
# Begin Source File

SOURCE=.\COLmutex.h
# End Source File
# Begin Source File

SOURCE=.\COLonce.h
# End Source File
# Begin Source File

SOURCE=.\COLostream.h
# End Source File
# Begin Source File

SOURCE=.\COLauto.h
# End Source File
# Begin Source File

SOURCE=.\COLplaceIterator.h
# End Source File
# Begin Source File

SOURCE=.\COLpreCompiled.h
# End Source File
# Begin Source File

SOURCE=.\COLqueue.h
# End Source File
# Begin Source File

SOURCE=.\COLreadable.h
# End Source File
# Begin Source File

SOURCE=.\COLreference.h
# End Source File
# Begin Source File

SOURCE=.\COLref.h
# End Source File
# Begin Source File

SOURCE=.\COLrefHashTable.h
# End Source File
# Begin Source File

SOURCE=.\COLrefVect.h
# End Source File
# Begin Source File

SOURCE=.\COLsimpleBuffer.h
# End Source File
# Begin Source File

SOURCE=.\COLsink.h
# End Source File
# Begin Source File

SOURCE=.\COLsinkCallback.h
# End Source File
# Begin Source File

SOURCE=.\COLsinkComposite.h
# End Source File
# Begin Source File

SOURCE=.\COLsinkNull.h
# End Source File
# Begin Source File

SOURCE=.\COLsinkReading.h
# End Source File
# Begin Source File

SOURCE=.\COLsinkSize.h
# End Source File
# Begin Source File

SOURCE=.\COLsinkStandard.h
# End Source File
# Begin Source File

SOURCE=.\COLsinkString.h
# End Source File
# Begin Source File

SOURCE=.\COLsinkWindowsDebug.h
# End Source File
# Begin Source File

SOURCE=.\COLsortedList.h
# End Source File
# Begin Source File

SOURCE=.\COLsource.h
# End Source File
# Begin Source File

SOURCE=.\COLsourceBinary.h
# End Source File
# Begin Source File

SOURCE=.\COLsourceStandardIn.h
# End Source File
# Begin Source File

SOURCE=.\COLsplit.h
# End Source File
# Begin Source File

SOURCE=.\COLstring.h
# End Source File
# Begin Source File

SOURCE=.\COLthreadLocal.h
# End Source File
# Begin Source File

SOURCE=.\COLtrackable.h
# End Source File
# Begin Source File

SOURCE=.\COLtracker.h
# End Source File
# Begin Source File

SOURCE=.\COLunicode.h
# End Source File
# Begin Source File

SOURCE=.\COLutils.h
# End Source File
# Begin Source File

SOURCE=.\COLvector.h
# End Source File
# End Group
# End Target
# End Project
