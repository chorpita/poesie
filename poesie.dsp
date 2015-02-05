# Microsoft Developer Studio Project File - Name="poesie" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=poesie - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "poesie.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "poesie.mak" CFG="poesie - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "poesie - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "poesie - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "poesie - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "poesie - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../wnnew/Debug/wnnew.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "poesie - Win32 Release"
# Name "poesie - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\chicken.cpp
# End Source File
# Begin Source File

SOURCE=.\compile.cpp
# End Source File
# Begin Source File

SOURCE=.\deer.cpp
# End Source File
# Begin Source File

SOURCE=.\deer2.cpp
# End Source File
# Begin Source File

SOURCE=.\DictionaryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\generic.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IPADialog.cpp
# End Source File
# Begin Source File

SOURCE=.\IPAPoemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\lexicon.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\phonetics.cpp
# End Source File
# Begin Source File

SOURCE=.\poesie.cpp
# End Source File
# Begin Source File

SOURCE=.\poesie.rc
# End Source File
# Begin Source File

SOURCE=.\poesieDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\poetics.cpp
# End Source File
# Begin Source File

SOURCE=.\POSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\reader.cpp
# End Source File
# Begin Source File

SOURCE=.\semantic.cpp
# End Source File
# Begin Source File

SOURCE=.\SilbeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\syntax.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\wnbsrch.cpp
# End Source File
# Begin Source File

SOURCE=.\wnmorph.cpp
# End Source File
# Begin Source File

SOURCE=.\wnsearch.cpp
# End Source File
# Begin Source File

SOURCE=.\wnwnglob.cpp
# End Source File
# Begin Source File

SOURCE=.\wnwnhelp.cpp
# End Source File
# Begin Source File

SOURCE=.\wnwnrtl.cpp
# End Source File
# Begin Source File

SOURCE=.\wnwnutil.cpp
# End Source File
# Begin Source File

SOURCE=.\WordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WordNetDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\chicken.h
# End Source File
# Begin Source File

SOURCE=.\compile.h
# End Source File
# Begin Source File

SOURCE=.\deer.h
# End Source File
# Begin Source File

SOURCE=.\DictionaryDlg.h
# End Source File
# Begin Source File

SOURCE=.\generic.h
# End Source File
# Begin Source File

SOURCE=.\ImportDlg.h
# End Source File
# Begin Source File

SOURCE=.\IPADialog.h
# End Source File
# Begin Source File

SOURCE=.\IPAPoemDlg.h
# End Source File
# Begin Source File

SOURCE=.\lexicon.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\phonetics.h
# End Source File
# Begin Source File

SOURCE=.\poesie.h
# End Source File
# Begin Source File

SOURCE=.\poesieDlg.h
# End Source File
# Begin Source File

SOURCE=.\poetics.h
# End Source File
# Begin Source File

SOURCE=.\POSDlg.h
# End Source File
# Begin Source File

SOURCE=.\reader.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\semantic.h
# End Source File
# Begin Source File

SOURCE=.\SilbeDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\syntax.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxDlg.h
# End Source File
# Begin Source File

SOURCE=.\wn\wn.h
# End Source File
# Begin Source File

SOURCE=.\WordDlg.h
# End Source File
# Begin Source File

SOURCE=.\WordNetDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\compiler.bmp
# End Source File
# Begin Source File

SOURCE=.\res\igfh.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ipatrans.bmp
# End Source File
# Begin Source File

SOURCE=.\res\poesie.ico
# End Source File
# Begin Source File

SOURCE=.\res\poesie.rc2
# End Source File
# Begin Source File

SOURCE=.\res\wordnet.bmp
# End Source File
# End Group
# End Target
# End Project
