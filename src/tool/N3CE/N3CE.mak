# Microsoft Developer Studio Generated NMAKE File, Based on N3CE.dsp
!IF "$(CFG)" == ""
CFG=N3CE - Win32 Debug
!MESSAGE No configuration specified. Defaulting to N3CE - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "N3CE - Win32 Release" && "$(CFG)" != "N3CE - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3CE.mak" CFG="N3CE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3CE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3CE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "N3CE - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\N3CE.exe"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\FormViewAnimation.obj"
	-@erase "$(INTDIR)\FormViewProperty.obj"
	-@erase "$(INTDIR)\FormViewTool.obj"
	-@erase "$(INTDIR)\MacSliderCtrl.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AnimControl.obj"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3Board.obj"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3CE.obj"
	-@erase "$(INTDIR)\N3CE.res"
	-@erase "$(INTDIR)\N3CEDoc.obj"
	-@erase "$(INTDIR)\N3CEView.obj"
	-@erase "$(INTDIR)\N3Chr.obj"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3FXBundle.obj"
	-@erase "$(INTDIR)\N3FXPartBase.obj"
	-@erase "$(INTDIR)\N3FXPartBillBoard.obj"
	-@erase "$(INTDIR)\N3FXPartBottomBoard.obj"
	-@erase "$(INTDIR)\N3FXParticle.obj"
	-@erase "$(INTDIR)\N3FXPartMesh.obj"
	-@erase "$(INTDIR)\N3FXPartParticles.obj"
	-@erase "$(INTDIR)\N3FXPlug.obj"
	-@erase "$(INTDIR)\N3FXPMesh.obj"
	-@erase "$(INTDIR)\N3FXPMeshInstance.obj"
	-@erase "$(INTDIR)\N3FXShape.obj"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3Joint.obj"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3ShapeExtra.obj"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3SndMgr.obj"
	-@erase "$(INTDIR)\N3SndObj.obj"
	-@erase "$(INTDIR)\N3SndObjStream.obj"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\PosDummy.obj"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TransDummy.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WaveFile.obj"
	-@erase ".\N3CE.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\N3CE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3CE.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3CE.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=d3d9.lib d3dx9.lib dsound.lib dxguid.lib winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\N3CE.pdb" /machine:I386 /force /out:"N3CE.exe" 
LINK32_OBJS= \
	"$(INTDIR)\FormViewAnimation.obj" \
	"$(INTDIR)\FormViewProperty.obj" \
	"$(INTDIR)\FormViewTool.obj" \
	"$(INTDIR)\MacSliderCtrl.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3CE.obj" \
	"$(INTDIR)\N3CEDoc.obj" \
	"$(INTDIR)\N3CEView.obj" \
	"$(INTDIR)\Pick.obj" \
	"$(INTDIR)\PosDummy.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TransDummy.obj" \
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXPartBase.obj" \
	"$(INTDIR)\N3FXPartBillBoard.obj" \
	"$(INTDIR)\N3FXPartBottomBoard.obj" \
	"$(INTDIR)\N3FXParticle.obj" \
	"$(INTDIR)\N3FXPartMesh.obj" \
	"$(INTDIR)\N3FXPartParticles.obj" \
	"$(INTDIR)\N3FXPMesh.obj" \
	"$(INTDIR)\N3FXPMeshInstance.obj" \
	"$(INTDIR)\N3FXShape.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Board.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3FXPlug.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3ShapeExtra.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3SndMgr.obj" \
	"$(INTDIR)\N3SndObj.obj" \
	"$(INTDIR)\N3SndObjStream.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\WaveFile.obj" \
	"$(INTDIR)\N3CE.res"

