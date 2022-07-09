# Microsoft Developer Studio Project File - Name="Ebenezer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Ebenezer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ebenezer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ebenezer.mak" CFG="Ebenezer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ebenezer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Ebenezer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Ebenezer", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Implode.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib Implode.lib winmm.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Ebenezer - Win32 Release"
# Name "Ebenezer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AISocket.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BattleSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CircularBuffer.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoefficientSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Compress.cpp
# End Source File
# Begin Source File

SOURCE=.\Ebenezer.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EbenezerDlg.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EVENT.cpp
# End Source File
# Begin Source File

SOURCE=.\EVENT_DATA.cpp
# End Source File
# Begin Source File

SOURCE=.\EventSet.cpp
# End Source File
# Begin Source File

SOURCE=.\EXEC.cpp
# End Source File
# Begin Source File

SOURCE=.\GameEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\HomeSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOCPort.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOCPSocket2.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ItemTableSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Knights.cpp
# End Source File
# Begin Source File

SOURCE=.\KnightsManager.cpp
# End Source File
# Begin Source File

SOURCE=.\KnightsRankSet.cpp
# End Source File
# Begin Source File

SOURCE=.\KnightsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\KnightsUserSet.cpp
# End Source File
# Begin Source File

SOURCE=.\LevelUpTableSet.cpp
# End Source File
# Begin Source File

SOURCE=.\LOGIC_ELSE.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicTableSet.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicType1Set.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicType2Set.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicType3Set.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicType4Set.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicType5Set.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicType8Set.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Npc.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Region.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SharedMem.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UdpSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\User.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ZoneInfoSet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AIPacket.h
# End Source File
# Begin Source File

SOURCE=.\AISocket.h
# End Source File
# Begin Source File

SOURCE=.\BattleSet.h
# End Source File
# Begin Source File

SOURCE=.\CircularBuffer.h
# End Source File
# Begin Source File

SOURCE=.\CoefficientSet.h
# End Source File
# Begin Source File

SOURCE=.\Compress.h
# End Source File
# Begin Source File

SOURCE=.\Define.h
# End Source File
# Begin Source File

SOURCE=.\Ebenezer.h
# End Source File
# Begin Source File

SOURCE=.\EbenezerDlg.h
# End Source File
# Begin Source File

SOURCE=.\EVENT.h
# End Source File
# Begin Source File

SOURCE=.\EVENT_DATA.h
# End Source File
# Begin Source File

SOURCE=.\EventSet.h
# End Source File
# Begin Source File

SOURCE=.\EXEC.h
# End Source File
# Begin Source File

SOURCE=.\GameDefine.h
# End Source File
# Begin Source File

SOURCE=.\GameEvent.h
# End Source File
# Begin Source File

SOURCE=.\HomeSet.h
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

SOURCE=.\ItemTableSet.h
# End Source File
# Begin Source File

SOURCE=.\JvCryption.h
# End Source File
# Begin Source File

SOURCE=.\Knights.h
# End Source File
# Begin Source File

SOURCE=.\KnightsManager.h
# End Source File
# Begin Source File

SOURCE=.\KnightsRankSet.h
# End Source File
# Begin Source File

SOURCE=.\KnightsSet.h
# End Source File
# Begin Source File

SOURCE=.\KnightsUserSet.h
# End Source File
# Begin Source File

SOURCE=.\LevelUpTableSet.h
# End Source File
# Begin Source File

SOURCE=.\LOGIC_ELSE.h
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

SOURCE=.\MagicType5Set.h
# End Source File
# Begin Source File

SOURCE=.\MagicType8Set.h
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\Npc.h
# End Source File
# Begin Source File

SOURCE=.\PacketDefine.h
# End Source File
# Begin Source File

SOURCE=.\Region.h
# End Source File
# Begin Source File

SOURCE=.\SharedMem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STLMap.h
# End Source File
# Begin Source File

SOURCE=.\UdpSocket.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\ZoneInfoSet.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Ebenezer.ico
# End Source File
# Begin Source File

SOURCE=.\Ebenezer.rc
# End Source File
# Begin Source File

SOURCE=.\res\Ebenezer.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "N3Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\N3Base\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=.\N3Base\My_Macro.h
# End Source File
# Begin Source File

SOURCE=.\N3Base\N3ShapeMgr.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\N3Base\N3ShapeMgr.h
# End Source File
# Begin Source File

SOURCE=.\N3Base\StdAfxBase.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\JvCryption.lib
# End Source File
# End Target
# End Project
