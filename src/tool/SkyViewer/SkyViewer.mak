# Microsoft Developer Studio Generated NMAKE File, Based on SkyViewer.dsp
!IF "$(CFG)" == ""
CFG=SkyViewer - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SkyViewer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SkyViewer - Win32 Release" && "$(CFG)" != "SkyViewer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SkyViewer.mak" CFG="SkyViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SkyViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SkyViewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SkyViewer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\SkyViewer.exe" "$(OUTDIR)\SkyViewer.pch"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\FormViewProperty.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Cloud.obj"
	-@erase "$(INTDIR)\N3ColorChange.obj"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3GERain.obj"
	-@erase "$(INTDIR)\N3GESnow.obj"
	-@erase "$(INTDIR)\N3GlobalEffect.obj"
	-@erase "$(INTDIR)\N3GlobalEffectMng.obj"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Moon.obj"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Sky.obj"
	-@erase "$(INTDIR)\N3SkyMng.obj"
	-@erase "$(INTDIR)\N3Star.obj"
	-@erase "$(INTDIR)\N3Sun.obj"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\SkyViewer.obj"
	-@erase "$(INTDIR)\SkyViewer.pch"
	-@erase "$(INTDIR)\SkyViewer.res"
	-@erase "$(INTDIR)\SkyViewerDoc.obj"
	-@erase "$(INTDIR)\SkyViewerView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\SkyViewer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\SkyViewer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SkyViewer.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SkyViewer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=d3d9.lib d3dx9.lib dxguid.lib  winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SkyViewer.pdb" /machine:I386 /out:"SkyViewer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\FormViewProperty.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\SkyViewer.obj" \
	"$(INTDIR)\SkyViewerDoc.obj" \
	"$(INTDIR)\SkyViewerView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Cloud.obj" \
	"$(INTDIR)\N3ColorChange.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3GERain.obj" \
	"$(INTDIR)\N3GESnow.obj" \
	"$(INTDIR)\N3GlobalEffect.obj" \
	"$(INTDIR)\N3GlobalEffectMng.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3Sky.obj" \
	"$(INTDIR)\N3SkyMng.obj" \
	"$(INTDIR)\N3Star.obj" \
	"$(INTDIR)\N3Sun.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\SkyViewer.res"

".\SkyViewer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\SkyViewer_Debug.exe" "$(OUTDIR)\SkyViewer.pch" "$(OUTDIR)\SkyViewer.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\FormViewProperty.obj"
	-@erase "$(INTDIR)\FormViewProperty.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.sbr"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3AnimKey.sbr"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3Base.sbr"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.sbr"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Camera.sbr"
	-@erase "$(INTDIR)\N3Cloud.obj"
	-@erase "$(INTDIR)\N3Cloud.sbr"
	-@erase "$(INTDIR)\N3ColorChange.obj"
	-@erase "$(INTDIR)\N3ColorChange.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3EngTool.sbr"
	-@erase "$(INTDIR)\N3GERain.obj"
	-@erase "$(INTDIR)\N3GERain.sbr"
	-@erase "$(INTDIR)\N3GESnow.obj"
	-@erase "$(INTDIR)\N3GESnow.sbr"
	-@erase "$(INTDIR)\N3GlobalEffect.obj"
	-@erase "$(INTDIR)\N3GlobalEffect.sbr"
	-@erase "$(INTDIR)\N3GlobalEffectMng.obj"
	-@erase "$(INTDIR)\N3GlobalEffectMng.sbr"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3IMesh.sbr"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Light.sbr"
	-@erase "$(INTDIR)\N3Moon.obj"
	-@erase "$(INTDIR)\N3Moon.sbr"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMesh.sbr"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.sbr"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Shape.sbr"
	-@erase "$(INTDIR)\N3Sky.obj"
	-@erase "$(INTDIR)\N3Sky.sbr"
	-@erase "$(INTDIR)\N3SkyMng.obj"
	-@erase "$(INTDIR)\N3SkyMng.sbr"
	-@erase "$(INTDIR)\N3Star.obj"
	-@erase "$(INTDIR)\N3Star.sbr"
	-@erase "$(INTDIR)\N3Sun.obj"
	-@erase "$(INTDIR)\N3Sun.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3TransformCollision.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\SkyViewer.obj"
	-@erase "$(INTDIR)\SkyViewer.pch"
	-@erase "$(INTDIR)\SkyViewer.res"
	-@erase "$(INTDIR)\SkyViewer.sbr"
	-@erase "$(INTDIR)\SkyViewerDoc.obj"
	-@erase "$(INTDIR)\SkyViewerDoc.sbr"
	-@erase "$(INTDIR)\SkyViewerView.obj"
	-@erase "$(INTDIR)\SkyViewerView.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SkyViewer.bsc"
	-@erase "$(OUTDIR)\SkyViewer_Debug.pdb"
	-@erase ".\SkyViewer_Debug.exe"
	-@erase ".\SkyViewer_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SkyViewer.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\SkyViewer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SkyViewer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\FormViewProperty.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\SkyViewer.sbr" \
	"$(INTDIR)\SkyViewerDoc.sbr" \
	"$(INTDIR)\SkyViewerView.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Cloud.sbr" \
	"$(INTDIR)\N3ColorChange.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3GERain.sbr" \
	"$(INTDIR)\N3GESnow.sbr" \
	"$(INTDIR)\N3GlobalEffect.sbr" \
	"$(INTDIR)\N3GlobalEffectMng.sbr" \
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Moon.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3Sky.sbr" \
	"$(INTDIR)\N3SkyMng.sbr" \
	"$(INTDIR)\N3Star.sbr" \
	"$(INTDIR)\N3Sun.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr"

