# Microsoft Developer Studio Generated NMAKE File, Based on N3ME.dsp
!IF "$(CFG)" == ""
CFG=N3ME - Win32 Debug
!MESSAGE No configuration specified. Defaulting to N3ME - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "N3ME - Win32 Release" && "$(CFG)" != "N3ME - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3ME.mak" CFG="N3ME - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3ME - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3ME - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "N3ME - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\N3ME.exe" "$(OUTDIR)\N3ME.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\BrushDlg.obj"
	-@erase "$(INTDIR)\BrushDlg.sbr"
	-@erase "$(INTDIR)\DlgAddDTex.obj"
	-@erase "$(INTDIR)\DlgAddDTex.sbr"
	-@erase "$(INTDIR)\DlgAddSoundGroup.obj"
	-@erase "$(INTDIR)\DlgAddSoundGroup.sbr"
	-@erase "$(INTDIR)\DlgBar.obj"
	-@erase "$(INTDIR)\DlgBar.sbr"
	-@erase "$(INTDIR)\DlgBase.obj"
	-@erase "$(INTDIR)\DlgBase.sbr"
	-@erase "$(INTDIR)\DlgBrowsePath.obj"
	-@erase "$(INTDIR)\DlgBrowsePath.sbr"
	-@erase "$(INTDIR)\DlgCtrlHeightScale.obj"
	-@erase "$(INTDIR)\DlgCtrlHeightScale.sbr"
	-@erase "$(INTDIR)\DlgDelGroup.obj"
	-@erase "$(INTDIR)\DlgDelGroup.sbr"
	-@erase "$(INTDIR)\DlgDTexGroupView.obj"
	-@erase "$(INTDIR)\DlgDTexGroupView.sbr"
	-@erase "$(INTDIR)\DlgEditEvent.obj"
	-@erase "$(INTDIR)\DlgEditEvent.sbr"
	-@erase "$(INTDIR)\DlgEditEventAttr.obj"
	-@erase "$(INTDIR)\DlgEditEventAttr.sbr"
	-@erase "$(INTDIR)\DlgEditWarp.obj"
	-@erase "$(INTDIR)\DlgEditWarp.sbr"
	-@erase "$(INTDIR)\DlgInputAttr.obj"
	-@erase "$(INTDIR)\DlgInputAttr.sbr"
	-@erase "$(INTDIR)\DlgInputGroup.obj"
	-@erase "$(INTDIR)\DlgInputGroup.sbr"
	-@erase "$(INTDIR)\DlgLight.obj"
	-@erase "$(INTDIR)\DlgLight.sbr"
	-@erase "$(INTDIR)\DlgLoadEvt.obj"
	-@erase "$(INTDIR)\DlgLoadEvt.sbr"
	-@erase "$(INTDIR)\DlgLoadNPCPath.obj"
	-@erase "$(INTDIR)\DlgLoadNPCPath.sbr"
	-@erase "$(INTDIR)\DlgLoadTileSet.obj"
	-@erase "$(INTDIR)\DlgLoadTileSet.sbr"
	-@erase "$(INTDIR)\DlgMakeNPCPath.obj"
	-@erase "$(INTDIR)\DlgMakeNPCPath.sbr"
	-@erase "$(INTDIR)\DlgMakeWall.obj"
	-@erase "$(INTDIR)\DlgMakeWall.sbr"
	-@erase "$(INTDIR)\DlgMapView.obj"
	-@erase "$(INTDIR)\DlgMapView.sbr"
	-@erase "$(INTDIR)\DlgModifyDTex.obj"
	-@erase "$(INTDIR)\DlgModifyDTex.sbr"
	-@erase "$(INTDIR)\DlgPondProperty.obj"
	-@erase "$(INTDIR)\DlgPondProperty.sbr"
	-@erase "$(INTDIR)\DlgRegenUser.obj"
	-@erase "$(INTDIR)\DlgRegenUser.sbr"
	-@erase "$(INTDIR)\DlgRiverProperty.obj"
	-@erase "$(INTDIR)\DlgRiverProperty.sbr"
	-@erase "$(INTDIR)\DlgSaveDivision.obj"
	-@erase "$(INTDIR)\DlgSaveDivision.sbr"
	-@erase "$(INTDIR)\DlgSaveEvt.obj"
	-@erase "$(INTDIR)\DlgSaveEvt.sbr"
	-@erase "$(INTDIR)\DlgSaveNewTileSet.obj"
	-@erase "$(INTDIR)\DlgSaveNewTileSet.sbr"
	-@erase "$(INTDIR)\DlgSaveNPCPath.obj"
	-@erase "$(INTDIR)\DlgSaveNPCPath.sbr"
	-@erase "$(INTDIR)\DlgSceneGraph.obj"
	-@erase "$(INTDIR)\DlgSceneGraph.sbr"
	-@erase "$(INTDIR)\DlgSetDTex.obj"
	-@erase "$(INTDIR)\DlgSetDTex.sbr"
	-@erase "$(INTDIR)\DlgSetLightMap.obj"
	-@erase "$(INTDIR)\DlgSetLightMap.sbr"
	-@erase "$(INTDIR)\DlgSetSound.obj"
	-@erase "$(INTDIR)\DlgSetSound.sbr"
	-@erase "$(INTDIR)\DlgShapeList.obj"
	-@erase "$(INTDIR)\DlgShapeList.sbr"
	-@erase "$(INTDIR)\DlgSowSeed.obj"
	-@erase "$(INTDIR)\DlgSowSeed.sbr"
	-@erase "$(INTDIR)\DlgTerrainSize.obj"
	-@erase "$(INTDIR)\DlgTerrainSize.sbr"
	-@erase "$(INTDIR)\DlgUnusedFiles.obj"
	-@erase "$(INTDIR)\DlgUnusedFiles.sbr"
	-@erase "$(INTDIR)\DTex.obj"
	-@erase "$(INTDIR)\DTex.sbr"
	-@erase "$(INTDIR)\DTexGroup.obj"
	-@erase "$(INTDIR)\DTexGroup.sbr"
	-@erase "$(INTDIR)\DTexGroupMng.obj"
	-@erase "$(INTDIR)\DTexGroupMng.sbr"
	-@erase "$(INTDIR)\DTexMng.obj"
	-@erase "$(INTDIR)\DTexMng.sbr"
	-@erase "$(INTDIR)\EventCell.obj"
	-@erase "$(INTDIR)\EventCell.sbr"
	-@erase "$(INTDIR)\EventMgr.obj"
	-@erase "$(INTDIR)\EventMgr.sbr"
	-@erase "$(INTDIR)\LightObjMgr.obj"
	-@erase "$(INTDIR)\LightObjMgr.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
	-@erase "$(INTDIR)\LyTerrain.obj"
	-@erase "$(INTDIR)\LyTerrain.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MapMng.obj"
	-@erase "$(INTDIR)\MapMng.sbr"
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
	-@erase "$(INTDIR)\N3ME.obj"
	-@erase "$(INTDIR)\N3ME.res"
	-@erase "$(INTDIR)\N3ME.sbr"
	-@erase "$(INTDIR)\N3MEDoc.obj"
	-@erase "$(INTDIR)\N3MEDoc.sbr"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3Mesh.sbr"
	-@erase "$(INTDIR)\N3MEView.obj"
	-@erase "$(INTDIR)\N3MEView.sbr"
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
	-@erase "$(INTDIR)\N3ShapeMgr.obj"
	-@erase "$(INTDIR)\N3ShapeMgr.sbr"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Skin.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3TransformCollision.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\NPCPath.obj"
	-@erase "$(INTDIR)\NPCPath.sbr"
	-@erase "$(INTDIR)\NPCPathMgr.obj"
	-@erase "$(INTDIR)\NPCPathMgr.sbr"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\Pick.sbr"
	-@erase "$(INTDIR)\PondMesh.obj"
	-@erase "$(INTDIR)\PondMesh.sbr"
	-@erase "$(INTDIR)\PondMng.obj"
	-@erase "$(INTDIR)\PondMng.sbr"
	-@erase "$(INTDIR)\PosDummy.obj"
	-@erase "$(INTDIR)\PosDummy.sbr"
	-@erase "$(INTDIR)\ProgressBar.obj"
	-@erase "$(INTDIR)\ProgressBar.sbr"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\QTNode.obj"
	-@erase "$(INTDIR)\QTNode.sbr"
	-@erase "$(INTDIR)\RegenUser.obj"
	-@erase "$(INTDIR)\RegenUser.sbr"
	-@erase "$(INTDIR)\RiverMesh.obj"
	-@erase "$(INTDIR)\RiverMesh.sbr"
	-@erase "$(INTDIR)\RiverMng.obj"
	-@erase "$(INTDIR)\RiverMng.sbr"
	-@erase "$(INTDIR)\RotDummy.obj"
	-@erase "$(INTDIR)\RotDummy.sbr"
	-@erase "$(INTDIR)\ScaleDummy.obj"
	-@erase "$(INTDIR)\ScaleDummy.sbr"
	-@erase "$(INTDIR)\ShellTree.obj"
	-@erase "$(INTDIR)\ShellTree.sbr"
	-@erase "$(INTDIR)\SoundCell.obj"
	-@erase "$(INTDIR)\SoundCell.sbr"
	-@erase "$(INTDIR)\SoundMgr.obj"
	-@erase "$(INTDIR)\SoundMgr.sbr"
	-@erase "$(INTDIR)\SowSeedMng.obj"
	-@erase "$(INTDIR)\SowSeedMng.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TransDummy.obj"
	-@erase "$(INTDIR)\TransDummy.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VtxPosDummy.obj"
	-@erase "$(INTDIR)\VtxPosDummy.sbr"
	-@erase "$(INTDIR)\Wall.obj"
	-@erase "$(INTDIR)\Wall.sbr"
	-@erase "$(INTDIR)\WallMgr.obj"
	-@erase "$(INTDIR)\WallMgr.sbr"
	-@erase "$(INTDIR)\WarpMgr.obj"
	-@erase "$(INTDIR)\WarpMgr.sbr"
	-@erase "$(OUTDIR)\N3ME.bsc"
	-@erase "$(OUTDIR)\N3ME.map"
	-@erase ".\N3ME.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Zi /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /D "_KNIGHT" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\N3ME.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3ME.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3ME.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Chr.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3FXBundle.sbr" \
	"$(INTDIR)\N3FXPartBase.sbr" \
	"$(INTDIR)\N3FXPartBillBoard.sbr" \
	"$(INTDIR)\N3FXPartBottomBoard.sbr" \
	"$(INTDIR)\N3FXParticle.sbr" \
	"$(INTDIR)\N3FXPartMesh.sbr" \
	"$(INTDIR)\N3FXPartParticles.sbr" \
	"$(INTDIR)\N3FXPlug.sbr" \
	"$(INTDIR)\N3FXPMesh.sbr" \
	"$(INTDIR)\N3FXPMeshInstance.sbr" \
	"$(INTDIR)\N3FXShape.sbr" \
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
	"$(INTDIR)\N3ShapeMgr.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr" \
	"$(INTDIR)\Pick.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3ME.sbr" \
	"$(INTDIR)\N3MEDoc.sbr" \
	"$(INTDIR)\N3MEView.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\BrushDlg.sbr" \
	"$(INTDIR)\DlgAddDTex.sbr" \
	"$(INTDIR)\DlgAddSoundGroup.sbr" \
	"$(INTDIR)\DlgBar.sbr" \
	"$(INTDIR)\DlgBase.sbr" \
	"$(INTDIR)\DlgBrowsePath.sbr" \
	"$(INTDIR)\DlgCtrlHeightScale.sbr" \
	"$(INTDIR)\DlgDelGroup.sbr" \
	"$(INTDIR)\DlgDTexGroupView.sbr" \
	"$(INTDIR)\DlgEditEvent.sbr" \
	"$(INTDIR)\DlgEditEventAttr.sbr" \
	"$(INTDIR)\DlgEditWarp.sbr" \
	"$(INTDIR)\DlgInputAttr.sbr" \
	"$(INTDIR)\DlgInputGroup.sbr" \
	"$(INTDIR)\DlgLight.sbr" \
	"$(INTDIR)\DlgLoadEvt.sbr" \
	"$(INTDIR)\DlgLoadNPCPath.sbr" \
	"$(INTDIR)\DlgLoadTileSet.sbr" \
	"$(INTDIR)\DlgMakeNPCPath.sbr" \
	"$(INTDIR)\DlgMakeWall.sbr" \
	"$(INTDIR)\DlgModifyDTex.sbr" \
	"$(INTDIR)\DlgPondProperty.sbr" \
	"$(INTDIR)\DlgRegenUser.sbr" \
	"$(INTDIR)\DlgRiverProperty.sbr" \
	"$(INTDIR)\DlgSaveDivision.sbr" \
	"$(INTDIR)\DlgSaveEvt.sbr" \
	"$(INTDIR)\DlgSaveNewTileSet.sbr" \
	"$(INTDIR)\DlgSaveNPCPath.sbr" \
	"$(INTDIR)\DlgSceneGraph.sbr" \
	"$(INTDIR)\DlgSetDTex.sbr" \
	"$(INTDIR)\DlgSetLightMap.sbr" \
	"$(INTDIR)\DlgSetSound.sbr" \
	"$(INTDIR)\DlgShapeList.sbr" \
	"$(INTDIR)\DlgSowSeed.sbr" \
	"$(INTDIR)\DlgTerrainSize.sbr" \
	"$(INTDIR)\DlgUnusedFiles.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\ShellTree.sbr" \
	"$(INTDIR)\ProgressBar.sbr" \
	"$(INTDIR)\DTex.sbr" \
	"$(INTDIR)\DTexGroup.sbr" \
	"$(INTDIR)\DTexGroupMng.sbr" \
	"$(INTDIR)\DTexMng.sbr" \
	"$(INTDIR)\EventCell.sbr" \
	"$(INTDIR)\EventMgr.sbr" \
	"$(INTDIR)\LightObjMgr.sbr" \
	"$(INTDIR)\LyTerrain.sbr" \
	"$(INTDIR)\MapMng.sbr" \
	"$(INTDIR)\NPCPath.sbr" \
	"$(INTDIR)\NPCPathMgr.sbr" \
	"$(INTDIR)\PondMesh.sbr" \
	"$(INTDIR)\PondMng.sbr" \
	"$(INTDIR)\PosDummy.sbr" \
	"$(INTDIR)\QTNode.sbr" \
	"$(INTDIR)\RegenUser.sbr" \
	"$(INTDIR)\RiverMesh.sbr" \
	"$(INTDIR)\RiverMng.sbr" \
	"$(INTDIR)\RotDummy.sbr" \
	"$(INTDIR)\ScaleDummy.sbr" \
	"$(INTDIR)\SoundCell.sbr" \
	"$(INTDIR)\SoundMgr.sbr" \
	"$(INTDIR)\TransDummy.sbr" \
	"$(INTDIR)\VtxPosDummy.sbr" \
	"$(INTDIR)\Wall.sbr" \
	"$(INTDIR)\WallMgr.sbr" \
	"$(INTDIR)\WarpMgr.sbr" \
	"$(INTDIR)\DlgMapView.sbr" \
	"$(INTDIR)\SowSeedMng.sbr"

