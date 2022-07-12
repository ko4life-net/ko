# Microsoft Developer Studio Generated NMAKE File, Based on Server.dsp
!IF "$(CFG)" == ""
CFG=Server - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Server - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Server - Win32 Release" && "$(CFG)" != "Server - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Server - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Server.exe" "$(OUTDIR)\Server.bsc"


CLEAN :
	-@erase "$(INTDIR)\CircularBuffer.obj"
	-@erase "$(INTDIR)\CircularBuffer.sbr"
	-@erase "$(INTDIR)\Compress.obj"
	-@erase "$(INTDIR)\Compress.sbr"
	-@erase "$(INTDIR)\Extern.obj"
	-@erase "$(INTDIR)\Extern.sbr"
	-@erase "$(INTDIR)\GameSocket.obj"
	-@erase "$(INTDIR)\GameSocket.sbr"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\global.sbr"
	-@erase "$(INTDIR)\Ini.obj"
	-@erase "$(INTDIR)\Ini.sbr"
	-@erase "$(INTDIR)\IOCPort.obj"
	-@erase "$(INTDIR)\IOCPort.sbr"
	-@erase "$(INTDIR)\IOCPSocket2.obj"
	-@erase "$(INTDIR)\IOCPSocket2.sbr"
	-@erase "$(INTDIR)\MagicProcess.obj"
	-@erase "$(INTDIR)\MagicProcess.sbr"
	-@erase "$(INTDIR)\MagicTableSet.obj"
	-@erase "$(INTDIR)\MagicTableSet.sbr"
	-@erase "$(INTDIR)\MagicType1Set.obj"
	-@erase "$(INTDIR)\MagicType1Set.sbr"
	-@erase "$(INTDIR)\MagicType2Set.obj"
	-@erase "$(INTDIR)\MagicType2Set.sbr"
	-@erase "$(INTDIR)\MagicType3Set.obj"
	-@erase "$(INTDIR)\MagicType3Set.sbr"
	-@erase "$(INTDIR)\MagicType4Set.obj"
	-@erase "$(INTDIR)\MagicType4Set.sbr"
	-@erase "$(INTDIR)\MakeDefensiveTableSet.obj"
	-@erase "$(INTDIR)\MakeDefensiveTableSet.sbr"
	-@erase "$(INTDIR)\MakeGradeItemTableSet.obj"
	-@erase "$(INTDIR)\MakeGradeItemTableSet.sbr"
	-@erase "$(INTDIR)\MakeLareItemTableSet.obj"
	-@erase "$(INTDIR)\MakeLareItemTableSet.sbr"
	-@erase "$(INTDIR)\MakeWeaponTableSet.obj"
	-@erase "$(INTDIR)\MakeWeaponTableSet.sbr"
	-@erase "$(INTDIR)\MAP.obj"
	-@erase "$(INTDIR)\MAP.sbr"
	-@erase "$(INTDIR)\MonTableSet.obj"
	-@erase "$(INTDIR)\MonTableSet.sbr"
	-@erase "$(INTDIR)\N3ShapeMgr.obj"
	-@erase "$(INTDIR)\N3ShapeMgr.sbr"
	-@erase "$(INTDIR)\Npc.obj"
	-@erase "$(INTDIR)\Npc.sbr"
	-@erase "$(INTDIR)\NpcItem.obj"
	-@erase "$(INTDIR)\NpcItem.sbr"
	-@erase "$(INTDIR)\NpcItemSet.obj"
	-@erase "$(INTDIR)\NpcItemSet.sbr"
	-@erase "$(INTDIR)\NpcMagicProcess.obj"
	-@erase "$(INTDIR)\NpcMagicProcess.sbr"
	-@erase "$(INTDIR)\NpcPosSet.obj"
	-@erase "$(INTDIR)\NpcPosSet.sbr"
	-@erase "$(INTDIR)\NpcTable.obj"
	-@erase "$(INTDIR)\NpcTable.sbr"
	-@erase "$(INTDIR)\NpcTableSet.obj"
	-@erase "$(INTDIR)\NpcTableSet.sbr"
	-@erase "$(INTDIR)\NpcThread.obj"
	-@erase "$(INTDIR)\NpcThread.sbr"
	-@erase "$(INTDIR)\Party.obj"
	-@erase "$(INTDIR)\Party.sbr"
	-@erase "$(INTDIR)\PartyUser.obj"
	-@erase "$(INTDIR)\PartyUser.sbr"
	-@erase "$(INTDIR)\PathFind.obj"
	-@erase "$(INTDIR)\PathFind.sbr"
	-@erase "$(INTDIR)\Region.obj"
	-@erase "$(INTDIR)\Region.sbr"
	-@erase "$(INTDIR)\RNpcPosSet.obj"
	-@erase "$(INTDIR)\RNpcPosSet.sbr"
	-@erase "$(INTDIR)\RoomEvent.obj"
	-@erase "$(INTDIR)\RoomEvent.sbr"
	-@erase "$(INTDIR)\Server.obj"
	-@erase "$(INTDIR)\Server.res"
	-@erase "$(INTDIR)\Server.sbr"
	-@erase "$(INTDIR)\ServerDlg.obj"
	-@erase "$(INTDIR)\ServerDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\User.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ZoneInfoSet.obj"
	-@erase "$(INTDIR)\ZoneInfoSet.sbr"
	-@erase "$(OUTDIR)\Server.bsc"
	-@erase "$(OUTDIR)\Server.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Server.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Server.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CircularBuffer.sbr" \
	"$(INTDIR)\Compress.sbr" \
	"$(INTDIR)\Extern.sbr" \
	"$(INTDIR)\GameSocket.sbr" \
	"$(INTDIR)\global.sbr" \
	"$(INTDIR)\Ini.sbr" \
	"$(INTDIR)\IOCPort.sbr" \
	"$(INTDIR)\IOCPSocket2.sbr" \
	"$(INTDIR)\MagicProcess.sbr" \
	"$(INTDIR)\MagicTableSet.sbr" \
	"$(INTDIR)\MagicType1Set.sbr" \
	"$(INTDIR)\MagicType2Set.sbr" \
	"$(INTDIR)\MagicType3Set.sbr" \
	"$(INTDIR)\MagicType4Set.sbr" \
	"$(INTDIR)\MakeDefensiveTableSet.sbr" \
	"$(INTDIR)\MakeGradeItemTableSet.sbr" \
	"$(INTDIR)\MakeLareItemTableSet.sbr" \
	"$(INTDIR)\MakeWeaponTableSet.sbr" \
	"$(INTDIR)\MAP.sbr" \
	"$(INTDIR)\MonTableSet.sbr" \
	"$(INTDIR)\Npc.sbr" \
	"$(INTDIR)\NpcItem.sbr" \
	"$(INTDIR)\NpcItemSet.sbr" \
	"$(INTDIR)\NpcMagicProcess.sbr" \
	"$(INTDIR)\NpcPosSet.sbr" \
	"$(INTDIR)\NpcTable.sbr" \
	"$(INTDIR)\NpcTableSet.sbr" \
	"$(INTDIR)\NpcThread.sbr" \
	"$(INTDIR)\Party.sbr" \
	"$(INTDIR)\PartyUser.sbr" \
	"$(INTDIR)\PathFind.sbr" \
	"$(INTDIR)\Region.sbr" \
	"$(INTDIR)\RNpcPosSet.sbr" \
	"$(INTDIR)\RoomEvent.sbr" \
	"$(INTDIR)\Server.sbr" \
	"$(INTDIR)\ServerDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\User.sbr" \
	"$(INTDIR)\ZoneInfoSet.sbr" \
	"$(INTDIR)\N3ShapeMgr.sbr"

