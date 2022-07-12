# Microsoft Developer Studio Generated NMAKE File, Based on N3FXE.dsp
!IF "$(CFG)" == ""
CFG=N3FXE - Win32 Debug
!MESSAGE No configuration specified. Defaulting to N3FXE - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "N3FXE - Win32 Release" && "$(CFG)" != "N3FXE - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3FXE.mak" CFG="N3FXE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3FXE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3FXE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "N3FXE - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\N3FXE.exe" "$(OUTDIR)\N3FXE.pch"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\DFont.obj"
	-@erase "$(INTDIR)\DlgEditFxg.obj"
	-@erase "$(INTDIR)\DlgEditPartBillBoard.obj"
	-@erase "$(INTDIR)\DlgEditPartGround.obj"
	-@erase "$(INTDIR)\DlgEditPartMesh.obj"
	-@erase "$(INTDIR)\DlgEditPartParticle.obj"
	-@erase "$(INTDIR)\DlgEditScript.obj"
	-@erase "$(INTDIR)\DlgNewFileName.obj"
	-@erase "$(INTDIR)\DlgNewPart.obj"
	-@erase "$(INTDIR)\DlgPercent.obj"
	-@erase "$(INTDIR)\DlgPutColor.obj"
	-@erase "$(INTDIR)\Ground.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AnimControl.obj"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3Board.obj"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Chr.obj"
	-@erase "$(INTDIR)\N3Cloak.obj"
	-@erase "$(INTDIR)\N3Cloud.obj"
	-@erase "$(INTDIR)\N3ColorChange.obj"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3FXBundle.obj"
	-@erase "$(INTDIR)\N3FXE.obj"
	-@erase "$(INTDIR)\N3FXE.pch"
	-@erase "$(INTDIR)\N3FXE.res"
	-@erase "$(INTDIR)\N3FXEDoc.obj"
	-@erase "$(INTDIR)\N3FXEView.obj"
	-@erase "$(INTDIR)\N3FXGroup.obj"
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
	-@erase "$(INTDIR)\N3GERain.obj"
	-@erase "$(INTDIR)\N3GESnow.obj"
	-@erase "$(INTDIR)\N3GlobalEffect.obj"
	-@erase "$(INTDIR)\N3GlobalEffectMng.obj"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3Joint.obj"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3Moon.obj"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3River2.obj"
	-@erase "$(INTDIR)\N3RiverPatch.obj"
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3ShapeMod.obj"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Sky.obj"
	-@erase "$(INTDIR)\N3SndMgr.obj"
	-@erase "$(INTDIR)\N3SndObj.obj"
	-@erase "$(INTDIR)\N3SndObjStream.obj"
	-@erase "$(INTDIR)\N3Star.obj"
	-@erase "$(INTDIR)\N3Sun.obj"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WaveFile.obj"
	-@erase ".\N3FXE.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\N3FXE.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3FXE.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3FXE.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib d3d8.lib d3dx8.lib dsound.lib dxguid.lib strmiids.lib dxerr8.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\N3FXE.pdb" /machine:I386 /out:"N3FXE.exe" 
LINK32_OBJS= \
	"$(INTDIR)\N3FXE.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DFont.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Board.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Cloak.obj" \
	"$(INTDIR)\N3Cloud.obj" \
	"$(INTDIR)\N3ColorChange.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3FXPlug.obj" \
	"$(INTDIR)\N3GERain.obj" \
	"$(INTDIR)\N3GESnow.obj" \
	"$(INTDIR)\N3GlobalEffect.obj" \
	"$(INTDIR)\N3GlobalEffectMng.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3River2.obj" \
	"$(INTDIR)\N3RiverPatch.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3ShapeMod.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Sky.obj" \
	"$(INTDIR)\N3SndMgr.obj" \
	"$(INTDIR)\N3SndObj.obj" \
	"$(INTDIR)\N3SndObjStream.obj" \
	"$(INTDIR)\N3Star.obj" \
	"$(INTDIR)\N3Sun.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Pick.obj" \
	"$(INTDIR)\WaveFile.obj" \
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXGroup.obj" \
	"$(INTDIR)\N3FXPartBase.obj" \
	"$(INTDIR)\N3FXPartBillBoard.obj" \
	"$(INTDIR)\N3FXPartBottomBoard.obj" \
	"$(INTDIR)\N3FXParticle.obj" \
	"$(INTDIR)\N3FXPartMesh.obj" \
	"$(INTDIR)\N3FXPartParticles.obj" \
	"$(INTDIR)\N3FXPMesh.obj" \
	"$(INTDIR)\N3FXPMeshInstance.obj" \
	"$(INTDIR)\N3FXShape.obj" \
	"$(INTDIR)\DlgEditFxg.obj" \
	"$(INTDIR)\DlgEditPartBillBoard.obj" \
	"$(INTDIR)\DlgEditPartGround.obj" \
	"$(INTDIR)\DlgEditPartMesh.obj" \
	"$(INTDIR)\DlgEditPartParticle.obj" \
	"$(INTDIR)\DlgEditScript.obj" \
	"$(INTDIR)\DlgNewFileName.obj" \
	"$(INTDIR)\DlgNewPart.obj" \
	"$(INTDIR)\DlgPercent.obj" \
	"$(INTDIR)\DlgPutColor.obj" \
	"$(INTDIR)\Ground.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3FXEDoc.obj" \
	"$(INTDIR)\N3FXEView.obj" \
	"$(INTDIR)\N3FXE.res"

