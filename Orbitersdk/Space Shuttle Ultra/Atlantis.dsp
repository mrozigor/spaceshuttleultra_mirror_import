# Microsoft Developer Studio Project File - Name="Atlantis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Atlantis - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "Atlantis.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "Atlantis.mak" CFG="Atlantis - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "Atlantis - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Atlantis - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
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
# PROP Output_Dir "..\..\Modules"
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib orbiter.lib orbiterSDK.lib DlgCtrl.lib /nologo /stack:0x400000 /subsystem:windows /dll /pdb:none /machine:I386 /out:"..\..\Modules\MG_Atlantis.dll"

!ELSEIF  "$(CFG)" == "Atlantis - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Modules"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /ML /W3 /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib orbiter.lib orbiterSDK.lib DlgCtrl.lib /nologo /stack:0x400000 /subsystem:windows /dll /pdb:none /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"..\..\Modules\MG_Atlantis.dll"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Atlantis - Win32 Release"
# Name "Atlantis - Win32 Debug"
# Begin Group "Meshes"

# PROP Default_Filter "msh"
# Begin Source File

SOURCE=..\..\Meshes\Atlantis\Atlantis.msh
# End Source File
# Begin Source File

SOURCE=..\..\Meshes\Atlantis\AtlantisCockpit.msh
# End Source File
# Begin Source File

SOURCE=..\..\Meshes\Atlantis\AtlantisVC.msh
# End Source File
# Begin Source File

SOURCE=..\..\Meshes\Atlantis\RMS.msh
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter "h;hpp"
# Begin Source File

SOURCE=.\AC_resource.h
# End Source File
# Begin Source File

SOURCE=.\Atlantis.h
# End Source File
# Begin Source File

SOURCE=.\AtlantisSubsystem.h
# End Source File
# Begin Source File

SOURCE=.\BasicPanel.h
# End Source File
# Begin Source File

SOURCE=.\GearOp.h
# End Source File
# Begin Source File

SOURCE=.\GPCVariables.h
# End Source File
# Begin Source File

SOURCE=.\MasterTimingUnit.h
# End Source File
# Begin Source File

SOURCE=.\meshres.h
# End Source File
# Begin Source File

SOURCE=.\meshres_vc.h
# End Source File
# Begin Source File

SOURCE=.\meshres_vc_additions.h
# End Source File
# Begin Source File

SOURCE=.\OMSPod.h
# End Source File
# Begin Source File

SOURCE=.\OMSSubsystem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Orbitersdk\include\Orbitersdk.h
# End Source File
# Begin Source File

SOURCE=.\PanelA4.h
# End Source File
# Begin Source File

SOURCE=.\PanelC2.h
# End Source File
# Begin Source File

SOURCE=.\PanelF7.h
# End Source File
# Begin Source File

SOURCE=.\PanelO3.h
# End Source File
# Begin Source File

SOURCE=.\PanelR2.h
# End Source File
# Begin Source File

SOURCE=.\PlBayOp.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource_RMS.h
# End Source File
# Begin Source File

SOURCE=.\Stopwatch.h
# End Source File
# Begin Source File

SOURCE=.\SubsystemDirector.h
# End Source File
# End Group
# Begin Group "Logs"

# PROP Default_Filter "log"
# Begin Source File

SOURCE=..\..\Orbiter.log
# End Source File
# End Group
# Begin Group "Source"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Atlantis.cpp
# End Source File
# Begin Source File

SOURCE=.\Atlantis.rc
# End Source File
# Begin Source File

SOURCE=.\AtlantisSubsystem.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\GearOp.cpp
# End Source File
# Begin Source File

SOURCE=.\GPC.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterTimingUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\OMSPod.cpp
# End Source File
# Begin Source File

SOURCE=.\OMSSubsystem.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelA4.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelC2.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelC3.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelF7.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelO3.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelR2.cpp
# End Source File
# Begin Source File

SOURCE=.\PlBayOp.cpp
# End Source File
# Begin Source File

SOURCE=.\Stopwatch.cpp
# End Source File
# Begin Source File

SOURCE=.\SubsystemDirector.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Bitmaps\clocknums.bmp
# End Source File
# Begin Source File

SOURCE=.\Bitmaps\digits.bmp
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

SOURCE=.\Bitmaps\tkbk_label.bmp
# End Source File
# End Target
# End Project