"$(OUTDIR)\N3ME.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=D3D8.lib D3DX8.lib dxerr8.lib dsound.lib dxguid.lib winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\N3ME.pdb" /map:"$(INTDIR)\N3ME.map" /machine:I386 /force /out:"N3ME.exe" /opt:ref 
LINK32_OBJS= \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXPartBase.obj" \
	"$(INTDIR)\N3FXPartBillBoard.obj" \
	"$(INTDIR)\N3FXPartBottomBoard.obj" \
	"$(INTDIR)\N3FXParticle.obj" \
	"$(INTDIR)\N3FXPartMesh.obj" \
	"$(INTDIR)\N3FXPartParticles.obj" \
	"$(INTDIR)\N3FXPlug.obj" \
	"$(INTDIR)\N3FXPMesh.obj" \
	"$(INTDIR)\N3FXPMeshInstance.obj" \
	"$(INTDIR)\N3FXShape.obj" \
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
	"$(INTDIR)\N3ShapeMgr.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Pick.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3ME.obj" \
	"$(INTDIR)\N3MEDoc.obj" \
	"$(INTDIR)\N3MEView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BrushDlg.obj" \
	"$(INTDIR)\DlgAddDTex.obj" \
	"$(INTDIR)\DlgAddSoundGroup.obj" \
	"$(INTDIR)\DlgBar.obj" \
	"$(INTDIR)\DlgBase.obj" \
	"$(INTDIR)\DlgBrowsePath.obj" \
	"$(INTDIR)\DlgCtrlHeightScale.obj" \
	"$(INTDIR)\DlgDelGroup.obj" \
	"$(INTDIR)\DlgDTexGroupView.obj" \
	"$(INTDIR)\DlgEditEvent.obj" \
	"$(INTDIR)\DlgEditEventAttr.obj" \
	"$(INTDIR)\DlgEditWarp.obj" \
	"$(INTDIR)\DlgInputAttr.obj" \
	"$(INTDIR)\DlgInputGroup.obj" \
	"$(INTDIR)\DlgLight.obj" \
	"$(INTDIR)\DlgLoadEvt.obj" \
	"$(INTDIR)\DlgLoadNPCPath.obj" \
	"$(INTDIR)\DlgLoadTileSet.obj" \
	"$(INTDIR)\DlgMakeNPCPath.obj" \
	"$(INTDIR)\DlgMakeWall.obj" \
	"$(INTDIR)\DlgModifyDTex.obj" \
	"$(INTDIR)\DlgPondProperty.obj" \
	"$(INTDIR)\DlgRegenUser.obj" \
	"$(INTDIR)\DlgRiverProperty.obj" \
	"$(INTDIR)\DlgSaveDivision.obj" \
	"$(INTDIR)\DlgSaveEvt.obj" \
	"$(INTDIR)\DlgSaveNewTileSet.obj" \
	"$(INTDIR)\DlgSaveNPCPath.obj" \
	"$(INTDIR)\DlgSceneGraph.obj" \
	"$(INTDIR)\DlgSetDTex.obj" \
	"$(INTDIR)\DlgSetLightMap.obj" \
	"$(INTDIR)\DlgSetSound.obj" \
	"$(INTDIR)\DlgShapeList.obj" \
	"$(INTDIR)\DlgSowSeed.obj" \
	"$(INTDIR)\DlgTerrainSize.obj" \
	"$(INTDIR)\DlgUnusedFiles.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\ShellTree.obj" \
	"$(INTDIR)\ProgressBar.obj" \
	"$(INTDIR)\DTex.obj" \
	"$(INTDIR)\DTexGroup.obj" \
	"$(INTDIR)\DTexGroupMng.obj" \
	"$(INTDIR)\DTexMng.obj" \
	"$(INTDIR)\EventCell.obj" \
	"$(INTDIR)\EventMgr.obj" \
	"$(INTDIR)\LightObjMgr.obj" \
	"$(INTDIR)\LyTerrain.obj" \
	"$(INTDIR)\MapMng.obj" \
	"$(INTDIR)\NPCPath.obj" \
	"$(INTDIR)\NPCPathMgr.obj" \
	"$(INTDIR)\PondMesh.obj" \
	"$(INTDIR)\PondMng.obj" \
	"$(INTDIR)\PosDummy.obj" \
	"$(INTDIR)\QTNode.obj" \
	"$(INTDIR)\RegenUser.obj" \
	"$(INTDIR)\RiverMesh.obj" \
	"$(INTDIR)\RiverMng.obj" \
	"$(INTDIR)\RotDummy.obj" \
	"$(INTDIR)\ScaleDummy.obj" \
	"$(INTDIR)\SoundCell.obj" \
	"$(INTDIR)\SoundMgr.obj" \
	"$(INTDIR)\TransDummy.obj" \
	"$(INTDIR)\VtxPosDummy.obj" \
	"$(INTDIR)\Wall.obj" \
	"$(INTDIR)\WallMgr.obj" \
	"$(INTDIR)\WarpMgr.obj" \
	"$(INTDIR)\DlgMapView.obj" \
	"$(INTDIR)\SowSeedMng.obj" \
	"$(INTDIR)\N3ME.res"