".\N3FXE.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\N3FXE_Debug.exe" "$(OUTDIR)\N3FXE.pch" "$(OUTDIR)\N3FXE.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\DFont.obj"
	-@erase "$(INTDIR)\DFont.sbr"
	-@erase "$(INTDIR)\DlgEditFxg.obj"
	-@erase "$(INTDIR)\DlgEditFxg.sbr"
	-@erase "$(INTDIR)\DlgEditPartBillBoard.obj"
	-@erase "$(INTDIR)\DlgEditPartBillBoard.sbr"
	-@erase "$(INTDIR)\DlgEditPartGround.obj"
	-@erase "$(INTDIR)\DlgEditPartGround.sbr"
	-@erase "$(INTDIR)\DlgEditPartMesh.obj"
	-@erase "$(INTDIR)\DlgEditPartMesh.sbr"
	-@erase "$(INTDIR)\DlgEditPartParticle.obj"
	-@erase "$(INTDIR)\DlgEditPartParticle.sbr"
	-@erase "$(INTDIR)\DlgEditScript.obj"
	-@erase "$(INTDIR)\DlgEditScript.sbr"
	-@erase "$(INTDIR)\DlgNewFileName.obj"
	-@erase "$(INTDIR)\DlgNewFileName.sbr"
	-@erase "$(INTDIR)\DlgNewPart.obj"
	-@erase "$(INTDIR)\DlgNewPart.sbr"
	-@erase "$(INTDIR)\DlgPercent.obj"
	-@erase "$(INTDIR)\DlgPercent.sbr"
	-@erase "$(INTDIR)\DlgPutColor.obj"
	-@erase "$(INTDIR)\DlgPutColor.sbr"
	-@erase "$(INTDIR)\Ground.obj"
	-@erase "$(INTDIR)\Ground.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
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
	-@erase "$(INTDIR)\N3Chr.obj"
	-@erase "$(INTDIR)\N3Chr.sbr"
	-@erase "$(INTDIR)\N3Cloak.obj"
	-@erase "$(INTDIR)\N3Cloak.sbr"
	-@erase "$(INTDIR)\N3Cloud.obj"
	-@erase "$(INTDIR)\N3Cloud.sbr"
	-@erase "$(INTDIR)\N3ColorChange.obj"
	-@erase "$(INTDIR)\N3ColorChange.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3EngTool.sbr"
	-@erase "$(INTDIR)\N3FXBundle.obj"
	-@erase "$(INTDIR)\N3FXBundle.sbr"
	-@erase "$(INTDIR)\N3FXE.obj"
	-@erase "$(INTDIR)\N3FXE.pch"
	-@erase "$(INTDIR)\N3FXE.res"
	-@erase "$(INTDIR)\N3FXE.sbr"
	-@erase "$(INTDIR)\N3FXEDoc.obj"
	-@erase "$(INTDIR)\N3FXEDoc.sbr"
	-@erase "$(INTDIR)\N3FXEView.obj"
	-@erase "$(INTDIR)\N3FXEView.sbr"
	-@erase "$(INTDIR)\N3FXGroup.obj"
	-@erase "$(INTDIR)\N3FXGroup.sbr"
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
	-@erase "$(INTDIR)\N3Joint.obj"
	-@erase "$(INTDIR)\N3Joint.sbr"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Light.sbr"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3Mesh.sbr"
	-@erase "$(INTDIR)\N3Moon.obj"
	-@erase "$(INTDIR)\N3Moon.sbr"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMesh.sbr"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.sbr"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.sbr"
	-@erase "$(INTDIR)\N3River2.obj"
	-@erase "$(INTDIR)\N3River2.sbr"
	-@erase "$(INTDIR)\N3RiverPatch.obj"
	-@erase "$(INTDIR)\N3RiverPatch.sbr"
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Scene.sbr"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Shape.sbr"
	-@erase "$(INTDIR)\N3ShapeMod.obj"
	-@erase "$(INTDIR)\N3ShapeMod.sbr"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Skin.sbr"
	-@erase "$(INTDIR)\N3Sky.obj"
	-@erase "$(INTDIR)\N3Sky.sbr"
	-@erase "$(INTDIR)\N3SndMgr.obj"
	-@erase "$(INTDIR)\N3SndMgr.sbr"
	-@erase "$(INTDIR)\N3SndObj.obj"
	-@erase "$(INTDIR)\N3SndObj.sbr"
	-@erase "$(INTDIR)\N3SndObjStream.obj"
	-@erase "$(INTDIR)\N3SndObjStream.sbr"
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
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\Pick.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WaveFile.obj"
	-@erase "$(INTDIR)\WaveFile.sbr"
	-@erase "$(OUTDIR)\N3FXE.bsc"
	-@erase "$(OUTDIR)\N3FXE_Debug.pdb"
	-@erase ".\N3FXE_Debug.exe"
	-@erase ".\N3FXE_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3FXE.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3FXE.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3FXE.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\N3FXE.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\DFont.sbr" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Board.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Chr.sbr" \
	"$(INTDIR)\N3Cloak.sbr" \
	"$(INTDIR)\N3Cloud.sbr" \
	"$(INTDIR)\N3ColorChange.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3FXPlug.sbr" \
	"$(INTDIR)\N3GERain.sbr" \
	"$(INTDIR)\N3GESnow.sbr" \
	"$(INTDIR)\N3GlobalEffect.sbr" \
	"$(INTDIR)\N3GlobalEffectMng.sbr" \
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Joint.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Mesh.sbr" \
	"$(INTDIR)\N3Moon.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshCreate.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3River2.sbr" \
	"$(INTDIR)\N3RiverPatch.sbr" \
	"$(INTDIR)\N3Scene.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3ShapeMod.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\N3Sky.sbr" \
	"$(INTDIR)\N3SndMgr.sbr" \
	"$(INTDIR)\N3SndObj.sbr" \
	"$(INTDIR)\N3SndObjStream.sbr" \
	"$(INTDIR)\N3Star.sbr" \
	"$(INTDIR)\N3Sun.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr" \
	"$(INTDIR)\Pick.sbr" \
	"$(INTDIR)\WaveFile.sbr" \
	"$(INTDIR)\N3FXBundle.sbr" \
	"$(INTDIR)\N3FXGroup.sbr" \
	"$(INTDIR)\N3FXPartBase.sbr" \
	"$(INTDIR)\N3FXPartBillBoard.sbr" \
	"$(INTDIR)\N3FXPartBottomBoard.sbr" \
	"$(INTDIR)\N3FXParticle.sbr" \
	"$(INTDIR)\N3FXPartMesh.sbr" \
	"$(INTDIR)\N3FXPartParticles.sbr" \
	"$(INTDIR)\N3FXPMesh.sbr" \
	"$(INTDIR)\N3FXPMeshInstance.sbr" \
	"$(INTDIR)\N3FXShape.sbr" \
	"$(INTDIR)\DlgEditFxg.sbr" \
	"$(INTDIR)\DlgEditPartBillBoard.sbr" \
	"$(INTDIR)\DlgEditPartGround.sbr" \
	"$(INTDIR)\DlgEditPartMesh.sbr" \
	"$(INTDIR)\DlgEditPartParticle.sbr" \
	"$(INTDIR)\DlgEditScript.sbr" \
	"$(INTDIR)\DlgNewFileName.sbr" \
	"$(INTDIR)\DlgNewPart.sbr" \
	"$(INTDIR)\DlgPercent.sbr" \
	"$(INTDIR)\DlgPutColor.sbr" \
	"$(INTDIR)\Ground.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3FXEDoc.sbr" \
	"$(INTDIR)\N3FXEView.sbr"