"$(OUTDIR)\Server.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ws2_32.lib implode.lib Winmm.lib Imm32.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Server.pdb" /machine:I386 /out:"$(OUTDIR)\Server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\CircularBuffer.obj" \
	"$(INTDIR)\Compress.obj" \
	"$(INTDIR)\Extern.obj" \
	"$(INTDIR)\GameSocket.obj" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\Ini.obj" \
	"$(INTDIR)\IOCPort.obj" \
	"$(INTDIR)\IOCPSocket2.obj" \
	"$(INTDIR)\MagicProcess.obj" \
	"$(INTDIR)\MagicTableSet.obj" \
	"$(INTDIR)\MagicType1Set.obj" \
	"$(INTDIR)\MagicType2Set.obj" \
	"$(INTDIR)\MagicType3Set.obj" \
	"$(INTDIR)\MagicType4Set.obj" \
	"$(INTDIR)\MakeDefensiveTableSet.obj" \
	"$(INTDIR)\MakeGradeItemTableSet.obj" \
	"$(INTDIR)\MakeLareItemTableSet.obj" \
	"$(INTDIR)\MakeWeaponTableSet.obj" \
	"$(INTDIR)\MAP.obj" \
	"$(INTDIR)\MonTableSet.obj" \
	"$(INTDIR)\Npc.obj" \
	"$(INTDIR)\NpcItem.obj" \
	"$(INTDIR)\NpcItemSet.obj" \
	"$(INTDIR)\NpcMagicProcess.obj" \
	"$(INTDIR)\NpcPosSet.obj" \
	"$(INTDIR)\NpcTable.obj" \
	"$(INTDIR)\NpcTableSet.obj" \
	"$(INTDIR)\NpcThread.obj" \
	"$(INTDIR)\Party.obj" \
	"$(INTDIR)\PartyUser.obj" \
	"$(INTDIR)\PathFind.obj" \
	"$(INTDIR)\Region.obj" \
	"$(INTDIR)\RNpcPosSet.obj" \
	"$(INTDIR)\RoomEvent.obj" \
	"$(INTDIR)\Server.obj" \
	"$(INTDIR)\ServerDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\ZoneInfoSet.obj" \
	"$(INTDIR)\N3ShapeMgr.obj" \
	"$(INTDIR)\Server.res"