".\N3ME.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "N3ME - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\N3ME_debug.exe" "$(OUTDIR)\N3ME.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\BrushDlg.obj"
	-@erase "$(INTDIR)\BrushDlg.sbr"
	-@erase "$(INTDIR)\DlgAddDTex.obj"
	-@erase "$(INTDIR)\DlgAddDTex.sbr"
	-@erase "$(INTDIR)\DlgAddSoundGroup.obj"
	-@erase "$(INTDIR)\DlgAddSoundGroup.sbr"
	-@erase "$(INTDIR)\DlgBar.obj"
	-@erase "$(INTDIR)\DlgBar.sbr"
	-@erase "$(INTDIR)\DlgBase.obj"
	-@erase "$(INTDIR)\DlgBase.sbr"
	-@erase "$(INTDIR)\DlgBrowsePath.obj"
	-@erase "$(INTDIR)\DlgBrowsePath.sbr"
	-@erase "$(INTDIR)\DlgCtrlHeightScale.obj"
	-@erase "$(INTDIR)\DlgCtrlHeightScale.sbr"
	-@erase "$(INTDIR)\DlgDelGroup.obj"
	-@erase "$(INTDIR)\DlgDelGroup.sbr"
	-@erase "$(INTDIR)\DlgDTexGroupView.obj"
	-@erase "$(INTDIR)\DlgDTexGroupView.sbr"
	-@erase "$(INTDIR)\DlgEditEvent.obj"
	-@erase "$(INTDIR)\DlgEditEvent.sbr"
	-@erase "$(INTDIR)\DlgEditEventAttr.obj"
	-@erase "$(INTDIR)\DlgEditEventAttr.sbr"
	-@erase "$(INTDIR)\DlgEditWarp.obj"
	-@erase "$(INTDIR)\DlgEditWarp.sbr"
	-@erase "$(INTDIR)\DlgInputAttr.obj"
	-@erase "$(INTDIR)\DlgInputAttr.sbr"
	-@erase "$(INTDIR)\DlgInputGroup.obj"
	-@erase "$(INTDIR)\DlgInputGroup.sbr"
	-@erase "$(INTDIR)\DlgLight.obj"
	-@erase "$(INTDIR)\DlgLight.sbr"
	-@erase "$(INTDIR)\DlgLoadEvt.obj"
	-@erase "$(INTDIR)\DlgLoadEvt.sbr"
	-@erase "$(INTDIR)\DlgLoadNPCPath.obj"
	-@erase "$(INTDIR)\DlgLoadNPCPath.sbr"
	-@erase "$(INTDIR)\DlgLoadTileSet.obj"
	-@erase "$(INTDIR)\DlgLoadTileSet.sbr"
	-@erase "$(INTDIR)\DlgMakeNPCPath.obj"
	-@erase "$(INTDIR)\DlgMakeNPCPath.sbr"
	-@erase "$(INTDIR)\DlgMakeWall.obj"
	-@erase "$(INTDIR)\DlgMakeWall.sbr"
	-@erase "$(INTDIR)\DlgMapView.obj"
	-@erase "$(INTDIR)\DlgMapView.sbr"
	-@erase "$(INTDIR)\DlgModifyDTex.obj"
	-@erase "$(INTDIR)\DlgModifyDTex.sbr"
	-@erase "$(INTDIR)\DlgPondProperty.obj"
	-@erase "$(INTDIR)\DlgPondProperty.sbr"
	-@erase "$(INTDIR)\DlgRegenUser.obj"
	-@erase "$(INTDIR)\DlgRegenUser.sbr"
	-@erase "$(INTDIR)\DlgRiverProperty.obj"
	-@erase "$(INTDIR)\DlgRiverProperty.sbr"
	-@erase "$(INTDIR)\DlgSaveDivision.obj"
	-@erase "$(INTDIR)\DlgSaveDivision.sbr"
	-@erase "$(INTDIR)\DlgSaveEvt.obj"
	-@erase "$(INTDIR)\DlgSaveEvt.sbr"
	-@erase "$(INTDIR)\DlgSaveNewTileSet.obj"
	-@erase "$(INTDIR)\DlgSaveNewTileSet.sbr"
	-@erase "$(INTDIR)\DlgSaveNPCPath.obj"
	-@erase "$(INTDIR)\DlgSaveNPCPath.sbr"
	-@erase "$(INTDIR)\DlgSceneGraph.obj"
	-@erase "$(INTDIR)\DlgSceneGraph.sbr"
	-@erase "$(INTDIR)\DlgSetDTex.obj"
	-@erase "$(INTDIR)\DlgSetDTex.sbr"
	-@erase "$(INTDIR)\DlgSetLightMap.obj"
	-@erase "$(INTDIR)\DlgSetLightMap.sbr"
	-@erase "$(INTDIR)\DlgSetSound.obj"
	-@erase "$(INTDIR)\DlgSetSound.sbr"
	-@erase "$(INTDIR)\DlgShapeList.obj"
	-@erase "$(INTDIR)\DlgShapeList.sbr"
	-@erase "$(INTDIR)\DlgSowSeed.obj"
	-@erase "$(INTDIR)\DlgSowSeed.sbr"
	-@erase "$(INTDIR)\DlgTerrainSize.obj"
	-@erase "$(INTDIR)\DlgTerrainSize.sbr"
	-@erase "$(INTDIR)\DlgUnusedFiles.obj"
	-@erase "$(INTDIR)\DlgUnusedFiles.sbr"
	-@erase "$(INTDIR)\DTex.obj"
	-@erase "$(INTDIR)\DTex.sbr"
	-@erase "$(INTDIR)\DTexGroup.obj"
	-@erase "$(INTDIR)\DTexGroup.sbr"
	-@erase "$(INTDIR)\DTexGroupMng.obj"
	-@erase "$(INTDIR)\DTexGroupMng.sbr"
	-@erase "$(INTDIR)\DTexMng.obj"
	-@erase "$(INTDIR)\DTexMng.sbr"
	-@erase "$(INTDIR)\EventCell.obj"
	-@erase "$(INTDIR)\EventCell.sbr"
	-@erase "$(INTDIR)\EventMgr.obj"
	-@erase "$(INTDIR)\EventMgr.sbr"
	-@erase "$(INTDIR)\LightObjMgr.obj"
	-@erase "$(INTDIR)\LightObjMgr.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
	-@erase "$(INTDIR)\LyTerrain.obj"
	-@erase "$(INTDIR)\LyTerrain.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MapMng.obj"
	-@erase "$(INTDIR)\MapMng.sbr"
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
	-@erase "$(INTDIR)\N3ME.obj"
	-@erase "$(INTDIR)\N3ME.res"
	-@erase "$(INTDIR)\N3ME.sbr"
	-@erase "$(INTDIR)\N3MEDoc.obj"
	-@erase "$(INTDIR)\N3MEDoc.sbr"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3Mesh.sbr"
	-@erase "$(INTDIR)\N3MEView.obj"
	-@erase "$(INTDIR)\N3MEView.sbr"
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
	-@erase "$(INTDIR)\N3ShapeMgr.obj"
	-@erase "$(INTDIR)\N3ShapeMgr.sbr"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Skin.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3TransformCollision.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\NPCPath.obj"
	-@erase "$(INTDIR)\NPCPath.sbr"
	-@erase "$(INTDIR)\NPCPathMgr.obj"
	-@erase "$(INTDIR)\NPCPathMgr.sbr"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\Pick.sbr"
	-@erase "$(INTDIR)\PondMesh.obj"
	-@erase "$(INTDIR)\PondMesh.sbr"
	-@erase "$(INTDIR)\PondMng.obj"
	-@erase "$(INTDIR)\PondMng.sbr"
	-@erase "$(INTDIR)\PosDummy.obj"
	-@erase "$(INTDIR)\PosDummy.sbr"
	-@erase "$(INTDIR)\ProgressBar.obj"
	-@erase "$(INTDIR)\ProgressBar.sbr"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\QTNode.obj"
	-@erase "$(INTDIR)\QTNode.sbr"
	-@erase "$(INTDIR)\RegenUser.obj"
	-@erase "$(INTDIR)\RegenUser.sbr"
	-@erase "$(INTDIR)\RiverMesh.obj"
	-@erase "$(INTDIR)\RiverMesh.sbr"
	-@erase "$(INTDIR)\RiverMng.obj"
	-@erase "$(INTDIR)\RiverMng.sbr"
	-@erase "$(INTDIR)\RotDummy.obj"
	-@erase "$(INTDIR)\RotDummy.sbr"
	-@erase "$(INTDIR)\ScaleDummy.obj"
	-@erase "$(INTDIR)\ScaleDummy.sbr"
	-@erase "$(INTDIR)\ShellTree.obj"
	-@erase "$(INTDIR)\ShellTree.sbr"
	-@erase "$(INTDIR)\SoundCell.obj"
	-@erase "$(INTDIR)\SoundCell.sbr"
	-@erase "$(INTDIR)\SoundMgr.obj"
	-@erase "$(INTDIR)\SoundMgr.sbr"
	-@erase "$(INTDIR)\SowSeedMng.obj"
	-@erase "$(INTDIR)\SowSeedMng.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TransDummy.obj"
	-@erase "$(INTDIR)\TransDummy.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VtxPosDummy.obj"
	-@erase "$(INTDIR)\VtxPosDummy.sbr"
	-@erase "$(INTDIR)\Wall.obj"
	-@erase "$(INTDIR)\Wall.sbr"
	-@erase "$(INTDIR)\WallMgr.obj"
	-@erase "$(INTDIR)\WallMgr.sbr"
	-@erase "$(INTDIR)\WarpMgr.obj"
	-@erase "$(INTDIR)\WarpMgr.sbr"
	-@erase "$(OUTDIR)\N3ME.bsc"
	-@erase "$(OUTDIR)\N3ME_debug.map"
	-@erase "$(OUTDIR)\N3ME_debug.pdb"
	-@erase ".\N3ME_debug.exe"
	-@erase ".\N3ME_debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /D "_KNIGHT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3ME.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3ME.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3ME.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Chr.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3FXBundle.sbr" \
	"$(INTDIR)\N3FXPartBase.sbr" \
	"$(INTDIR)\N3FXPartBillBoard.sbr" \
	"$(INTDIR)\N3FXPartBottomBoard.sbr" \
	"$(INTDIR)\N3FXParticle.sbr" \
	"$(INTDIR)\N3FXPartMesh.sbr" \
	"$(INTDIR)\N3FXPartParticles.sbr" \
	"$(INTDIR)\N3FXPlug.sbr" \
	"$(INTDIR)\N3FXPMesh.sbr" \
	"$(INTDIR)\N3FXPMeshInstance.sbr" \
	"$(INTDIR)\N3FXShape.sbr" \
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
	"$(INTDIR)\N3ShapeMgr.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr" \
	"$(INTDIR)\Pick.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3ME.sbr" \
	"$(INTDIR)\N3MEDoc.sbr" \
	"$(INTDIR)\N3MEView.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\BrushDlg.sbr" \
	"$(INTDIR)\DlgAddDTex.sbr" \
	"$(INTDIR)\DlgAddSoundGroup.sbr" \
	"$(INTDIR)\DlgBar.sbr" \
	"$(INTDIR)\DlgBase.sbr" \
	"$(INTDIR)\DlgBrowsePath.sbr" \
	"$(INTDIR)\DlgCtrlHeightScale.sbr" \
	"$(INTDIR)\DlgDelGroup.sbr" \
	"$(INTDIR)\DlgDTexGroupView.sbr" \
	"$(INTDIR)\DlgEditEvent.sbr" \
	"$(INTDIR)\DlgEditEventAttr.sbr" \
	"$(INTDIR)\DlgEditWarp.sbr" \
	"$(INTDIR)\DlgInputAttr.sbr" \
	"$(INTDIR)\DlgInputGroup.sbr" \
	"$(INTDIR)\DlgLight.sbr" \
	"$(INTDIR)\DlgLoadEvt.sbr" \
	"$(INTDIR)\DlgLoadNPCPath.sbr" \
	"$(INTDIR)\DlgLoadTileSet.sbr" \
	"$(INTDIR)\DlgMakeNPCPath.sbr" \
	"$(INTDIR)\DlgMakeWall.sbr" \
	"$(INTDIR)\DlgModifyDTex.sbr" \
	"$(INTDIR)\DlgPondProperty.sbr" \
	"$(INTDIR)\DlgRegenUser.sbr" \
	"$(INTDIR)\DlgRiverProperty.sbr" \
	"$(INTDIR)\DlgSaveDivision.sbr" \
	"$(INTDIR)\DlgSaveEvt.sbr" \
	"$(INTDIR)\DlgSaveNewTileSet.sbr" \
	"$(INTDIR)\DlgSaveNPCPath.sbr" \
	"$(INTDIR)\DlgSceneGraph.sbr" \
	"$(INTDIR)\DlgSetDTex.sbr" \
	"$(INTDIR)\DlgSetLightMap.sbr" \
	"$(INTDIR)\DlgSetSound.sbr" \
	"$(INTDIR)\DlgShapeList.sbr" \
	"$(INTDIR)\DlgSowSeed.sbr" \
	"$(INTDIR)\DlgTerrainSize.sbr" \
	"$(INTDIR)\DlgUnusedFiles.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\ShellTree.sbr" \
	"$(INTDIR)\ProgressBar.sbr" \
	"$(INTDIR)\DTex.sbr" \
	"$(INTDIR)\DTexGroup.sbr" \
	"$(INTDIR)\DTexGroupMng.sbr" \
	"$(INTDIR)\DTexMng.sbr" \
	"$(INTDIR)\EventCell.sbr" \
	"$(INTDIR)\EventMgr.sbr" \
	"$(INTDIR)\LightObjMgr.sbr" \
	"$(INTDIR)\LyTerrain.sbr" \
	"$(INTDIR)\MapMng.sbr" \
	"$(INTDIR)\NPCPath.sbr" \
	"$(INTDIR)\NPCPathMgr.sbr" \
	"$(INTDIR)\PondMesh.sbr" \
	"$(INTDIR)\PondMng.sbr" \
	"$(INTDIR)\PosDummy.sbr" \
	"$(INTDIR)\QTNode.sbr" \
	"$(INTDIR)\RegenUser.sbr" \
	"$(INTDIR)\RiverMesh.sbr" \
	"$(INTDIR)\RiverMng.sbr" \
	"$(INTDIR)\RotDummy.sbr" \
	"$(INTDIR)\ScaleDummy.sbr" \
	"$(INTDIR)\SoundCell.sbr" \
	"$(INTDIR)\SoundMgr.sbr" \
	"$(INTDIR)\TransDummy.sbr" \
	"$(INTDIR)\VtxPosDummy.sbr" \
	"$(INTDIR)\Wall.sbr" \
	"$(INTDIR)\WallMgr.sbr" \
	"$(INTDIR)\WarpMgr.sbr" \
	"$(INTDIR)\DlgMapView.sbr" \
	"$(INTDIR)\SowSeedMng.sbr"