"$(OUTDIR)\N3FXE.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib d3d8.lib d3dx8.lib dsound.lib dxguid.lib strmiids.lib dxerr8.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\N3FXE_Debug.pdb" /debug /machine:I386 /out:"N3FXE_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\N3FXE.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DFont.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Board.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Cloak.obj" \
	"$(INTDIR)\N3Cloud.obj" \
	"$(INTDIR)\N3ColorChange.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3FXPlug.obj" \
	"$(INTDIR)\N3GERain.obj" \
	"$(INTDIR)\N3GESnow.obj" \
	"$(INTDIR)\N3GlobalEffect.obj" \
	"$(INTDIR)\N3GlobalEffectMng.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3River2.obj" \
	"$(INTDIR)\N3RiverPatch.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3ShapeMod.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Sky.obj" \
	"$(INTDIR)\N3SndMgr.obj" \
	"$(INTDIR)\N3SndObj.obj" \
	"$(INTDIR)\N3SndObjStream.obj" \
	"$(INTDIR)\N3Star.obj" \
	"$(INTDIR)\N3Sun.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Pick.obj" \
	"$(INTDIR)\WaveFile.obj" \
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXGroup.obj" \
	"$(INTDIR)\N3FXPartBase.obj" \
	"$(INTDIR)\N3FXPartBillBoard.obj" \
	"$(INTDIR)\N3FXPartBottomBoard.obj" \
	"$(INTDIR)\N3FXParticle.obj" \
	"$(INTDIR)\N3FXPartMesh.obj" \
	"$(INTDIR)\N3FXPartParticles.obj" \
	"$(INTDIR)\N3FXPMesh.obj" \
	"$(INTDIR)\N3FXPMeshInstance.obj" \
	"$(INTDIR)\N3FXShape.obj" \
	"$(INTDIR)\DlgEditFxg.obj" \
	"$(INTDIR)\DlgEditPartBillBoard.obj" \
	"$(INTDIR)\DlgEditPartGround.obj" \
	"$(INTDIR)\DlgEditPartMesh.obj" \
	"$(INTDIR)\DlgEditPartParticle.obj" \
	"$(INTDIR)\DlgEditScript.obj" \
	"$(INTDIR)\DlgNewFileName.obj" \
	"$(INTDIR)\DlgNewPart.obj" \
	"$(INTDIR)\DlgPercent.obj" \
	"$(INTDIR)\DlgPutColor.obj" \
	"$(INTDIR)\Ground.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3FXEDoc.obj" \
	"$(INTDIR)\N3FXEView.obj" \
	"$(INTDIR)\N3FXE.res"

