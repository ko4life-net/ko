# Microsoft Developer Studio Generated NMAKE File, Based on N3TexViewer.dsp
!IF "$(CFG)" == ""
CFG=N3TexViewer - Win32 Debug
!MESSAGE No configuration specified. Defaulting to N3TexViewer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "N3TexViewer - Win32 Release" && "$(CFG)" != "N3TexViewer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3TexViewer.mak" CFG="N3TexViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3TexViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3TexViewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\N3TexViewer.exe"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\DlgFormat.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3TexViewer.obj"
	-@erase "$(INTDIR)\N3TexViewer.res"
	-@erase "$(INTDIR)\N3TexViewerDoc.obj"
	-@erase "$(INTDIR)\N3TexViewerView.obj"
	-@erase "$(INTDIR)\ProgressBar.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\N3TexViewer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\N3TexViewer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /D "_N3TOOL" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3TexViewer.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3TexViewer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=d3d9.lib d3dx9.lib dxguid.lib dsound.lib winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\N3TexViewer.pdb" /machine:I386 /force /out:"N3TexViewer.exe" /force:multiple 
LINK32_OBJS= \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DlgFormat.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3TexViewer.obj" \
	"$(INTDIR)\N3TexViewerDoc.obj" \
	"$(INTDIR)\N3TexViewerView.obj" \
	"$(INTDIR)\ProgressBar.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\N3TexViewer.res"

".\N3TexViewer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\N3TexViewer_Debug.exe" "$(OUTDIR)\N3TexViewer.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\DlgFormat.obj"
	-@erase "$(INTDIR)\DlgFormat.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.sbr"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3Base.sbr"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3EngTool.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3TexViewer.obj"
	-@erase "$(INTDIR)\N3TexViewer.res"
	-@erase "$(INTDIR)\N3TexViewer.sbr"
	-@erase "$(INTDIR)\N3TexViewerDoc.obj"
	-@erase "$(INTDIR)\N3TexViewerDoc.sbr"
	-@erase "$(INTDIR)\N3TexViewerView.obj"
	-@erase "$(INTDIR)\N3TexViewerView.sbr"
	-@erase "$(INTDIR)\ProgressBar.obj"
	-@erase "$(INTDIR)\ProgressBar.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\N3TexViewer.bsc"
	-@erase "$(OUTDIR)\N3TexViewer_Debug.pdb"
	-@erase ".\N3TexViewer_Debug.exe"
	-@erase ".\N3TexViewer_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3TexViewer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /D "_N3TOOL" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3TexViewer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3TexViewer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\DlgFormat.sbr" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3TexViewer.sbr" \
	"$(INTDIR)\N3TexViewerDoc.sbr" \
	"$(INTDIR)\N3TexViewerView.sbr" \
	"$(INTDIR)\ProgressBar.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\N3TexViewer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=d3d9.lib d3dx9d.lib dxguidd.lib dsound.lib winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\N3TexViewer_Debug.pdb" /debug /machine:I386 /out:"N3TexViewer_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DlgFormat.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3TexViewer.obj" \
	"$(INTDIR)\N3TexViewerDoc.obj" \
	"$(INTDIR)\N3TexViewerView.obj" \
	"$(INTDIR)\ProgressBar.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\N3TexViewer.res"

".\N3TexViewer_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("N3TexViewer.dep")
!INCLUDE "N3TexViewer.dep"
!ELSE 
!MESSAGE Warning: cannot find "N3TexViewer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "N3TexViewer - Win32 Release" || "$(CFG)" == "N3TexViewer - Win32 Debug"
SOURCE=..\..\engine\N3Base\BitMapFile.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\BitMapFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\DlgFormat.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\DlgFormat.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\DlgFormat.obj"	"$(INTDIR)\DlgFormat.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\LogWriter.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\LogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\LogWriter.obj"	"$(INTDIR)\LogWriter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AlphaPrimitiveManager.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3AlphaPrimitiveManager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Base.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3BaseFileAccess.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3BaseFileAccess.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Eng.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3EngTool.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3EngTool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Texture.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3Texture.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\N3TexViewer.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3TexViewer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3TexViewer.obj"	"$(INTDIR)\N3TexViewer.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3TexViewer.rc

"$(INTDIR)\N3TexViewer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\N3TexViewerDoc.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3TexViewerDoc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3TexViewerDoc.obj"	"$(INTDIR)\N3TexViewerDoc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3TexViewerView.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\N3TexViewerView.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\N3TexViewerView.obj"	"$(INTDIR)\N3TexViewerView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ProgressBar.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\ProgressBar.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\ProgressBar.obj"	"$(INTDIR)\ProgressBar.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"


"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