"$(OUTDIR)\Server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Server.exe" "$(OUTDIR)\Server.bsc"


CLEAN :
	-@erase "$(INTDIR)\CircularBuffer.obj"
	-@erase "$(INTDIR)\CircularBuffer.sbr"
	-@erase "$(INTDIR)\Compress.obj"
	-@erase "$(INTDIR)\Compress.sbr"
	-@erase "$(INTDIR)\Extern.obj"
	-@erase "$(INTDIR)\Extern.sbr"
	-@erase "$(INTDIR)\GameSocket.obj"
	-@erase "$(INTDIR)\GameSocket.sbr"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\global.sbr"
	-@erase "$(INTDIR)\Ini.obj"
	-@erase "$(INTDIR)\Ini.sbr"
	-@erase "$(INTDIR)\IOCPort.obj"
	-@erase "$(INTDIR)\IOCPort.sbr"
	-@erase "$(INTDIR)\IOCPSocket2.obj"
	-@erase "$(INTDIR)\IOCPSocket2.sbr"
	-@erase "$(INTDIR)\MagicProcess.obj"
	-@erase "$(INTDIR)\MagicProcess.sbr"
	-@erase "$(INTDIR)\MagicTableSet.obj"
	-@erase "$(INTDIR)\MagicTableSet.sbr"
	-@erase "$(INTDIR)\MagicType1Set.obj"
	-@erase "$(INTDIR)\MagicType1Set.sbr"
	-@erase "$(INTDIR)\MagicType2Set.obj"
	-@erase "$(INTDIR)\MagicType2Set.sbr"
	-@erase "$(INTDIR)\MagicType3Set.obj"
	-@erase "$(INTDIR)\MagicType3Set.sbr"
	-@erase "$(INTDIR)\MagicType4Set.obj"
	-@erase "$(INTDIR)\MagicType4Set.sbr"
	-@erase "$(INTDIR)\MakeDefensiveTableSet.obj"
	-@erase "$(INTDIR)\MakeDefensiveTableSet.sbr"
	-@erase "$(INTDIR)\MakeGradeItemTableSet.obj"
	-@erase "$(INTDIR)\MakeGradeItemTableSet.sbr"
	-@erase "$(INTDIR)\MakeLareItemTableSet.obj"
	-@erase "$(INTDIR)\MakeLareItemTableSet.sbr"
	-@erase "$(INTDIR)\MakeWeaponTableSet.obj"
	-@erase "$(INTDIR)\MakeWeaponTableSet.sbr"
	-@erase "$(INTDIR)\MAP.obj"
	-@erase "$(INTDIR)\MAP.sbr"
	-@erase "$(INTDIR)\MonTableSet.obj"
	-@erase "$(INTDIR)\MonTableSet.sbr"
	-@erase "$(INTDIR)\N3ShapeMgr.obj"
	-@erase "$(INTDIR)\N3ShapeMgr.sbr"
	-@erase "$(INTDIR)\Npc.obj"
	-@erase "$(INTDIR)\Npc.sbr"
	-@erase "$(INTDIR)\NpcItem.obj"
	-@erase "$(INTDIR)\NpcItem.sbr"
	-@erase "$(INTDIR)\NpcItemSet.obj"
	-@erase "$(INTDIR)\NpcItemSet.sbr"
	-@erase "$(INTDIR)\NpcMagicProcess.obj"
	-@erase "$(INTDIR)\NpcMagicProcess.sbr"
	-@erase "$(INTDIR)\NpcPosSet.obj"
	-@erase "$(INTDIR)\NpcPosSet.sbr"
	-@erase "$(INTDIR)\NpcTable.obj"
	-@erase "$(INTDIR)\NpcTable.sbr"
	-@erase "$(INTDIR)\NpcTableSet.obj"
	-@erase "$(INTDIR)\NpcTableSet.sbr"
	-@erase "$(INTDIR)\NpcThread.obj"
	-@erase "$(INTDIR)\NpcThread.sbr"
	-@erase "$(INTDIR)\Party.obj"
	-@erase "$(INTDIR)\Party.sbr"
	-@erase "$(INTDIR)\PartyUser.obj"
	-@erase "$(INTDIR)\PartyUser.sbr"
	-@erase "$(INTDIR)\PathFind.obj"
	-@erase "$(INTDIR)\PathFind.sbr"
	-@erase "$(INTDIR)\Region.obj"
	-@erase "$(INTDIR)\Region.sbr"
	-@erase "$(INTDIR)\RNpcPosSet.obj"
	-@erase "$(INTDIR)\RNpcPosSet.sbr"
	-@erase "$(INTDIR)\RoomEvent.obj"
	-@erase "$(INTDIR)\RoomEvent.sbr"
	-@erase "$(INTDIR)\Server.obj"
	-@erase "$(INTDIR)\Server.res"
	-@erase "$(INTDIR)\Server.sbr"
	-@erase "$(INTDIR)\ServerDlg.obj"
	-@erase "$(INTDIR)\ServerDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\User.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ZoneInfoSet.obj"
	-@erase "$(INTDIR)\ZoneInfoSet.sbr"
	-@erase "$(OUTDIR)\Server.bsc"
	-@erase "$(OUTDIR)\Server.exe"
	-@erase "$(OUTDIR)\Server.ilk"
	-@erase "$(OUTDIR)\Server.map"
	-@erase "$(OUTDIR)\Server.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Server.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Server.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CircularBuffer.sbr" \
	"$(INTDIR)\Compress.sbr" \
	"$(INTDIR)\Extern.sbr" \
	"$(INTDIR)\GameSocket.sbr" \
	"$(INTDIR)\global.sbr" \
	"$(INTDIR)\Ini.sbr" \
	"$(INTDIR)\IOCPort.sbr" \
	"$(INTDIR)\IOCPSocket2.sbr" \
	"$(INTDIR)\MagicProcess.sbr" \
	"$(INTDIR)\MagicTableSet.sbr" \
	"$(INTDIR)\MagicType1Set.sbr" \
	"$(INTDIR)\MagicType2Set.sbr" \
	"$(INTDIR)\MagicType3Set.sbr" \
	"$(INTDIR)\MagicType4Set.sbr" \
	"$(INTDIR)\MakeDefensiveTableSet.sbr" \
	"$(INTDIR)\MakeGradeItemTableSet.sbr" \
	"$(INTDIR)\MakeLareItemTableSet.sbr" \
	"$(INTDIR)\MakeWeaponTableSet.sbr" \
	"$(INTDIR)\MAP.sbr" \
	"$(INTDIR)\MonTableSet.sbr" \
	"$(INTDIR)\Npc.sbr" \
	"$(INTDIR)\NpcItem.sbr" \
	"$(INTDIR)\NpcItemSet.sbr" \
	"$(INTDIR)\NpcMagicProcess.sbr" \
	"$(INTDIR)\NpcPosSet.sbr" \
	"$(INTDIR)\NpcTable.sbr" \
	"$(INTDIR)\NpcTableSet.sbr" \
	"$(INTDIR)\NpcThread.sbr" \
	"$(INTDIR)\Party.sbr" \
	"$(INTDIR)\PartyUser.sbr" \
	"$(INTDIR)\PathFind.sbr" \
	"$(INTDIR)\Region.sbr" \
	"$(INTDIR)\RNpcPosSet.sbr" \
	"$(INTDIR)\RoomEvent.sbr" \
	"$(INTDIR)\Server.sbr" \
	"$(INTDIR)\ServerDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\User.sbr" \
	"$(INTDIR)\ZoneInfoSet.sbr" \
	"$(INTDIR)\N3ShapeMgr.sbr"

