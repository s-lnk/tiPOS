# Microsoft Developer Studio Project File - Name="apptest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=apptest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "apptest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "apptest.mak" CFG="apptest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "apptest - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "apptest - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "apptest - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f makefile"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "makefile.exe"
# PROP BASE Bsc_Name "makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f makefile"
# PROP Rebuild_Opt "/a"
# PROP Target_File "apptest.exe"
# PROP Bsc_Name "apptest.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "apptest - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f makefile"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "makefile.exe"
# PROP BASE Bsc_Name "makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f makefile"
# PROP Rebuild_Opt "/a"
# PROP Target_File "apptest.exe"
# PROP Bsc_Name "apptest.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "apptest - Win32 Release"
# Name "apptest - Win32 Debug"

!IF  "$(CFG)" == "apptest - Win32 Release"

!ELSEIF  "$(CFG)" == "apptest - Win32 Debug"

!ENDIF 

# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\inc\josapi.h
# End Source File
# Begin Source File

SOURCE=..\inc\wlsapi.h
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\apptest.c
# End Source File
# Begin Source File

SOURCE=.\src\batlogo.h
# End Source File
# Begin Source File

SOURCE=.\src\comtest.c
# End Source File
# Begin Source File

SOURCE=.\src\init.s
# End Source File
# Begin Source File

SOURCE=.\src\kbtest.c
# End Source File
# Begin Source File

SOURCE=.\ldscript
# End Source File
# Begin Source File

SOURCE=.\src\lnkadmin.c
# End Source File
# Begin Source File

SOURCE=.\src\lnkpos.c
# End Source File
# Begin Source File

SOURCE=.\src\lnkprint.c
# End Source File
# Begin Source File

SOURCE=.\src\lnkshowinfo.c
# End Source File
# Begin Source File

SOURCE=.\src\main.c
# End Source File
# Begin Source File

SOURCE=.\src\prnapi.c
# End Source File
# Begin Source File

SOURCE=.\src\public.c
# End Source File
# Begin Source File

SOURCE=.\src\public.h
# End Source File
# Begin Source File

SOURCE=.\src\systest.c
# End Source File
# Begin Source File

SOURCE=.\src\wlstest.c
# End Source File
# End Group
# Begin Group "make"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\makefile
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