".\N3CE.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\N3CE_Debug.exe" "$(OUTDIR)\N3CE.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\FormViewAnimation.obj"
	-@erase "$(INTDIR)\FormViewAnimation.sbr"
	-@erase "$(INTDIR)\FormViewProperty.obj"
	-@erase "$(INTDIR)\FormViewProperty.sbr"
	-@erase "$(INTDIR)\FormViewTool.obj"
	-@erase "$(INTDIR)\FormViewTool.sbr"
	-@erase "$(INTDIR)\MacSliderCtrl.obj"
	-@erase "$(INTDIR)\MacSliderCtrl.sbr"
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
	-@erase "$(INTDIR)\N3Board.obj"
	-@erase "$(INTDIR)\N3Board.sbr"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Camera.sbr"
	-@erase "$(INTDIR)\N3CE.obj"
	-@erase "$(INTDIR)\N3CE.res"
	-@erase "$(INTDIR)\N3CE.sbr"
	-@erase "$(INTDIR)\N3CEDoc.obj"
	-@erase "$(INTDIR)\N3CEDoc.sbr"
	-@erase "$(INTDIR)\N3CEView.obj"
	-@erase "$(INTDIR)\N3CEView.sbr"
	-@erase "$(INTDIR)\N3Chr.obj"
	-@erase "$(INTDIR)\N3Chr.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3EngTool.sbr"
	-@erase "$(INTDIR)\N3FXBundle.obj"
	-@erase "$(INTDIR)\N3FXBundle.sbr"
	-@erase "$(INTDIR)\N3FXPartBase.obj"
	-@erase "$(INTDIR)\N3FXPartBase.sbr"
	-@erase "$(INTDIR)\N3FXPartBillBoard.obj"
	-@erase "$(INTDIR)\N3FXPartBillBoard.sbr"
	-@erase "$(INTDIR)\N3FXPartBottomBoard.obj"
	-@erase "$(INTDIR)\N3FXPartBottomBoard.sbr"
	-@erase "$(INTDIR)\N3FXParticle.obj"
	-@erase "$(INTDIR)\N3FXParticle.sbr"
	-@erase "$(INTDIR)\N3FXPartMesh.obj"
	-@erase "$(INTDIR)\N3FXPartMesh.sbr"
	-@erase "$(INTDIR)\N3FXPartParticles.obj"
	-@erase "$(INTDIR)\N3FXPartParticles.sbr"
	-@erase "$(INTDIR)\N3FXPlug.obj"
	-@erase "$(INTDIR)\N3FXPlug.sbr"
	-@erase "$(INTDIR)\N3FXPMesh.obj"
	-@erase "$(INTDIR)\N3FXPMesh.sbr"
	-@erase "$(INTDIR)\N3FXPMeshInstance.obj"
	-@erase "$(INTDIR)\N3FXPMeshInstance.sbr"
	-@erase "$(INTDIR)\N3FXShape.obj"
	-@erase "$(INTDIR)\N3FXShape.sbr"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3IMesh.sbr"
	-@erase "$(INTDIR)\N3Joint.obj"
	-@erase "$(INTDIR)\N3Joint.sbr"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Light.sbr"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3Mesh.sbr"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMesh.sbr"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.sbr"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.sbr"
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Scene.sbr"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Shape.sbr"
	-@erase "$(INTDIR)\N3ShapeExtra.obj"
	-@erase "$(INTDIR)\N3ShapeExtra.sbr"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Skin.sbr"
	-@erase "$(INTDIR)\N3SndMgr.obj"
	-@erase "$(INTDIR)\N3SndMgr.sbr"
	-@erase "$(INTDIR)\N3SndObj.obj"
	-@erase "$(INTDIR)\N3SndObj.sbr"
	-@erase "$(INTDIR)\N3SndObjStream.obj"
	-@erase "$(INTDIR)\N3SndObjStream.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3TransformCollision.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\Pick.sbr"
	-@erase "$(INTDIR)\PosDummy.obj"
	-@erase "$(INTDIR)\PosDummy.sbr"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TransDummy.obj"
	-@erase "$(INTDIR)\TransDummy.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WaveFile.obj"
	-@erase "$(INTDIR)\WaveFile.sbr"
	-@erase "$(OUTDIR)\N3CE.bsc"
	-@erase "$(OUTDIR)\N3CE_Debug.pdb"
	-@erase ".\N3CE_Debug.exe"
	-@erase ".\N3CE_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3CE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3CE.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3CE.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\FormViewAnimation.sbr" \
	"$(INTDIR)\FormViewProperty.sbr" \
	"$(INTDIR)\FormViewTool.sbr" \
	"$(INTDIR)\MacSliderCtrl.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3CE.sbr" \
	"$(INTDIR)\N3CEDoc.sbr" \
	"$(INTDIR)\N3CEView.sbr" \
	"$(INTDIR)\Pick.sbr" \
	"$(INTDIR)\PosDummy.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TransDummy.sbr" \
	"$(INTDIR)\N3FXBundle.sbr" \
	"$(INTDIR)\N3FXPartBase.sbr" \
	"$(INTDIR)\N3FXPartBillBoard.sbr" \
	"$(INTDIR)\N3FXPartBottomBoard.sbr" \
	"$(INTDIR)\N3FXParticle.sbr" \
	"$(INTDIR)\N3FXPartMesh.sbr" \
	"$(INTDIR)\N3FXPartParticles.sbr" \
	"$(INTDIR)\N3FXPMesh.sbr" \
	"$(INTDIR)\N3FXPMeshInstance.sbr" \
	"$(INTDIR)\N3FXShape.sbr" \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Board.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Chr.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3FXPlug.sbr" \
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Joint.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Mesh.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshCreate.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3Scene.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3ShapeExtra.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\N3SndMgr.sbr" \
	"$(INTDIR)\N3SndObj.sbr" \
	"$(INTDIR)\N3SndObjStream.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr" \
	"$(INTDIR)\WaveFile.sbr"