"$(OUTDIR)\N3ME.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=D3D8.lib D3DX8.lib dxerr8.lib dsound.lib dxguid.lib winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\N3ME_debug.pdb" /map:"$(INTDIR)\N3ME_debug.map" /debug /machine:I386 /force /out:"N3ME_debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXPartBase.obj" \
	"$(INTDIR)\N3FXPartBillBoard.obj" \
	"$(INTDIR)\N3FXPartBottomBoard.obj" \
	"$(INTDIR)\N3FXParticle.obj" \
	"$(INTDIR)\N3FXPartMesh.obj" \
	"$(INTDIR)\N3FXPartParticles.obj" \
	"$(INTDIR)\N3FXPlug.obj" \
	"$(INTDIR)\N3FXPMesh.obj" \
	"$(INTDIR)\N3FXPMeshInstance.obj" \
	"$(INTDIR)\N3FXShape.obj" \
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
	"$(INTDIR)\N3ShapeMgr.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Pick.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3ME.obj" \
	"$(INTDIR)\N3MEDoc.obj" \
	"$(INTDIR)\N3MEView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\BrushDlg.obj" \
	"$(INTDIR)\DlgAddDTex.obj" \
	"$(INTDIR)\DlgAddSoundGroup.obj" \
	"$(INTDIR)\DlgBar.obj" \
	"$(INTDIR)\DlgBase.obj" \
	"$(INTDIR)\DlgBrowsePath.obj" \
	"$(INTDIR)\DlgCtrlHeightScale.obj" \
	"$(INTDIR)\DlgDelGroup.obj" \
	"$(INTDIR)\DlgDTexGroupView.obj" \
	"$(INTDIR)\DlgEditEvent.obj" \
	"$(INTDIR)\DlgEditEventAttr.obj" \
	"$(INTDIR)\DlgEditWarp.obj" \
	"$(INTDIR)\DlgInputAttr.obj" \
	"$(INTDIR)\DlgInputGroup.obj" \
	"$(INTDIR)\DlgLight.obj" \
	"$(INTDIR)\DlgLoadEvt.obj" \
	"$(INTDIR)\DlgLoadNPCPath.obj" \
	"$(INTDIR)\DlgLoadTileSet.obj" \
	"$(INTDIR)\DlgMakeNPCPath.obj" \
	"$(INTDIR)\DlgMakeWall.obj" \
	"$(INTDIR)\DlgModifyDTex.obj" \
	"$(INTDIR)\DlgPondProperty.obj" \
	"$(INTDIR)\DlgRegenUser.obj" \
	"$(INTDIR)\DlgRiverProperty.obj" \
	"$(INTDIR)\DlgSaveDivision.obj" \
	"$(INTDIR)\DlgSaveEvt.obj" \
	"$(INTDIR)\DlgSaveNewTileSet.obj" \
	"$(INTDIR)\DlgSaveNPCPath.obj" \
	"$(INTDIR)\DlgSceneGraph.obj" \
	"$(INTDIR)\DlgSetDTex.obj" \
	"$(INTDIR)\DlgSetLightMap.obj" \
	"$(INTDIR)\DlgSetSound.obj" \
	"$(INTDIR)\DlgShapeList.obj" \
	"$(INTDIR)\DlgSowSeed.obj" \
	"$(INTDIR)\DlgTerrainSize.obj" \
	"$(INTDIR)\DlgUnusedFiles.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\ShellTree.obj" \
	"$(INTDIR)\ProgressBar.obj" \
	"$(INTDIR)\DTex.obj" \
	"$(INTDIR)\DTexGroup.obj" \
	"$(INTDIR)\DTexGroupMng.obj" \
	"$(INTDIR)\DTexMng.obj" \
	"$(INTDIR)\EventCell.obj" \
	"$(INTDIR)\EventMgr.obj" \
	"$(INTDIR)\LightObjMgr.obj" \
	"$(INTDIR)\LyTerrain.obj" \
	"$(INTDIR)\MapMng.obj" \
	"$(INTDIR)\NPCPath.obj" \
	"$(INTDIR)\NPCPathMgr.obj" \
	"$(INTDIR)\PondMesh.obj" \
	"$(INTDIR)\PondMng.obj" \
	"$(INTDIR)\PosDummy.obj" \
	"$(INTDIR)\QTNode.obj" \
	"$(INTDIR)\RegenUser.obj" \
	"$(INTDIR)\RiverMesh.obj" \
	"$(INTDIR)\RiverMng.obj" \
	"$(INTDIR)\RotDummy.obj" \
	"$(INTDIR)\ScaleDummy.obj" \
	"$(INTDIR)\SoundCell.obj" \
	"$(INTDIR)\SoundMgr.obj" \
	"$(INTDIR)\TransDummy.obj" \
	"$(INTDIR)\VtxPosDummy.obj" \
	"$(INTDIR)\Wall.obj" \
	"$(INTDIR)\WallMgr.obj" \
	"$(INTDIR)\WarpMgr.obj" \
	"$(INTDIR)\DlgMapView.obj" \
	"$(INTDIR)\SowSeedMng.obj" \
	"$(INTDIR)\N3ME.res"

