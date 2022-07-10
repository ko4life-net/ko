# Microsoft Developer Studio Generated NMAKE File, Based on ServerInfoViewer.dsp
!IF "$(CFG)" == ""
CFG=ServerInfoViewer - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ServerInfoViewer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ServerInfoViewer - Win32 Release" && "$(CFG)" != "ServerInfoViewer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ServerInfoViewer.mak" CFG="ServerInfoViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServerInfoViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ServerInfoViewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ServerInfoViewer.exe"


CLEAN :
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AnimControl.obj"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\ServerInfoViewer.obj"
	-@erase "$(INTDIR)\ServerInfoViewer.pch"
	-@erase "$(INTDIR)\ServerInfoViewer.res"
	-@erase "$(INTDIR)\ServerInfoViewerDoc.obj"
	-@erase "$(INTDIR)\ServerInfoViewerView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ServerInfoViewer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D _N3TOOL /Fp"$(INTDIR)\ServerInfoViewer.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ServerInfoViewer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ServerInfoViewer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ServerInfoViewer.pdb" /machine:I386 /out:"$(OUTDIR)\ServerInfoViewer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\ServerInfoViewer.obj" \
	"$(INTDIR)\ServerInfoViewerDoc.obj" \
	"$(INTDIR)\ServerInfoViewerView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ServerInfoViewer.res"

"$(OUTDIR)\ServerInfoViewer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ServerInfoViewer.exe" "$(OUTDIR)\ServerInfoViewer.pch" "$(OUTDIR)\ServerInfoViewer.bsc"


CLEAN :
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.sbr"
	-@erase "$(INTDIR)\N3AnimControl.obj"
	-@erase "$(INTDIR)\N3AnimControl.sbr"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3AnimKey.sbr"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3Base.sbr"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.sbr"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Camera.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3EngTool.sbr"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Light.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\ServerInfoViewer.obj"
	-@erase "$(INTDIR)\ServerInfoViewer.pch"
	-@erase "$(INTDIR)\ServerInfoViewer.res"
	-@erase "$(INTDIR)\ServerInfoViewer.sbr"
	-@erase "$(INTDIR)\ServerInfoViewerDoc.obj"
	-@erase "$(INTDIR)\ServerInfoViewerDoc.sbr"
	-@erase "$(INTDIR)\ServerInfoViewerView.obj"
	-@erase "$(INTDIR)\ServerInfoViewerView.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ServerInfoViewer.bsc"
	-@erase "$(OUTDIR)\ServerInfoViewer.exe"
	-@erase "$(OUTDIR)\ServerInfoViewer.ilk"
	-@erase "$(OUTDIR)\ServerInfoViewer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ServerInfoViewer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ServerInfoViewer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ServerInfoViewer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\ServerInfoViewer.sbr" \
	"$(INTDIR)\ServerInfoViewerDoc.sbr" \
	"$(INTDIR)\ServerInfoViewerView.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\ServerInfoViewer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ServerInfoViewer.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ServerInfoViewer.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\ServerInfoViewer.obj" \
	"$(INTDIR)\ServerInfoViewerDoc.obj" \
	"$(INTDIR)\ServerInfoViewerView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ServerInfoViewer.res"

"$(OUTDIR)\ServerInfoViewer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ServerInfoViewer.dep")
!INCLUDE "ServerInfoViewer.dep"
!ELSE 
!MESSAGE Warning: cannot find "ServerInfoViewer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ServerInfoViewer - Win32 Release" || "$(CFG)" == "ServerInfoViewer - Win32 Debug"
SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\N3Base\N3AlphaPrimitiveManager.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3AlphaPrimitiveManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3AnimControl.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3AnimControl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3AnimKey.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3AnimKey.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3Base.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3BaseFileAccess.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3BaseFileAccess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3Camera.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3Camera.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3Eng.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3EngTool.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3EngTool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3Light.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3Light.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\N3Base\N3Transform.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\N3Transform.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ServerInfoViewer.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\ServerInfoViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\ServerInfoViewer.obj"	"$(INTDIR)\ServerInfoViewer.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ServerInfoViewer.rc

"$(INTDIR)\ServerInfoViewer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ServerInfoViewerDoc.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\ServerInfoViewerDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\ServerInfoViewerDoc.obj"	"$(INTDIR)\ServerInfoViewerDoc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ServerInfoViewerView.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"


"$(INTDIR)\ServerInfoViewerView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ServerInfoViewer.pch"


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"


"$(INTDIR)\ServerInfoViewerView.obj"	"$(INTDIR)\ServerInfoViewerView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ServerInfoViewer - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ServerInfoViewer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ServerInfoViewer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ServerInfoViewer - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ServerInfoViewer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ServerInfoViewer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