"$(OUTDIR)\N3CE.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=d3d9.lib d3dx9d.lib dsound.lib dxguidd.lib winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\N3CE_Debug.pdb" /debug /machine:I386 /out:"N3CE_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\FormViewAnimation.obj" \
	"$(INTDIR)\FormViewProperty.obj" \
	"$(INTDIR)\FormViewTool.obj" \
	"$(INTDIR)\MacSliderCtrl.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3CE.obj" \
	"$(INTDIR)\N3CEDoc.obj" \
	"$(INTDIR)\N3CEView.obj" \
	"$(INTDIR)\Pick.obj" \
	"$(INTDIR)\PosDummy.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TransDummy.obj" \
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXPartBase.obj" \
	"$(INTDIR)\N3FXPartBillBoard.obj" \
	"$(INTDIR)\N3FXPartBottomBoard.obj" \
	"$(INTDIR)\N3FXParticle.obj" \
	"$(INTDIR)\N3FXPartMesh.obj" \
	"$(INTDIR)\N3FXPartParticles.obj" \
	"$(INTDIR)\N3FXPMesh.obj" \
	"$(INTDIR)\N3FXPMeshInstance.obj" \
	"$(INTDIR)\N3FXShape.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Board.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3FXPlug.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3ShapeExtra.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3SndMgr.obj" \
	"$(INTDIR)\N3SndObj.obj" \
	"$(INTDIR)\N3SndObjStream.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\WaveFile.obj" \
	"$(INTDIR)\N3CE.res"

".\N3CE_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("N3CE.dep")
!INCLUDE "N3CE.dep"
!ELSE 
!MESSAGE Warning: cannot find "N3CE.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "N3CE - Win32 Release" || "$(CFG)" == "N3CE - Win32 Debug"
SOURCE=.\FormViewAnimation.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\FormViewAnimation.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\FormViewAnimation.obj"	"$(INTDIR)\FormViewAnimation.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\FormViewProperty.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\FormViewProperty.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\FormViewProperty.obj"	"$(INTDIR)\FormViewProperty.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\FormViewTool.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\FormViewTool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\FormViewTool.obj"	"$(INTDIR)\FormViewTool.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MacSliderCtrl.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\MacSliderCtrl.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\MacSliderCtrl.obj"	"$(INTDIR)\MacSliderCtrl.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3CE.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3CE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3CE.obj"	"$(INTDIR)\N3CE.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3CE.rc

