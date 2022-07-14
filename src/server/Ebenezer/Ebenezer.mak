# Microsoft Developer Studio Generated NMAKE File, Based on Ebenezer.dsp
!IF "$(CFG)" == ""
CFG=Ebenezer - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Ebenezer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Ebenezer - Win32 Release" && "$(CFG)" != "Ebenezer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Ebenezer.exe"


CLEAN :
	-@erase "$(INTDIR)\AISocket.obj"
	-@erase "$(INTDIR)\BattleSet.obj"
	-@erase "$(INTDIR)\CircularBuffer.obj"
	-@erase "$(INTDIR)\CoefficientSet.obj"
	-@erase "$(INTDIR)\Compress.obj"
	-@erase "$(INTDIR)\Ebenezer.obj"
	-@erase "$(INTDIR)\Ebenezer.pch"
	-@erase "$(INTDIR)\Ebenezer.res"
	-@erase "$(INTDIR)\EbenezerDlg.obj"
	-@erase "$(INTDIR)\EVENT.obj"
	-@erase "$(INTDIR)\EVENT_DATA.obj"
	-@erase "$(INTDIR)\EventSet.obj"
	-@erase "$(INTDIR)\EXEC.obj"
	-@erase "$(INTDIR)\GameEvent.obj"
	-@erase "$(INTDIR)\HomeSet.obj"
	-@erase "$(INTDIR)\Ini.obj"
	-@erase "$(INTDIR)\IOCPort.obj"
	-@erase "$(INTDIR)\IOCPSocket2.obj"
	-@erase "$(INTDIR)\ItemTableSet.obj"
	-@erase "$(INTDIR)\Knights.obj"
	-@erase "$(INTDIR)\KnightsManager.obj"
	-@erase "$(INTDIR)\KnightsRankSet.obj"
	-@erase "$(INTDIR)\KnightsSet.obj"
	-@erase "$(INTDIR)\KnightsUserSet.obj"
	-@erase "$(INTDIR)\LevelUpTableSet.obj"
	-@erase "$(INTDIR)\LOGIC_ELSE.obj"
	-@erase "$(INTDIR)\MagicProcess.obj"
	-@erase "$(INTDIR)\MagicTableSet.obj"
	-@erase "$(INTDIR)\MagicType1Set.obj"
	-@erase "$(INTDIR)\MagicType2Set.obj"
	-@erase "$(INTDIR)\MagicType3Set.obj"
	-@erase "$(INTDIR)\MagicType4Set.obj"
	-@erase "$(INTDIR)\MagicType5Set.obj"
	-@erase "$(INTDIR)\MagicType8Set.obj"
	-@erase "$(INTDIR)\Map.obj"
	-@erase "$(INTDIR)\N3ShapeMgr.obj"
	-@erase "$(INTDIR)\Npc.obj"
	-@erase "$(INTDIR)\Region.obj"
	-@erase "$(INTDIR)\SharedMem.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\UdpSocket.obj"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ZoneInfoSet.obj"
	-@erase "$(OUTDIR)\Ebenezer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Ebenezer.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ebenezer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Implode.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Ebenezer.pdb" /machine:I386 /out:"$(OUTDIR)\Ebenezer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AISocket.obj" \
	"$(INTDIR)\BattleSet.obj" \
	"$(INTDIR)\CircularBuffer.obj" \
	"$(INTDIR)\CoefficientSet.obj" \
	"$(INTDIR)\Compress.obj" \
	"$(INTDIR)\Ebenezer.obj" \
	"$(INTDIR)\EbenezerDlg.obj" \
	"$(INTDIR)\EVENT.obj" \
	"$(INTDIR)\EVENT_DATA.obj" \
	"$(INTDIR)\EventSet.obj" \
	"$(INTDIR)\EXEC.obj" \
	"$(INTDIR)\GameEvent.obj" \
	"$(INTDIR)\HomeSet.obj" \
	"$(INTDIR)\Ini.obj" \
	"$(INTDIR)\IOCPort.obj" \
	"$(INTDIR)\IOCPSocket2.obj" \
	"$(INTDIR)\ItemTableSet.obj" \
	"$(INTDIR)\Knights.obj" \
	"$(INTDIR)\KnightsManager.obj" \
	"$(INTDIR)\KnightsRankSet.obj" \
	"$(INTDIR)\KnightsSet.obj" \
	"$(INTDIR)\KnightsUserSet.obj" \
	"$(INTDIR)\LevelUpTableSet.obj" \
	"$(INTDIR)\LOGIC_ELSE.obj" \
	"$(INTDIR)\MagicProcess.obj" \
	"$(INTDIR)\MagicTableSet.obj" \
	"$(INTDIR)\MagicType1Set.obj" \
	"$(INTDIR)\MagicType2Set.obj" \
	"$(INTDIR)\MagicType3Set.obj" \
	"$(INTDIR)\MagicType4Set.obj" \
	"$(INTDIR)\MagicType5Set.obj" \
	"$(INTDIR)\MagicType8Set.obj" \
	"$(INTDIR)\Map.obj" \
	"$(INTDIR)\Npc.obj" \
	"$(INTDIR)\Region.obj" \
	"$(INTDIR)\SharedMem.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UdpSocket.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\ZoneInfoSet.obj" \
	"$(INTDIR)\N3ShapeMgr.obj" \
	"$(INTDIR)\Ebenezer.res" \
	".\JvCryption.lib"