"$(OUTDIR)\Server.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ws2_32.lib implode.lib Winmm.lib Imm32.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Server.pdb" /map:"$(INTDIR)\Server.map" /debug /machine:I386 /out:"$(OUTDIR)\Server.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\CircularBuffer.obj" \
	"$(INTDIR)\Compress.obj" \
	"$(INTDIR)\Extern.obj" \
	"$(INTDIR)\GameSocket.obj" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\Ini.obj" \
	"$(INTDIR)\IOCPort.obj" \
	"$(INTDIR)\IOCPSocket2.obj" \
	"$(INTDIR)\MagicProcess.obj" \
	"$(INTDIR)\MagicTableSet.obj" \
	"$(INTDIR)\MagicType1Set.obj" \
	"$(INTDIR)\MagicType2Set.obj" \
	"$(INTDIR)\MagicType3Set.obj" \
	"$(INTDIR)\MagicType4Set.obj" \
	"$(INTDIR)\MakeDefensiveTableSet.obj" \
	"$(INTDIR)\MakeGradeItemTableSet.obj" \
	"$(INTDIR)\MakeLareItemTableSet.obj" \
	"$(INTDIR)\MakeWeaponTableSet.obj" \
	"$(INTDIR)\MAP.obj" \
	"$(INTDIR)\MonTableSet.obj" \
	"$(INTDIR)\Npc.obj" \
	"$(INTDIR)\NpcItem.obj" \
	"$(INTDIR)\NpcItemSet.obj" \
	"$(INTDIR)\NpcMagicProcess.obj" \
	"$(INTDIR)\NpcPosSet.obj" \
	"$(INTDIR)\NpcTable.obj" \
	"$(INTDIR)\NpcTableSet.obj" \
	"$(INTDIR)\NpcThread.obj" \
	"$(INTDIR)\Party.obj" \
	"$(INTDIR)\PartyUser.obj" \
	"$(INTDIR)\PathFind.obj" \
	"$(INTDIR)\Region.obj" \
	"$(INTDIR)\RNpcPosSet.obj" \
	"$(INTDIR)\RoomEvent.obj" \
	"$(INTDIR)\Server.obj" \
	"$(INTDIR)\ServerDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\ZoneInfoSet.obj" \
	"$(INTDIR)\N3ShapeMgr.obj" \
	"$(INTDIR)\Server.res"