".\N3ME_debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("N3ME.dep")
!INCLUDE "N3ME.dep"
!ELSE 
!MESSAGE Warning: cannot find "N3ME.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "N3ME - Win32 Release" || "$(CFG)" == "N3ME - Win32 Debug"
SOURCE=..\N3Base\BitMapFile.cpp

"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\LogWriter.cpp

"$(INTDIR)\LogWriter.obj"	"$(INTDIR)\LogWriter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3AlphaPrimitiveManager.cpp

"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3AnimControl.cpp

"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3AnimKey.cpp

"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Base.cpp

"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3BaseFileAccess.cpp

"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Camera.cpp

"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Chr.cpp

"$(INTDIR)\N3Chr.obj"	"$(INTDIR)\N3Chr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Eng.cpp

"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3BASE\N3EngTool.cpp

"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXBundle.cpp

"$(INTDIR)\N3FXBundle.obj"	"$(INTDIR)\N3FXBundle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXPartBase.cpp

"$(INTDIR)\N3FXPartBase.obj"	"$(INTDIR)\N3FXPartBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXPartBillBoard.cpp

"$(INTDIR)\N3FXPartBillBoard.obj"	"$(INTDIR)\N3FXPartBillBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXPartBottomBoard.cpp

"$(INTDIR)\N3FXPartBottomBoard.obj"	"$(INTDIR)\N3FXPartBottomBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXParticle.cpp

