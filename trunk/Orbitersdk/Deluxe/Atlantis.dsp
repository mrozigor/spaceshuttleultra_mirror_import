# Microsoft Developer Studio Project File - Name="Atlantis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Atlantis - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Atlantis.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Atlantis.mak" CFG="Atlantis - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Atlantis - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Atlantis - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Atlantis - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Modules"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib /nologo /stack:0x400000 /subsystem:windows /dll /pdb:none /machine:I386 /out:"..\..\..\Modules\MG_Atlantis.dll"

!ELSEIF  "$(CFG)" == "Atlantis - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Modules"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /ML /W3 /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0x400000 /subsystem:windows /dll /pdb:none /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"..\..\..\Modules\MG_Atlantis.dll"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Atlantis - Win32 Release"
# Name "Atlantis - Win32 Debug"
# Begin Group "Meshes"

# PROP Default_Filter "msh"
# Begin Source File

SOURCE=..\..\..\Meshes\Atlantis\Atlantis.msh

!IF  "$(CFG)" == "Atlantis - Win32 Release"

# Begin Custom Build - Performing Custom Build Step on $(InputPath)
ProjDir=.
InputPath=..\..\..\Meshes\Atlantis\Atlantis.msh

"meshres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo $(InputPath) - meshres.h | $(ProjDir)\..\..\utils\meshc.exe

# End Custom Build

!ELSEIF  "$(CFG)" == "Atlantis - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Meshes\Atlantis\AtlantisVC.msh

!IF  "$(CFG)" == "Atlantis - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=..\..\..\Meshes\Atlantis\AtlantisVC.msh

"meshres_vc.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo $(InputPath) _VC meshres_vc.h | $(ProjDir)\..\..\utils\meshc.exe

# End Custom Build

!ELSEIF  "$(CFG)" == "Atlantis - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\AC_resource.h
# End Source File
# Begin Source File

SOURCE=.\Atlantis.cpp
# End Source File
# Begin Source File

SOURCE=.\Atlantis.h
# End Source File
# Begin Source File

SOURCE=.\Atlantis.rc
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\down.ico
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\meshres.h
# End Source File
# Begin Source File

SOURCE=.\meshres_vc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Orbitersdk\include\Orbitersdk.h
# End Source File
# Begin Source File

SOURCE=.\PlBayOp.cpp
# End Source File
# Begin Source File

SOURCE=.\PlBayOp.h
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\tkbk_label.bmp
# End Source File
# Begin Source File

SOURCE=..\..\..\Orbitersdk\lib\Orbiter.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\Orbitersdk\lib\Orbitersdk.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\DlgCtrl.lib
# End Source File
# End Target
# End Project