"$(OUTDIR)\Ebenezer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Ebenezer.exe" "$(OUTDIR)\Ebenezer.bsc"


CLEAN :
	-@erase "$(INTDIR)\AISocket.obj"
	-@erase "$(INTDIR)\AISocket.sbr"
	-@erase "$(INTDIR)\BattleSet.obj"
	-@erase "$(INTDIR)\BattleSet.sbr"
	-@erase "$(INTDIR)\CircularBuffer.obj"
	-@erase "$(INTDIR)\CircularBuffer.sbr"
	-@erase "$(INTDIR)\CoefficientSet.obj"
	-@erase "$(INTDIR)\CoefficientSet.sbr"
	-@erase "$(INTDIR)\Compress.obj"
	-@erase "$(INTDIR)\Compress.sbr"
	-@erase "$(INTDIR)\Ebenezer.obj"
	-@erase "$(INTDIR)\Ebenezer.res"
	-@erase "$(INTDIR)\Ebenezer.sbr"
	-@erase "$(INTDIR)\EbenezerDlg.obj"
	-@erase "$(INTDIR)\EbenezerDlg.sbr"
	-@erase "$(INTDIR)\EVENT.obj"
	-@erase "$(INTDIR)\EVENT.sbr"
	-@erase "$(INTDIR)\EVENT_DATA.obj"
	-@erase "$(INTDIR)\EVENT_DATA.sbr"
	-@erase "$(INTDIR)\EventSet.obj"
	-@erase "$(INTDIR)\EventSet.sbr"
	-@erase "$(INTDIR)\EXEC.obj"
	-@erase "$(INTDIR)\EXEC.sbr"
	-@erase "$(INTDIR)\GameEvent.obj"
	-@erase "$(INTDIR)\GameEvent.sbr"
	-@erase "$(INTDIR)\HomeSet.obj"
	-@erase "$(INTDIR)\HomeSet.sbr"
	-@erase "$(INTDIR)\Ini.obj"
	-@erase "$(INTDIR)\Ini.sbr"
	-@erase "$(INTDIR)\IOCPort.obj"
	-@erase "$(INTDIR)\IOCPort.sbr"
	-@erase "$(INTDIR)\IOCPSocket2.obj"
	-@erase "$(INTDIR)\IOCPSocket2.sbr"
	-@erase "$(INTDIR)\ItemTableSet.obj"
	-@erase "$(INTDIR)\ItemTableSet.sbr"
	-@erase "$(INTDIR)\Knights.obj"
	-@erase "$(INTDIR)\Knights.sbr"
	-@erase "$(INTDIR)\KnightsManager.obj"
	-@erase "$(INTDIR)\KnightsManager.sbr"
	-@erase "$(INTDIR)\KnightsRankSet.obj"
	-@erase "$(INTDIR)\KnightsRankSet.sbr"
	-@erase "$(INTDIR)\KnightsSet.obj"
	-@erase "$(INTDIR)\KnightsSet.sbr"
	-@erase "$(INTDIR)\KnightsUserSet.obj"
	-@erase "$(INTDIR)\KnightsUserSet.sbr"
	-@erase "$(INTDIR)\LevelUpTableSet.obj"
	-@erase "$(INTDIR)\LevelUpTableSet.sbr"
	-@erase "$(INTDIR)\LOGIC_ELSE.obj"
	-@erase "$(INTDIR)\LOGIC_ELSE.sbr"
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
	-@erase "$(INTDIR)\MagicType5Set.obj"
	-@erase "$(INTDIR)\MagicType5Set.sbr"
	-@erase "$(INTDIR)\MagicType8Set.obj"
	-@erase "$(INTDIR)\MagicType8Set.sbr"
	-@erase "$(INTDIR)\Map.obj"
	-@erase "$(INTDIR)\Map.sbr"
	-@erase "$(INTDIR)\N3ShapeMgr.obj"
	-@erase "$(INTDIR)\N3ShapeMgr.sbr"
	-@erase "$(INTDIR)\Npc.obj"
	-@erase "$(INTDIR)\Npc.sbr"
	-@erase "$(INTDIR)\Region.obj"
	-@erase "$(INTDIR)\Region.sbr"
	-@erase "$(INTDIR)\SharedMem.obj"
	-@erase "$(INTDIR)\SharedMem.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\UdpSocket.obj"
	-@erase "$(INTDIR)\UdpSocket.sbr"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\User.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ZoneInfoSet.obj"
	-@erase "$(INTDIR)\ZoneInfoSet.sbr"
	-@erase "$(OUTDIR)\Ebenezer.bsc"
	-@erase "$(OUTDIR)\Ebenezer.exe"
	-@erase "$(OUTDIR)\Ebenezer.ilk"
	-@erase "$(OUTDIR)\Ebenezer.map"
	-@erase "$(OUTDIR)\Ebenezer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Ebenezer.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ebenezer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AISocket.sbr" \
	"$(INTDIR)\BattleSet.sbr" \
	"$(INTDIR)\CircularBuffer.sbr" \
	"$(INTDIR)\CoefficientSet.sbr" \
	"$(INTDIR)\Compress.sbr" \
	"$(INTDIR)\Ebenezer.sbr" \
	"$(INTDIR)\EbenezerDlg.sbr" \
	"$(INTDIR)\EVENT.sbr" \
	"$(INTDIR)\EVENT_DATA.sbr" \
	"$(INTDIR)\EventSet.sbr" \
	"$(INTDIR)\EXEC.sbr" \
	"$(INTDIR)\GameEvent.sbr" \
	"$(INTDIR)\HomeSet.sbr" \
	"$(INTDIR)\Ini.sbr" \
	"$(INTDIR)\IOCPort.sbr" \
	"$(INTDIR)\IOCPSocket2.sbr" \
	"$(INTDIR)\ItemTableSet.sbr" \
	"$(INTDIR)\Knights.sbr" \
	"$(INTDIR)\KnightsManager.sbr" \
	"$(INTDIR)\KnightsRankSet.sbr" \
	"$(INTDIR)\KnightsSet.sbr" \
	"$(INTDIR)\KnightsUserSet.sbr" \
	"$(INTDIR)\LevelUpTableSet.sbr" \
	"$(INTDIR)\LOGIC_ELSE.sbr" \
	"$(INTDIR)\MagicProcess.sbr" \
	"$(INTDIR)\MagicTableSet.sbr" \
	"$(INTDIR)\MagicType1Set.sbr" \
	"$(INTDIR)\MagicType2Set.sbr" \
	"$(INTDIR)\MagicType3Set.sbr" \
	"$(INTDIR)\MagicType4Set.sbr" \
	"$(INTDIR)\MagicType5Set.sbr" \
	"$(INTDIR)\MagicType8Set.sbr" \
	"$(INTDIR)\Map.sbr" \
	"$(INTDIR)\Npc.sbr" \
	"$(INTDIR)\Region.sbr" \
	"$(INTDIR)\SharedMem.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\UdpSocket.sbr" \
	"$(INTDIR)\User.sbr" \
	"$(INTDIR)\ZoneInfoSet.sbr" \
	"$(INTDIR)\N3ShapeMgr.sbr"