"$(INTDIR)\N3FXParticle.obj"	"$(INTDIR)\N3FXParticle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXPartMesh.cpp

"$(INTDIR)\N3FXPartMesh.obj"	"$(INTDIR)\N3FXPartMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXPartParticles.cpp

"$(INTDIR)\N3FXPartParticles.obj"	"$(INTDIR)\N3FXPartParticles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXPlug.cpp

"$(INTDIR)\N3FXPlug.obj"	"$(INTDIR)\N3FXPlug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXPMesh.cpp

"$(INTDIR)\N3FXPMesh.obj"	"$(INTDIR)\N3FXPMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXPMeshInstance.cpp

"$(INTDIR)\N3FXPMeshInstance.obj"	"$(INTDIR)\N3FXPMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3FXShape.cpp

"$(INTDIR)\N3FXShape.obj"	"$(INTDIR)\N3FXShape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3IMesh.cpp

"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Joint.cpp

"$(INTDIR)\N3Joint.obj"	"$(INTDIR)\N3Joint.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Light.cpp

"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Mesh.cpp

"$(INTDIR)\N3Mesh.obj"	"$(INTDIR)\N3Mesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3PMesh.cpp

"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3PMeshCreate.cpp

"$(INTDIR)\N3PMeshCreate.obj"	"$(INTDIR)\N3PMeshCreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3PMeshInstance.cpp