"$(OUTDIR)\Server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Server.dep")
!INCLUDE "Server.dep"
!ELSE 
!MESSAGE Warning: cannot find "Server.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Server - Win32 Release" || "$(CFG)" == "Server - Win32 Debug"
SOURCE=.\CircularBuffer.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\CircularBuffer.obj"	"$(INTDIR)\CircularBuffer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\CircularBuffer.obj"	"$(INTDIR)\CircularBuffer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Compress.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Compress.obj"	"$(INTDIR)\Compress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Compress.obj"	"$(INTDIR)\Compress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Extern.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Extern.obj"	"$(INTDIR)\Extern.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Extern.obj"	"$(INTDIR)\Extern.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\GameSocket.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\GameSocket.obj"	"$(INTDIR)\GameSocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\GameSocket.obj"	"$(INTDIR)\GameSocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\global.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\global.obj"	"$(INTDIR)\global.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\global.obj"	"$(INTDIR)\global.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Ini.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Ini.obj"	"$(INTDIR)\Ini.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Ini.obj"	"$(INTDIR)\Ini.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\IOCPort.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\IOCPort.obj"	"$(INTDIR)\IOCPort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\IOCPort.obj"	"$(INTDIR)\IOCPort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\IOCPSocket2.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\IOCPSocket2.obj"	"$(INTDIR)\IOCPSocket2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\IOCPSocket2.obj"	"$(INTDIR)\IOCPSocket2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MagicProcess.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MagicProcess.obj"	"$(INTDIR)\MagicProcess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MagicProcess.obj"	"$(INTDIR)\MagicProcess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MagicTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MagicTableSet.obj"	"$(INTDIR)\MagicTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MagicTableSet.obj"	"$(INTDIR)\MagicTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MagicType1Set.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MagicType1Set.obj"	"$(INTDIR)\MagicType1Set.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MagicType1Set.obj"	"$(INTDIR)\MagicType1Set.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MagicType2Set.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MagicType2Set.obj"	"$(INTDIR)\MagicType2Set.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MagicType2Set.obj"	"$(INTDIR)\MagicType2Set.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MagicType3Set.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MagicType3Set.obj"	"$(INTDIR)\MagicType3Set.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MagicType3Set.obj"	"$(INTDIR)\MagicType3Set.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MagicType4Set.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MagicType4Set.obj"	"$(INTDIR)\MagicType4Set.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MagicType4Set.obj"	"$(INTDIR)\MagicType4Set.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MakeDefensiveTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MakeDefensiveTableSet.obj"	"$(INTDIR)\MakeDefensiveTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MakeDefensiveTableSet.obj"	"$(INTDIR)\MakeDefensiveTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MakeGradeItemTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MakeGradeItemTableSet.obj"	"$(INTDIR)\MakeGradeItemTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MakeGradeItemTableSet.obj"	"$(INTDIR)\MakeGradeItemTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MakeLareItemTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MakeLareItemTableSet.obj"	"$(INTDIR)\MakeLareItemTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MakeLareItemTableSet.obj"	"$(INTDIR)\MakeLareItemTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MakeWeaponTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MakeWeaponTableSet.obj"	"$(INTDIR)\MakeWeaponTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MakeWeaponTableSet.obj"	"$(INTDIR)\MakeWeaponTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MAP.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MAP.obj"	"$(INTDIR)\MAP.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MAP.obj"	"$(INTDIR)\MAP.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\MonTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\MonTableSet.obj"	"$(INTDIR)\MonTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\MonTableSet.obj"	"$(INTDIR)\MonTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Npc.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Npc.obj"	"$(INTDIR)\Npc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Npc.obj"	"$(INTDIR)\Npc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\NpcItem.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NpcItem.obj"	"$(INTDIR)\NpcItem.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\NpcItem.obj"	"$(INTDIR)\NpcItem.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\NpcItemSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NpcItemSet.obj"	"$(INTDIR)\NpcItemSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\NpcItemSet.obj"	"$(INTDIR)\NpcItemSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\NpcMagicProcess.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NpcMagicProcess.obj"	"$(INTDIR)\NpcMagicProcess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\NpcMagicProcess.obj"	"$(INTDIR)\NpcMagicProcess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\NpcPosSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NpcPosSet.obj"	"$(INTDIR)\NpcPosSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\NpcPosSet.obj"	"$(INTDIR)\NpcPosSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\NpcTable.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NpcTable.obj"	"$(INTDIR)\NpcTable.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\NpcTable.obj"	"$(INTDIR)\NpcTable.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\NpcTableSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NpcTableSet.obj"	"$(INTDIR)\NpcTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\NpcTableSet.obj"	"$(INTDIR)\NpcTableSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\NpcThread.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NpcThread.obj"	"$(INTDIR)\NpcThread.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\NpcThread.obj"	"$(INTDIR)\NpcThread.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Party.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Party.obj"	"$(INTDIR)\Party.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Party.obj"	"$(INTDIR)\Party.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\PartyUser.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PartyUser.obj"	"$(INTDIR)\PartyUser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\PartyUser.obj"	"$(INTDIR)\PartyUser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\PathFind.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\PathFind.obj"	"$(INTDIR)\PathFind.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\PathFind.obj"	"$(INTDIR)\PathFind.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Region.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Region.obj"	"$(INTDIR)\Region.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Region.obj"	"$(INTDIR)\Region.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\RNpcPosSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\RNpcPosSet.obj"	"$(INTDIR)\RNpcPosSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\RNpcPosSet.obj"	"$(INTDIR)\RNpcPosSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\RoomEvent.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\RoomEvent.obj"	"$(INTDIR)\RoomEvent.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\RoomEvent.obj"	"$(INTDIR)\RoomEvent.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Server.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Server.obj"	"$(INTDIR)\Server.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Server.obj"	"$(INTDIR)\Server.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ServerDlg.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ServerDlg.obj"	"$(INTDIR)\ServerDlg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ServerDlg.obj"	"$(INTDIR)\ServerDlg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\User.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\User.obj"	"$(INTDIR)\User.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\User.obj"	"$(INTDIR)\User.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ZoneInfoSet.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ZoneInfoSet.obj"	"$(INTDIR)\ZoneInfoSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ZoneInfoSet.obj"	"$(INTDIR)\ZoneInfoSet.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\N3BASE\N3ShapeMgr.cpp

!IF  "$(CFG)" == "Server - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\N3ShapeMgr.obj"	"$(INTDIR)\N3ShapeMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /D "_REPENT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\N3ShapeMgr.obj"	"$(INTDIR)\N3ShapeMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Server.rc

"$(INTDIR)\Server.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