"$(OUTDIR)\Ebenezer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ws2_32.lib Implode.lib winmm.lib d3d9.lib d3dx9.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Ebenezer.pdb" /map:"$(INTDIR)\Ebenezer.map" /debug /machine:I386 /out:"$(OUTDIR)\Ebenezer.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AISocket.obj" \
	"$(INTDIR)\BattleSet.obj" \
	"$(INTDIR)\CircularBuffer.obj" \
	"$(INTDIR)\CoefficientSet.obj" \
	"$(INTDIR)\Compress.obj" \
	"$(INTDIR)\Ebenezer.obj" \
	"$(INTDIR)\EbenezerDlg.obj" \
	"$(INTDIR)\EVENT.obj" \
	"$(INTDIR)\EVENT_DATA.obj" \
	"$(INTDIR)\EventSet.obj" \
	"$(INTDIR)\EXEC.obj" \
	"$(INTDIR)\GameEvent.obj" \
	"$(INTDIR)\HomeSet.obj" \
	"$(INTDIR)\Ini.obj" \
	"$(INTDIR)\IOCPort.obj" \
	"$(INTDIR)\IOCPSocket2.obj" \
	"$(INTDIR)\ItemTableSet.obj" \
	"$(INTDIR)\Knights.obj" \
	"$(INTDIR)\KnightsManager.obj" \
	"$(INTDIR)\KnightsRankSet.obj" \
	"$(INTDIR)\KnightsSet.obj" \
	"$(INTDIR)\KnightsUserSet.obj" \
	"$(INTDIR)\LevelUpTableSet.obj" \
	"$(INTDIR)\LOGIC_ELSE.obj" \
	"$(INTDIR)\MagicProcess.obj" \
	"$(INTDIR)\MagicTableSet.obj" \
	"$(INTDIR)\MagicType1Set.obj" \
	"$(INTDIR)\MagicType2Set.obj" \
	"$(INTDIR)\MagicType3Set.obj" \
	"$(INTDIR)\MagicType4Set.obj" \
	"$(INTDIR)\MagicType5Set.obj" \
	"$(INTDIR)\MagicType8Set.obj" \
	"$(INTDIR)\Map.obj" \
	"$(INTDIR)\Npc.obj" \
	"$(INTDIR)\Region.obj" \
	"$(INTDIR)\SharedMem.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UdpSocket.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\ZoneInfoSet.obj" \
	"$(INTDIR)\N3ShapeMgr.obj" \
	"$(INTDIR)\Ebenezer.res" \
	".\JvCryption.lib"