"$(OUTDIR)\SkyViewer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=d3d9.lib d3dx9d.lib dxguidd.lib  winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SkyViewer_Debug.pdb" /debug /machine:I386 /out:"SkyViewer_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\FormViewProperty.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\SkyViewer.obj" \
	"$(INTDIR)\SkyViewerDoc.obj" \
	"$(INTDIR)\SkyViewerView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Cloud.obj" \
	"$(INTDIR)\N3ColorChange.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3GERain.obj" \
	"$(INTDIR)\N3GESnow.obj" \
	"$(INTDIR)\N3GlobalEffect.obj" \
	"$(INTDIR)\N3GlobalEffectMng.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3Sky.obj" \
	"$(INTDIR)\N3SkyMng.obj" \
	"$(INTDIR)\N3Star.obj" \
	"$(INTDIR)\N3Sun.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\SkyViewer.res"

".\SkyViewer_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SkyViewer.dep")
!INCLUDE "SkyViewer.dep"
!ELSE 
!MESSAGE Warning: cannot find "SkyViewer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SkyViewer - Win32 Release" || "$(CFG)" == "SkyViewer - Win32 Debug"
SOURCE=.\FormViewProperty.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\FormViewProperty.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\FormViewProperty.obj"	"$(INTDIR)\FormViewProperty.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE="..\Widget\PropertyList.cpp"

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\PropertyList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\PropertyList.obj"	"$(INTDIR)\PropertyList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\SkyViewer.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\SkyViewer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\SkyViewer.obj"	"$(INTDIR)\SkyViewer.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SkyViewer.rc

"$(INTDIR)\SkyViewer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\SkyViewerDoc.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\SkyViewerDoc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\SkyViewerDoc.obj"	"$(INTDIR)\SkyViewerDoc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SkyViewerView.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\SkyViewerView.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\SkyViewerView.obj"	"$(INTDIR)\SkyViewerView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\SkyViewer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SkyViewer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SkyViewer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\SkyViewer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\engine\N3Base\BitMapFile.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\BitMapFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AlphaPrimitiveManager.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3AlphaPrimitiveManager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimKey.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3AnimKey.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Base.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3BaseFileAccess.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3BaseFileAccess.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Camera.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Camera.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Cloud.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Cloud.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Cloud.obj"	"$(INTDIR)\N3Cloud.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3ColorChange.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3ColorChange.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3ColorChange.obj"	"$(INTDIR)\N3ColorChange.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Eng.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3EngTool.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3EngTool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GERain.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3GERain.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3GERain.obj"	"$(INTDIR)\N3GERain.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GESnow.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3GESnow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3GESnow.obj"	"$(INTDIR)\N3GESnow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GlobalEffect.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3GlobalEffect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3GlobalEffect.obj"	"$(INTDIR)\N3GlobalEffect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GlobalEffectMng.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3GlobalEffectMng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3GlobalEffectMng.obj"	"$(INTDIR)\N3GlobalEffectMng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3IMesh.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3IMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Light.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Light.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Moon.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Moon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Moon.obj"	"$(INTDIR)\N3Moon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMesh.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3PMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshInstance.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3PMeshInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Shape.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Shape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Sky.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Sky.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Sky.obj"	"$(INTDIR)\N3Sky.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3SkyMng.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3SkyMng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3SkyMng.obj"	"$(INTDIR)\N3SkyMng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Star.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Star.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Star.obj"	"$(INTDIR)\N3Star.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Sun.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Sun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Sun.obj"	"$(INTDIR)\N3Sun.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Texture.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Texture.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Transform.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3Transform.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3TransformCollision.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3TransformCollision.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3VMesh.cpp

!IF  "$(CFG)" == "SkyViewer - Win32 Release"


"$(INTDIR)\N3VMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"


"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

