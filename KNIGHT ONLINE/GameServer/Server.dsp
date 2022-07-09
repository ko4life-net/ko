# Microsoft Developer Studio Project File - Name="Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak" CFG="Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Server - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Server - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/AIServer", KBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Server - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib implode.lib Winmm.lib Imm32.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib implode.lib Winmm.lib Imm32.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Server - Win32 Release"
# Name "Server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CircularBuffer.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compress.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Extern.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameSocket.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\global.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ini.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOCPort.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOCPSocket2.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MagicProcess.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MagicTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MagicType1Set.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MagicType2Set.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MagicType3Set.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MagicType4Set.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MakeDefensiveTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MakeGradeItemTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MakeLareItemTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MakeWeaponTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MAP.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MonTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Npc.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NpcItem.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NpcItemSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NpcMagicProcess.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NpcPosSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NpcTable.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NpcTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NpcThread.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Party.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PartyUser.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PathFind.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Region.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RNpcPosSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RoomEvent.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Server.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ServerDlg.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\User.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ZoneInfoSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CircularBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Compress.h
# End Source File
# Begin Source File

SOURCE=.\Define.h
# End Source File
# Begin Source File

SOURCE=.\Extern.h
# End Source File
# Begin Source File

SOURCE=.\GameSocket.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\IMPLODE.H
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\IOCPort.h
# End Source File
# Begin Source File

SOURCE=.\IOCPSocket2.h
# End Source File
# Begin Source File

SOURCE=.\MagicProcess.h
# End Source File
# Begin Source File

SOURCE=.\MagicTableSet.h
# End Source File
# Begin Source File

SOURCE=.\MagicType1Set.h
# End Source File
# Begin Source File

SOURCE=.\MagicType2Set.h
# End Source File
# Begin Source File

SOURCE=.\MagicType3Set.h
# End Source File
# Begin Source File

SOURCE=.\MagicType4Set.h
# End Source File
# Begin Source File

SOURCE=.\MakeDefensiveTableSet.h
# End Source File
# Begin Source File

SOURCE=.\MakeGradeItemTableSet.h
# End Source File
# Begin Source File

SOURCE=.\MakeLareItemTableSet.h
# End Source File
# Begin Source File

SOURCE=.\MakeWeaponTableSet.h
# End Source File
# Begin Source File

SOURCE=.\MAP.h
# End Source File
# Begin Source File

SOURCE=.\MonTableSet.h
# End Source File
# Begin Source File

SOURCE=.\Npc.h
# End Source File
# Begin Source File

SOURCE=.\NpcItem.h
# End Source File
# Begin Source File

SOURCE=.\NpcItemSet.h
# End Source File
# Begin Source File

SOURCE=.\NpcMagicProcess.h
# End Source File
# Begin Source File

SOURCE=.\NpcPosSet.h
# End Source File
# Begin Source File

SOURCE=.\NpcTable.h
# End Source File
# Begin Source File

SOURCE=.\NpcTableSet.h
# End Source File
# Begin Source File

SOURCE=.\NpcThread.h
# End Source File
# Begin Source File

SOURCE=.\Packet.h
# End Source File
# Begin Source File

SOURCE=.\Party.h
# End Source File
# Begin Source File

SOURCE=.\PartyUser.h
# End Source File
# Begin Source File

SOURCE=.\PathFind.h
# End Source File
# Begin Source File

SOURCE=.\Region.h
# End Source File
# Begin Source File

SOURCE=.\RNpcPosSet.h
# End Source File
# Begin Source File

SOURCE=.\RoomEvent.h
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STLMap.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\ZoneInfoSet.h
# End Source File
# End Group
# Begin Group "N3Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\N3BASE\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=.\N3BASE\My_Macro.h
# End Source File
# Begin Source File

SOURCE=.\N3BASE\N3ShapeMgr.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\N3BASE\N3ShapeMgr.h
# End Source File
# Begin Source File

SOURCE=.\N3BASE\StdAfxBase.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\Server.ico
# End Source File
# Begin Source File

SOURCE=.\Server.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