"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Scene.cpp

"$(INTDIR)\N3Scene.obj"	"$(INTDIR)\N3Scene.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Shape.cpp

"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3BASE\N3ShapeExtra.cpp

"$(INTDIR)\N3ShapeExtra.obj"	"$(INTDIR)\N3ShapeExtra.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3ShapeMgr.cpp

"$(INTDIR)\N3ShapeMgr.obj"	"$(INTDIR)\N3ShapeMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Skin.cpp

"$(INTDIR)\N3Skin.obj"	"$(INTDIR)\N3Skin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Texture.cpp

"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3Transform.cpp

"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3TransformCollision.cpp

"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\N3VMesh.cpp

"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\N3Base\Pick.cpp

"$(INTDIR)\Pick.obj"	"$(INTDIR)\Pick.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3ME.cpp

"$(INTDIR)\N3ME.obj"	"$(INTDIR)\N3ME.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3ME.rc

"$(INTDIR)\N3ME.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\N3MEDoc.cpp

"$(INTDIR)\N3MEDoc.obj"	"$(INTDIR)\N3MEDoc.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3MEView.cpp

"$(INTDIR)\N3MEView.obj"	"$(INTDIR)\N3MEView.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BrushDlg.cpp

"$(INTDIR)\BrushDlg.obj"	"$(INTDIR)\BrushDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgAddDTex.cpp

"$(INTDIR)\DlgAddDTex.obj"	"$(INTDIR)\DlgAddDTex.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgAddSoundGroup.cpp

"$(INTDIR)\DlgAddSoundGroup.obj"	"$(INTDIR)\DlgAddSoundGroup.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgBar.cpp

"$(INTDIR)\DlgBar.obj"	"$(INTDIR)\DlgBar.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgBase.cpp

"$(INTDIR)\DlgBase.obj"	"$(INTDIR)\DlgBase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgBrowsePath.cpp

"$(INTDIR)\DlgBrowsePath.obj"	"$(INTDIR)\DlgBrowsePath.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgCtrlHeightScale.cpp

"$(INTDIR)\DlgCtrlHeightScale.obj"	"$(INTDIR)\DlgCtrlHeightScale.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgDelGroup.cpp

"$(INTDIR)\DlgDelGroup.obj"	"$(INTDIR)\DlgDelGroup.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgDTexGroupView.cpp

"$(INTDIR)\DlgDTexGroupView.obj"	"$(INTDIR)\DlgDTexGroupView.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgEditEvent.cpp

"$(INTDIR)\DlgEditEvent.obj"	"$(INTDIR)\DlgEditEvent.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgEditEventAttr.cpp

"$(INTDIR)\DlgEditEventAttr.obj"	"$(INTDIR)\DlgEditEventAttr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgEditWarp.cpp

"$(INTDIR)\DlgEditWarp.obj"	"$(INTDIR)\DlgEditWarp.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgInputAttr.cpp

"$(INTDIR)\DlgInputAttr.obj"	"$(INTDIR)\DlgInputAttr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgInputGroup.cpp

"$(INTDIR)\DlgInputGroup.obj"	"$(INTDIR)\DlgInputGroup.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgLight.cpp

"$(INTDIR)\DlgLight.obj"	"$(INTDIR)\DlgLight.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgLoadEvt.cpp