".\N3FXE_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("N3FXE.dep")
!INCLUDE "N3FXE.dep"
!ELSE 
!MESSAGE Warning: cannot find "N3FXE.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "N3FXE - Win32 Release" || "$(CFG)" == "N3FXE - Win32 Debug"
SOURCE=.\N3FXE.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXE.obj"	"$(INTDIR)\N3FXE.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3FXE.rc

"$(INTDIR)\N3FXE.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\N3FXE.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\N3FXE.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3FXE.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\N3FXE.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\engine\N3Base\BitMapFile.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\BitMapFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\DFont.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DFont.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DFont.obj"	"$(INTDIR)\DFont.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\LogWriter.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\LogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\LogWriter.obj"	"$(INTDIR)\LogWriter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AlphaPrimitiveManager.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3AlphaPrimitiveManager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimControl.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3AnimControl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimKey.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3AnimKey.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Base.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3BaseFileAccess.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3BaseFileAccess.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Board.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Board.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Board.obj"	"$(INTDIR)\N3Board.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Camera.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Camera.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Chr.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Chr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Chr.obj"	"$(INTDIR)\N3Chr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Cloak.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Cloak.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Cloak.obj"	"$(INTDIR)\N3Cloak.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Cloud.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Cloud.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Cloud.obj"	"$(INTDIR)\N3Cloud.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3ColorChange.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3ColorChange.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3ColorChange.obj"	"$(INTDIR)\N3ColorChange.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Eng.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3EngTool.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3EngTool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPlug.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXPlug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXPlug.obj"	"$(INTDIR)\N3FXPlug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GERain.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3GERain.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3GERain.obj"	"$(INTDIR)\N3GERain.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GESnow.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3GESnow.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3GESnow.obj"	"$(INTDIR)\N3GESnow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GlobalEffect.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3GlobalEffect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3GlobalEffect.obj"	"$(INTDIR)\N3GlobalEffect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GlobalEffectMng.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3GlobalEffectMng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3GlobalEffectMng.obj"	"$(INTDIR)\N3GlobalEffectMng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3IMesh.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3IMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Joint.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Joint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Joint.obj"	"$(INTDIR)\N3Joint.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Light.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Light.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Mesh.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Mesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Mesh.obj"	"$(INTDIR)\N3Mesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Moon.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Moon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Moon.obj"	"$(INTDIR)\N3Moon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMesh.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3PMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshCreate.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3PMeshCreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3PMeshCreate.obj"	"$(INTDIR)\N3PMeshCreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshInstance.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3PMeshInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3River2.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3River2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3River2.obj"	"$(INTDIR)\N3River2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3RiverPatch.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3RiverPatch.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3RiverPatch.obj"	"$(INTDIR)\N3RiverPatch.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Scene.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Scene.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Scene.obj"	"$(INTDIR)\N3Scene.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Shape.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Shape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3ShapeMod.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3ShapeMod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3ShapeMod.obj"	"$(INTDIR)\N3ShapeMod.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Skin.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Skin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Skin.obj"	"$(INTDIR)\N3Skin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Sky.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Sky.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Sky.obj"	"$(INTDIR)\N3Sky.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3SndMgr.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3SndMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3SndMgr.obj"	"$(INTDIR)\N3SndMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3SndObj.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3SndObj.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3SndObj.obj"	"$(INTDIR)\N3SndObj.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3SndObjStream.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3SndObjStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3SndObjStream.obj"	"$(INTDIR)\N3SndObjStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Star.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Star.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Star.obj"	"$(INTDIR)\N3Star.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Sun.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Sun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Sun.obj"	"$(INTDIR)\N3Sun.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Texture.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Texture.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Transform.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3Transform.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3TransformCollision.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3TransformCollision.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3VMesh.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3VMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\Pick.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\Pick.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\Pick.obj"	"$(INTDIR)\Pick.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\WaveFile.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\WaveFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\WaveFile.obj"	"$(INTDIR)\WaveFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXBundle.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXBundle.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXBundle.obj"	"$(INTDIR)\N3FXBundle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXGroup.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXGroup.obj"	"$(INTDIR)\N3FXGroup.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBase.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXPartBase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXPartBase.obj"	"$(INTDIR)\N3FXPartBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBillBoard.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXPartBillBoard.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXPartBillBoard.obj"	"$(INTDIR)\N3FXPartBillBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBottomBoard.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXPartBottomBoard.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXPartBottomBoard.obj"	"$(INTDIR)\N3FXPartBottomBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXParticle.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXParticle.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXParticle.obj"	"$(INTDIR)\N3FXParticle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartMesh.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXPartMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXPartMesh.obj"	"$(INTDIR)\N3FXPartMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartParticles.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXPartParticles.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXPartParticles.obj"	"$(INTDIR)\N3FXPartParticles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPMesh.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXPMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXPMesh.obj"	"$(INTDIR)\N3FXPMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPMeshInstance.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXPMeshInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXPMeshInstance.obj"	"$(INTDIR)\N3FXPMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXShape.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXShape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXShape.obj"	"$(INTDIR)\N3FXShape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\DlgEditFxg.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgEditFxg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgEditFxg.obj"	"$(INTDIR)\DlgEditFxg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgEditPartBillBoard.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgEditPartBillBoard.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgEditPartBillBoard.obj"	"$(INTDIR)\DlgEditPartBillBoard.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgEditPartGround.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgEditPartGround.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgEditPartGround.obj"	"$(INTDIR)\DlgEditPartGround.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgEditPartMesh.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgEditPartMesh.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgEditPartMesh.obj"	"$(INTDIR)\DlgEditPartMesh.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgEditPartParticle.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgEditPartParticle.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgEditPartParticle.obj"	"$(INTDIR)\DlgEditPartParticle.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgEditScript.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgEditScript.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgEditScript.obj"	"$(INTDIR)\DlgEditScript.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgNewFileName.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgNewFileName.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgNewFileName.obj"	"$(INTDIR)\DlgNewFileName.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgNewPart.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgNewPart.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgNewPart.obj"	"$(INTDIR)\DlgNewPart.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgPercent.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgPercent.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgPercent.obj"	"$(INTDIR)\DlgPercent.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgPutColor.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\DlgPutColor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\DlgPutColor.obj"	"$(INTDIR)\DlgPutColor.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Ground.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\Ground.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\Ground.obj"	"$(INTDIR)\Ground.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3FXEDoc.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXEDoc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXEDoc.obj"	"$(INTDIR)\N3FXEDoc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3FXEView.cpp

!IF  "$(CFG)" == "N3FXE - Win32 Release"


"$(INTDIR)\N3FXEView.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"


"$(INTDIR)\N3FXEView.obj"	"$(INTDIR)\N3FXEView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