"$(INTDIR)\N3CE.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\N3CEDoc.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3CEDoc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3CEDoc.obj"	"$(INTDIR)\N3CEDoc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3CEView.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3CEView.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3CEView.obj"	"$(INTDIR)\N3CEView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\Pick.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\Pick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\Pick.obj"	"$(INTDIR)\Pick.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\PosDummy.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\PosDummy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\PosDummy.obj"	"$(INTDIR)\PosDummy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE="..\Widget\PropertyList.cpp"

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\PropertyList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\PropertyList.obj"	"$(INTDIR)\PropertyList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TransDummy.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\TransDummy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\TransDummy.obj"	"$(INTDIR)\TransDummy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXBundle.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXBundle.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXBundle.obj"	"$(INTDIR)\N3FXBundle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBase.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXPartBase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXPartBase.obj"	"$(INTDIR)\N3FXPartBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBillBoard.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXPartBillBoard.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXPartBillBoard.obj"	"$(INTDIR)\N3FXPartBillBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBottomBoard.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXPartBottomBoard.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXPartBottomBoard.obj"	"$(INTDIR)\N3FXPartBottomBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXParticle.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXParticle.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXParticle.obj"	"$(INTDIR)\N3FXParticle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartMesh.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXPartMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXPartMesh.obj"	"$(INTDIR)\N3FXPartMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartParticles.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXPartParticles.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXPartParticles.obj"	"$(INTDIR)\N3FXPartParticles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPMesh.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXPMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXPMesh.obj"	"$(INTDIR)\N3FXPMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPMeshInstance.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXPMeshInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXPMeshInstance.obj"	"$(INTDIR)\N3FXPMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXShape.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXShape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXShape.obj"	"$(INTDIR)\N3FXShape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\BitMapFile.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\BitMapFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AlphaPrimitiveManager.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\N3CE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\N3AlphaPrimitiveManager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3CE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimControl.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3AnimControl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimKey.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3AnimKey.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Base.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3BaseFileAccess.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\N3CE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\N3BaseFileAccess.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3CE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Board.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Board.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Board.obj"	"$(INTDIR)\N3Board.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Camera.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Camera.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Chr.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Chr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Chr.obj"	"$(INTDIR)\N3Chr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Eng.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3EngTool.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3EngTool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPlug.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3FXPlug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3FXPlug.obj"	"$(INTDIR)\N3FXPlug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3IMesh.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3IMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Joint.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Joint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Joint.obj"	"$(INTDIR)\N3Joint.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Light.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Light.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Mesh.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Mesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Mesh.obj"	"$(INTDIR)\N3Mesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMesh.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3PMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshCreate.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3PMeshCreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3PMeshCreate.obj"	"$(INTDIR)\N3PMeshCreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshInstance.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3PMeshInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Scene.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Scene.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Scene.obj"	"$(INTDIR)\N3Scene.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Shape.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Shape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3ShapeExtra.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3ShapeExtra.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3ShapeExtra.obj"	"$(INTDIR)\N3ShapeExtra.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Skin.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Skin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Skin.obj"	"$(INTDIR)\N3Skin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3SndMgr.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3SndMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3SndMgr.obj"	"$(INTDIR)\N3SndMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3SndObj.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3SndObj.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3SndObj.obj"	"$(INTDIR)\N3SndObj.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3SndObjStream.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3SndObjStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3SndObjStream.obj"	"$(INTDIR)\N3SndObjStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Texture.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Texture.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Transform.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3Transform.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3TransformCollision.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3TransformCollision.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3VMesh.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\N3VMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\WaveFile.cpp

!IF  "$(CFG)" == "N3CE - Win32 Release"


"$(INTDIR)\WaveFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3CE - Win32 Debug"


"$(INTDIR)\WaveFile.obj"	"$(INTDIR)\WaveFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