"$(INTDIR)\DlgLoadEvt.obj"	"$(INTDIR)\DlgLoadEvt.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgLoadNPCPath.cpp

"$(INTDIR)\DlgLoadNPCPath.obj"	"$(INTDIR)\DlgLoadNPCPath.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgLoadTileSet.cpp

"$(INTDIR)\DlgLoadTileSet.obj"	"$(INTDIR)\DlgLoadTileSet.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgMakeNPCPath.cpp

"$(INTDIR)\DlgMakeNPCPath.obj"	"$(INTDIR)\DlgMakeNPCPath.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgMakeWall.cpp

"$(INTDIR)\DlgMakeWall.obj"	"$(INTDIR)\DlgMakeWall.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgModifyDTex.cpp

"$(INTDIR)\DlgModifyDTex.obj"	"$(INTDIR)\DlgModifyDTex.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgPondProperty.cpp

"$(INTDIR)\DlgPondProperty.obj"	"$(INTDIR)\DlgPondProperty.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgRegenUser.cpp

"$(INTDIR)\DlgRegenUser.obj"	"$(INTDIR)\DlgRegenUser.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgRiverProperty.cpp

"$(INTDIR)\DlgRiverProperty.obj"	"$(INTDIR)\DlgRiverProperty.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSaveDivision.cpp

"$(INTDIR)\DlgSaveDivision.obj"	"$(INTDIR)\DlgSaveDivision.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSaveEvt.cpp

"$(INTDIR)\DlgSaveEvt.obj"	"$(INTDIR)\DlgSaveEvt.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSaveNewTileSet.cpp

"$(INTDIR)\DlgSaveNewTileSet.obj"	"$(INTDIR)\DlgSaveNewTileSet.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSaveNPCPath.cpp

"$(INTDIR)\DlgSaveNPCPath.obj"	"$(INTDIR)\DlgSaveNPCPath.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSceneGraph.cpp

"$(INTDIR)\DlgSceneGraph.obj"	"$(INTDIR)\DlgSceneGraph.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSetDTex.cpp

"$(INTDIR)\DlgSetDTex.obj"	"$(INTDIR)\DlgSetDTex.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSetLightMap.cpp

"$(INTDIR)\DlgSetLightMap.obj"	"$(INTDIR)\DlgSetLightMap.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSetSound.cpp

"$(INTDIR)\DlgSetSound.obj"	"$(INTDIR)\DlgSetSound.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgShapeList.cpp

"$(INTDIR)\DlgShapeList.obj"	"$(INTDIR)\DlgShapeList.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgSowSeed.cpp

"$(INTDIR)\DlgSowSeed.obj"	"$(INTDIR)\DlgSowSeed.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgTerrainSize.cpp

"$(INTDIR)\DlgTerrainSize.obj"	"$(INTDIR)\DlgTerrainSize.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgUnusedFiles.cpp

"$(INTDIR)\DlgUnusedFiles.obj"	"$(INTDIR)\DlgUnusedFiles.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE="..\Common Control\PropertyList.cpp"

"$(INTDIR)\PropertyList.obj"	"$(INTDIR)\PropertyList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ShellTree.cpp

"$(INTDIR)\ShellTree.obj"	"$(INTDIR)\ShellTree.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ProgressBar.cpp

"$(INTDIR)\ProgressBar.obj"	"$(INTDIR)\ProgressBar.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DTex.cpp

"$(INTDIR)\DTex.obj"	"$(INTDIR)\DTex.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DTexGroup.cpp

"$(INTDIR)\DTexGroup.obj"	"$(INTDIR)\DTexGroup.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DTexGroupMng.cpp

"$(INTDIR)\DTexGroupMng.obj"	"$(INTDIR)\DTexGroupMng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DTexMng.cpp

"$(INTDIR)\DTexMng.obj"	"$(INTDIR)\DTexMng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EventCell.cpp

"$(INTDIR)\EventCell.obj"	"$(INTDIR)\EventCell.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EventMgr.cpp

"$(INTDIR)\EventMgr.obj"	"$(INTDIR)\EventMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LightObjMgr.cpp

"$(INTDIR)\LightObjMgr.obj"	"$(INTDIR)\LightObjMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LyTerrain.cpp

"$(INTDIR)\LyTerrain.obj"	"$(INTDIR)\LyTerrain.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MapMng.cpp

"$(INTDIR)\MapMng.obj"	"$(INTDIR)\MapMng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NPCPath.cpp

"$(INTDIR)\NPCPath.obj"	"$(INTDIR)\NPCPath.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NPCPathMgr.cpp

"$(INTDIR)\NPCPathMgr.obj"	"$(INTDIR)\NPCPathMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PondMesh.cpp

"$(INTDIR)\PondMesh.obj"	"$(INTDIR)\PondMesh.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PondMng.cpp

"$(INTDIR)\PondMng.obj"	"$(INTDIR)\PondMng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PosDummy.cpp

"$(INTDIR)\PosDummy.obj"	"$(INTDIR)\PosDummy.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\QTNode.cpp

"$(INTDIR)\QTNode.obj"	"$(INTDIR)\QTNode.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RegenUser.cpp

"$(INTDIR)\RegenUser.obj"	"$(INTDIR)\RegenUser.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RiverMesh.cpp

"$(INTDIR)\RiverMesh.obj"	"$(INTDIR)\RiverMesh.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RiverMng.cpp

"$(INTDIR)\RiverMng.obj"	"$(INTDIR)\RiverMng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RotDummy.cpp

"$(INTDIR)\RotDummy.obj"	"$(INTDIR)\RotDummy.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ScaleDummy.cpp

"$(INTDIR)\ScaleDummy.obj"	"$(INTDIR)\ScaleDummy.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SoundCell.cpp

"$(INTDIR)\SoundCell.obj"	"$(INTDIR)\SoundCell.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SoundMgr.cpp

"$(INTDIR)\SoundMgr.obj"	"$(INTDIR)\SoundMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TransDummy.cpp

"$(INTDIR)\TransDummy.obj"	"$(INTDIR)\TransDummy.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\VtxPosDummy.cpp

"$(INTDIR)\VtxPosDummy.obj"	"$(INTDIR)\VtxPosDummy.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Wall.cpp

"$(INTDIR)\Wall.obj"	"$(INTDIR)\Wall.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WallMgr.cpp

"$(INTDIR)\WallMgr.obj"	"$(INTDIR)\WallMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WarpMgr.cpp

"$(INTDIR)\WarpMgr.obj"	"$(INTDIR)\WarpMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgMapView.cpp

"$(INTDIR)\DlgMapView.obj"	"$(INTDIR)\DlgMapView.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SowSeedMng.cpp

"$(INTDIR)\SowSeedMng.obj"	"$(INTDIR)\SowSeedMng.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

