# Microsoft Developer Studio Project File - Name="SFI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SFI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SFI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SFI.mak" CFG="SFI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SFI - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SFI - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SFI - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /vmg /GR /GX /O2 /Zi /Ob2 /I "..\\" /D "NDEBUG" /D "_LIB" /D COL_DEBUG_LEVEL=0 /D "WIN32" /D "COL_DLL_EXPORTS" /D "COL_ASSERTS_ON" /Yu"SFIprecomp.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SFI - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /Gi /vmg /GR /GX /Gf /Ob2 /Zi /I "..\\" /D "_MT" /D "_DLL" /D "COL_ASSERTS_ON" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "COL_DLL_EXPORTS" /Yu"SFIprecomp.h" /FD /GZ /c
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

# Name "SFI - Win32 Release"
# Name "SFI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SFIbase32.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIbase64.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIbase64DecodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIbase64EncodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIcrypt3.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIhexDecodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIhexEncodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIhexValidFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIhtmlCrop.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIhtmlEscape.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIjsonEscape.cpp
# End Source File
# Begin Source File

SOURCE=.\SFImd5.cpp
# End Source File
# Begin Source File

SOURCE=.\SFImd5Imp.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIprecomp.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\SFIsqlEscape.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIunixToUnix.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIuriEscapeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIuriUnescapeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIxmlAttributeDataEncodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIxmlDataEncodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SFIxmlTagEncodeFilter.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SFIbase32.h
# End Source File
# Begin Source File

SOURCE=.\SFIbase64.h
# End Source File
# Begin Source File

SOURCE=.\SFIbase64DecodeFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIbase64EncodeFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIcrypt3.h
# End Source File
# Begin Source File

SOURCE=.\SFIhexDecodeFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIhexEncodeFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIhexValidFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIhtmlCrop.h
# End Source File
# Begin Source File

SOURCE=.\SFIhtmlEscape.h
# End Source File
# Begin Source File

SOURCE=.\SFIjsonEscape.h
# End Source File
# Begin Source File

SOURCE=.\SFImd5.h
# End Source File
# Begin Source File

SOURCE=.\SFImd5Imp.h
# End Source File
# Begin Source File

SOURCE=.\SFIprecomp.h
# End Source File
# Begin Source File

SOURCE=.\SFIsqlEscape.h
# End Source File
# Begin Source File

SOURCE=.\SFIunixToUnix.h
# End Source File
# Begin Source File

SOURCE=.\SFIuriEscapeFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIuriUnescapeFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIxmlAttributeDataEncodeFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIxmlDataEncodeFilter.h
# End Source File
# Begin Source File

SOURCE=.\SFIxmlTagEncodeFilter.h
# End Source File
# End Group
# End Target
# End Project