"$(OUTDIR)\Ebenezer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Ebenezer.dep")
!INCLUDE "Ebenezer.dep"
!ELSE 
!MESSAGE Warning: cannot find "Ebenezer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Ebenezer - Win32 Release" || "$(CFG)" == "Ebenezer - Win32 Debug"
SOURCE=.\AISocket.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\AISocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\AISocket.obj"	"$(INTDIR)\AISocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\BattleSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\BattleSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\BattleSet.obj"	"$(INTDIR)\BattleSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CircularBuffer.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\CircularBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\CircularBuffer.obj"	"$(INTDIR)\CircularBuffer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\CoefficientSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\CoefficientSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\CoefficientSet.obj"	"$(INTDIR)\CoefficientSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Compress.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\Compress.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\Compress.obj"	"$(INTDIR)\Compress.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Ebenezer.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Ebenezer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Ebenezer.obj"	"$(INTDIR)\Ebenezer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\EbenezerDlg.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\EbenezerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\EbenezerDlg.obj"	"$(INTDIR)\EbenezerDlg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\EVENT.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\EVENT.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\EVENT.obj"	"$(INTDIR)\EVENT.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\EVENT_DATA.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\EVENT_DATA.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\EVENT_DATA.obj"	"$(INTDIR)\EVENT_DATA.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\EventSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\EventSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\EventSet.obj"	"$(INTDIR)\EventSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\EXEC.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\EXEC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\EXEC.obj"	"$(INTDIR)\EXEC.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\GameEvent.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\GameEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\GameEvent.obj"	"$(INTDIR)\GameEvent.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HomeSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\HomeSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\HomeSet.obj"	"$(INTDIR)\HomeSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Ini.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Ini.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Ini.obj"	"$(INTDIR)\Ini.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\IOCPort.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\IOCPort.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\IOCPort.obj"	"$(INTDIR)\IOCPort.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\IOCPSocket2.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\IOCPSocket2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\IOCPSocket2.obj"	"$(INTDIR)\IOCPSocket2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ItemTableSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\ItemTableSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\ItemTableSet.obj"	"$(INTDIR)\ItemTableSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Knights.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\Knights.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\Knights.obj"	"$(INTDIR)\Knights.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\KnightsManager.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\KnightsManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\KnightsManager.obj"	"$(INTDIR)\KnightsManager.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\KnightsRankSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\KnightsRankSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\KnightsRankSet.obj"	"$(INTDIR)\KnightsRankSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\KnightsSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\KnightsSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\KnightsSet.obj"	"$(INTDIR)\KnightsSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\KnightsUserSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\KnightsUserSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\KnightsUserSet.obj"	"$(INTDIR)\KnightsUserSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LevelUpTableSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\LevelUpTableSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\LevelUpTableSet.obj"	"$(INTDIR)\LevelUpTableSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LOGIC_ELSE.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\LOGIC_ELSE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\LOGIC_ELSE.obj"	"$(INTDIR)\LOGIC_ELSE.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MagicProcess.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\MagicProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\MagicProcess.obj"	"$(INTDIR)\MagicProcess.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MagicTableSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\MagicTableSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\MagicTableSet.obj"	"$(INTDIR)\MagicTableSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MagicType1Set.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\MagicType1Set.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\MagicType1Set.obj"	"$(INTDIR)\MagicType1Set.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MagicType2Set.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\MagicType2Set.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\MagicType2Set.obj"	"$(INTDIR)\MagicType2Set.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MagicType3Set.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\MagicType3Set.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\MagicType3Set.obj"	"$(INTDIR)\MagicType3Set.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MagicType4Set.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\MagicType4Set.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\MagicType4Set.obj"	"$(INTDIR)\MagicType4Set.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MagicType5Set.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\MagicType5Set.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\MagicType5Set.obj"	"$(INTDIR)\MagicType5Set.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MagicType8Set.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\MagicType8Set.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\MagicType8Set.obj"	"$(INTDIR)\MagicType8Set.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Map.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Map.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Map.obj"	"$(INTDIR)\Map.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Npc.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Npc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Npc.obj"	"$(INTDIR)\Npc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Region.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Region.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Region.obj"	"$(INTDIR)\Region.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SharedMem.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\SharedMem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\SharedMem.obj"	"$(INTDIR)\SharedMem.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Ebenezer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\UdpSocket.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\UdpSocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\UdpSocket.obj"	"$(INTDIR)\UdpSocket.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\User.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\User.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\User.obj"	"$(INTDIR)\User.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ZoneInfoSet.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\ZoneInfoSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\ZoneInfoSet.obj"	"$(INTDIR)\ZoneInfoSet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Ebenezer.rc

"$(INTDIR)\Ebenezer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\N3Base\N3ShapeMgr.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\N3ShapeMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_3DSERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\N3ShapeMgr.obj"	"$(INTDIR)\N3ShapeMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

