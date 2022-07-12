# Microsoft Developer Studio Generated NMAKE File, Based on KscViewer.dsp
!IF "$(CFG)" == ""
CFG=KscViewer - Win32 Debug
!MESSAGE No configuration specified. Defaulting to KscViewer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "KscViewer - Win32 Release" && "$(CFG)" != "KscViewer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KscViewer.mak" CFG="KscViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KscViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KscViewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "KscViewer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\KscViewer.exe"


CLEAN :
	-@erase "$(INTDIR)\JpegFile.obj"
	-@erase "$(INTDIR)\KscViewer.obj"
	-@erase "$(INTDIR)\KscViewer.pch"
	-@erase "$(INTDIR)\KscViewer.res"
	-@erase "$(INTDIR)\KscViewerDoc.obj"
	-@erase "$(INTDIR)\KscViewerView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\N3JpegFile.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\KscViewer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\KscViewer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\KscViewer.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KscViewer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=..\..\engine\JPEG\jpegLib.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\KscViewer.pdb" /machine:I386 /out:"KscViewer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\JpegFile.obj" \
	"$(INTDIR)\KscViewer.obj" \
	"$(INTDIR)\KscViewerDoc.obj" \
	"$(INTDIR)\KscViewerView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3JpegFile.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\KscViewer.res"

".\KscViewer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "KscViewer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\KscViewer_Debug.exe" "$(OUTDIR)\KscViewer.bsc"


CLEAN :
	-@erase "$(INTDIR)\JpegFile.obj"
	-@erase "$(INTDIR)\JpegFile.sbr"
	-@erase "$(INTDIR)\KscViewer.obj"
	-@erase "$(INTDIR)\KscViewer.pch"
	-@erase "$(INTDIR)\KscViewer.res"
	-@erase "$(INTDIR)\KscViewer.sbr"
	-@erase "$(INTDIR)\KscViewerDoc.obj"
	-@erase "$(INTDIR)\KscViewerDoc.sbr"
	-@erase "$(INTDIR)\KscViewerView.obj"
	-@erase "$(INTDIR)\KscViewerView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\N3JpegFile.obj"
	-@erase "$(INTDIR)\N3JpegFile.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\KscViewer.bsc"
	-@erase "$(OUTDIR)\KscViewer_Debug.pdb"
	-@erase ".\KscViewer_Debug.exe"
	-@erase ".\KscViewer_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\KscViewer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\KscViewer.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KscViewer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\JpegFile.sbr" \
	"$(INTDIR)\KscViewer.sbr" \
	"$(INTDIR)\KscViewerDoc.sbr" \
	"$(INTDIR)\KscViewerView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3JpegFile.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\KscViewer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\..\engine\JPEG\jpegLib.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\KscViewer_Debug.pdb" /debug /machine:I386 /out:"KscViewer_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\JpegFile.obj" \
	"$(INTDIR)\KscViewer.obj" \
	"$(INTDIR)\KscViewerDoc.obj" \
	"$(INTDIR)\KscViewerView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3JpegFile.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\KscViewer.res"

".\KscViewer_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("KscViewer.dep")
!INCLUDE "KscViewer.dep"
!ELSE 
!MESSAGE Warning: cannot find "KscViewer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "KscViewer - Win32 Release" || "$(CFG)" == "KscViewer - Win32 Debug"
SOURCE=..\..\engine\JPEG\JpegFile.cpp

!IF  "$(CFG)" == "KscViewer - Win32 Release"


"$(INTDIR)\JpegFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "KscViewer - Win32 Debug"


"$(INTDIR)\JpegFile.obj"	"$(INTDIR)\JpegFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\KscViewer.cpp

!IF  "$(CFG)" == "KscViewer - Win32 Release"


"$(INTDIR)\KscViewer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ELSEIF  "$(CFG)" == "KscViewer - Win32 Debug"


"$(INTDIR)\KscViewer.obj"	"$(INTDIR)\KscViewer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ENDIF 

SOURCE=.\KscViewer.rc

"$(INTDIR)\KscViewer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\KscViewerDoc.cpp

!IF  "$(CFG)" == "KscViewer - Win32 Release"


"$(INTDIR)\KscViewerDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ELSEIF  "$(CFG)" == "KscViewer - Win32 Debug"


"$(INTDIR)\KscViewerDoc.obj"	"$(INTDIR)\KscViewerDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ENDIF 

SOURCE=.\KscViewerView.cpp

!IF  "$(CFG)" == "KscViewer - Win32 Release"


"$(INTDIR)\KscViewerView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ELSEIF  "$(CFG)" == "KscViewer - Win32 Debug"


"$(INTDIR)\KscViewerView.obj"	"$(INTDIR)\KscViewerView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "KscViewer - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ELSEIF  "$(CFG)" == "KscViewer - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ENDIF 

SOURCE=.\N3JpegFile.cpp

!IF  "$(CFG)" == "KscViewer - Win32 Release"


"$(INTDIR)\N3JpegFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ELSEIF  "$(CFG)" == "KscViewer - Win32 Debug"


"$(INTDIR)\N3JpegFile.obj"	"$(INTDIR)\N3JpegFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\KscViewer.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "KscViewer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\KscViewer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\KscViewer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "KscViewer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\KscViewer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\KscViewer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

