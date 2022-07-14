# Microsoft Developer Studio Generated NMAKE File, Based on N3Indoor.dsp
!IF "$(CFG)" == ""
CFG=N3Indoor - Win32 Debug
!MESSAGE No configuration specified. Defaulting to N3Indoor - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "N3Indoor - Win32 Release" && "$(CFG)" != "N3Indoor - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3Indoor.mak" CFG="N3Indoor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3Indoor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3Indoor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "N3Indoor - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\N3Indoor.exe"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\ComDialog.obj"
	-@erase "$(INTDIR)\DFont.obj"
	-@erase "$(INTDIR)\DlgBase.obj"
	-@erase "$(INTDIR)\DlgBrowsePath.obj"
	-@erase "$(INTDIR)\DlgShapeList.obj"
	-@erase "$(INTDIR)\DlgUnusedFiles.obj"
	-@erase "$(INTDIR)\EnvironSoundPage.obj"
	-@erase "$(INTDIR)\EventEditPage.obj"
	-@erase "$(INTDIR)\FloorDlg.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MakeNpcPathPage.obj"
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
	-@erase "$(INTDIR)\N3GlobalEffect.obj"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3Indoor.obj"
	-@erase "$(INTDIR)\N3Indoor.pch"
	-@erase "$(INTDIR)\N3Indoor.res"
	-@erase "$(INTDIR)\N3IndoorDoc.obj"
	-@erase "$(INTDIR)\N3IndoorView.obj"
	-@erase "$(INTDIR)\N3Joint.obj"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3Moon.obj"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3ShapeExtra.obj"
	-@erase "$(INTDIR)\N3ShapeMod.obj"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\OrganizeView.obj"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\PortalFactory.obj"
	-@erase "$(INTDIR)\PortalVolume.obj"
	-@erase "$(INTDIR)\PosDummy.obj"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PVSManager.obj"
	-@erase "$(INTDIR)\RotDummy.obj"
	-@erase "$(INTDIR)\ScaleDummy.obj"
	-@erase "$(INTDIR)\ShapeTypeDlg.obj"
	-@erase "$(INTDIR)\ShellTree.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SwappedDummy.obj"
	-@erase "$(INTDIR)\TotalToolSheet.obj"
	-@erase "$(INTDIR)\TransDummy.obj"
	-@erase "$(INTDIR)\UserRegenPage.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\N3Indoor.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3INDOOR" /Fp"$(INTDIR)\N3Indoor.pch" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3Indoor.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3Indoor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib d3d9.lib d3dx9.lib dsound.lib dxguid.lib strmiids.lib  /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\N3Indoor.pdb" /machine:I386 /out:"$(OUTDIR)\N3Indoor.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ComDialog.obj" \
	"$(INTDIR)\DlgBase.obj" \
	"$(INTDIR)\DlgBrowsePath.obj" \
	"$(INTDIR)\DlgShapeList.obj" \
	"$(INTDIR)\DlgUnusedFiles.obj" \
	"$(INTDIR)\EnvironSoundPage.obj" \
	"$(INTDIR)\EventEditPage.obj" \
	"$(INTDIR)\FloorDlg.obj" \
	"$(INTDIR)\MakeNpcPathPage.obj" \
	"$(INTDIR)\ShapeTypeDlg.obj" \
	"$(INTDIR)\TotalToolSheet.obj" \
	"$(INTDIR)\UserRegenPage.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3Indoor.obj" \
	"$(INTDIR)\N3IndoorDoc.obj" \
	"$(INTDIR)\N3IndoorView.obj" \
	"$(INTDIR)\OrganizeView.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\ShellTree.obj" \
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
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXGroup.obj" \
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
	"$(INTDIR)\N3GlobalEffect.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\Pick.obj" \
	"$(INTDIR)\PortalFactory.obj" \
	"$(INTDIR)\PortalVolume.obj" \
	"$(INTDIR)\PVSManager.obj" \
	"$(INTDIR)\PosDummy.obj" \
	"$(INTDIR)\RotDummy.obj" \
	"$(INTDIR)\ScaleDummy.obj" \
	"$(INTDIR)\SwappedDummy.obj" \
	"$(INTDIR)\TransDummy.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3ShapeExtra.obj" \
	"$(INTDIR)\N3ShapeMod.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\N3Indoor.res"

"$(OUTDIR)\N3Indoor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\N3Indoor_debug.exe" "$(OUTDIR)\N3Indoor.pch" "$(OUTDIR)\N3Indoor.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\ComDialog.obj"
	-@erase "$(INTDIR)\ComDialog.sbr"
	-@erase "$(INTDIR)\DFont.obj"
	-@erase "$(INTDIR)\DFont.sbr"
	-@erase "$(INTDIR)\DlgBase.obj"
	-@erase "$(INTDIR)\DlgBase.sbr"
	-@erase "$(INTDIR)\DlgBrowsePath.obj"
	-@erase "$(INTDIR)\DlgBrowsePath.sbr"
	-@erase "$(INTDIR)\DlgShapeList.obj"
	-@erase "$(INTDIR)\DlgShapeList.sbr"
	-@erase "$(INTDIR)\DlgUnusedFiles.obj"
	-@erase "$(INTDIR)\DlgUnusedFiles.sbr"
	-@erase "$(INTDIR)\EnvironSoundPage.obj"
	-@erase "$(INTDIR)\EnvironSoundPage.sbr"
	-@erase "$(INTDIR)\EventEditPage.obj"
	-@erase "$(INTDIR)\EventEditPage.sbr"
	-@erase "$(INTDIR)\FloorDlg.obj"
	-@erase "$(INTDIR)\FloorDlg.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MakeNpcPathPage.obj"
	-@erase "$(INTDIR)\MakeNpcPathPage.sbr"
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
	-@erase "$(INTDIR)\N3GlobalEffect.obj"
	-@erase "$(INTDIR)\N3GlobalEffect.sbr"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3IMesh.sbr"
	-@erase "$(INTDIR)\N3Indoor.obj"
	-@erase "$(INTDIR)\N3Indoor.pch"
	-@erase "$(INTDIR)\N3Indoor.res"
	-@erase "$(INTDIR)\N3Indoor.sbr"
	-@erase "$(INTDIR)\N3IndoorDoc.obj"
	-@erase "$(INTDIR)\N3IndoorDoc.sbr"
	-@erase "$(INTDIR)\N3IndoorView.obj"
	-@erase "$(INTDIR)\N3IndoorView.sbr"
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
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Scene.sbr"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Shape.sbr"
	-@erase "$(INTDIR)\N3ShapeExtra.obj"
	-@erase "$(INTDIR)\N3ShapeExtra.sbr"
	-@erase "$(INTDIR)\N3ShapeMod.obj"
	-@erase "$(INTDIR)\N3ShapeMod.sbr"
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
	-@erase "$(INTDIR)\OrganizeView.obj"
	-@erase "$(INTDIR)\OrganizeView.sbr"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\Pick.sbr"
	-@erase "$(INTDIR)\PortalFactory.obj"
	-@erase "$(INTDIR)\PortalFactory.sbr"
	-@erase "$(INTDIR)\PortalVolume.obj"
	-@erase "$(INTDIR)\PortalVolume.sbr"
	-@erase "$(INTDIR)\PosDummy.obj"
	-@erase "$(INTDIR)\PosDummy.sbr"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\PVSManager.obj"
	-@erase "$(INTDIR)\PVSManager.sbr"
	-@erase "$(INTDIR)\RotDummy.obj"
	-@erase "$(INTDIR)\RotDummy.sbr"
	-@erase "$(INTDIR)\ScaleDummy.obj"
	-@erase "$(INTDIR)\ScaleDummy.sbr"
	-@erase "$(INTDIR)\ShapeTypeDlg.obj"
	-@erase "$(INTDIR)\ShapeTypeDlg.sbr"
	-@erase "$(INTDIR)\ShellTree.obj"
	-@erase "$(INTDIR)\ShellTree.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SwappedDummy.obj"
	-@erase "$(INTDIR)\SwappedDummy.sbr"
	-@erase "$(INTDIR)\TotalToolSheet.obj"
	-@erase "$(INTDIR)\TotalToolSheet.sbr"
	-@erase "$(INTDIR)\TransDummy.obj"
	-@erase "$(INTDIR)\TransDummy.sbr"
	-@erase "$(INTDIR)\UserRegenPage.obj"
	-@erase "$(INTDIR)\UserRegenPage.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\N3Indoor.bsc"
	-@erase "$(OUTDIR)\N3Indoor_debug.pdb"
	-@erase ".\N3Indoor_debug.exe"
	-@erase ".\N3Indoor_debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3INDOOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3Indoor.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3Indoor.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3Indoor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ComDialog.sbr" \
	"$(INTDIR)\DlgBase.sbr" \
	"$(INTDIR)\DlgBrowsePath.sbr" \
	"$(INTDIR)\DlgShapeList.sbr" \
	"$(INTDIR)\DlgUnusedFiles.sbr" \
	"$(INTDIR)\EnvironSoundPage.sbr" \
	"$(INTDIR)\EventEditPage.sbr" \
	"$(INTDIR)\FloorDlg.sbr" \
	"$(INTDIR)\MakeNpcPathPage.sbr" \
	"$(INTDIR)\ShapeTypeDlg.sbr" \
	"$(INTDIR)\TotalToolSheet.sbr" \
	"$(INTDIR)\UserRegenPage.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3Indoor.sbr" \
	"$(INTDIR)\N3IndoorDoc.sbr" \
	"$(INTDIR)\N3IndoorView.sbr" \
	"$(INTDIR)\OrganizeView.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\ShellTree.sbr" \
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
	"$(INTDIR)\N3FXBundle.sbr" \
	"$(INTDIR)\N3FXGroup.sbr" \
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
	"$(INTDIR)\N3GlobalEffect.sbr" \
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Joint.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Mesh.sbr" \
	"$(INTDIR)\N3Moon.sbr" \
	"$(INTDIR)\N3Scene.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\Pick.sbr" \
	"$(INTDIR)\PortalFactory.sbr" \
	"$(INTDIR)\PortalVolume.sbr" \
	"$(INTDIR)\PVSManager.sbr" \
	"$(INTDIR)\PosDummy.sbr" \
	"$(INTDIR)\RotDummy.sbr" \
	"$(INTDIR)\ScaleDummy.sbr" \
	"$(INTDIR)\SwappedDummy.sbr" \
	"$(INTDIR)\TransDummy.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshCreate.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3ShapeExtra.sbr" \
	"$(INTDIR)\N3ShapeMod.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3VMesh.sbr"

"$(OUTDIR)\N3Indoor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib d3d9.lib d3dx9.lib dsound.lib dxguid.lib strmiids.lib  /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\N3Indoor_debug.pdb" /debug /machine:I386 /out:"N3Indoor_debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ComDialog.obj" \
	"$(INTDIR)\DlgBase.obj" \
	"$(INTDIR)\DlgBrowsePath.obj" \
	"$(INTDIR)\DlgShapeList.obj" \
	"$(INTDIR)\DlgUnusedFiles.obj" \
	"$(INTDIR)\EnvironSoundPage.obj" \
	"$(INTDIR)\EventEditPage.obj" \
	"$(INTDIR)\FloorDlg.obj" \
	"$(INTDIR)\MakeNpcPathPage.obj" \
	"$(INTDIR)\ShapeTypeDlg.obj" \
	"$(INTDIR)\TotalToolSheet.obj" \
	"$(INTDIR)\UserRegenPage.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3Indoor.obj" \
	"$(INTDIR)\N3IndoorDoc.obj" \
	"$(INTDIR)\N3IndoorView.obj" \
	"$(INTDIR)\OrganizeView.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\ShellTree.obj" \
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
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXGroup.obj" \
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
	"$(INTDIR)\N3GlobalEffect.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\Pick.obj" \
	"$(INTDIR)\PortalFactory.obj" \
	"$(INTDIR)\PortalVolume.obj" \
	"$(INTDIR)\PVSManager.obj" \
	"$(INTDIR)\PosDummy.obj" \
	"$(INTDIR)\RotDummy.obj" \
	"$(INTDIR)\ScaleDummy.obj" \
	"$(INTDIR)\SwappedDummy.obj" \
	"$(INTDIR)\TransDummy.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3ShapeExtra.obj" \
	"$(INTDIR)\N3ShapeMod.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\N3Indoor.res"

".\N3Indoor_debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("N3Indoor.dep")
!INCLUDE "N3Indoor.dep"
!ELSE 
!MESSAGE Warning: cannot find "N3Indoor.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "N3Indoor - Win32 Release" || "$(CFG)" == "N3Indoor - Win32 Debug"
SOURCE=.\N3Indoor.rc

"$(INTDIR)\N3Indoor.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ComDialog.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\ComDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\ComDialog.obj"	"$(INTDIR)\ComDialog.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgBase.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\DlgBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\DlgBase.obj"	"$(INTDIR)\DlgBase.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgBrowsePath.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\DlgBrowsePath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\DlgBrowsePath.obj"	"$(INTDIR)\DlgBrowsePath.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgShapeList.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\DlgShapeList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\DlgShapeList.obj"	"$(INTDIR)\DlgShapeList.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgUnusedFiles.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\DlgUnusedFiles.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\DlgUnusedFiles.obj"	"$(INTDIR)\DlgUnusedFiles.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\EnvironSoundPage.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\EnvironSoundPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\EnvironSoundPage.obj"	"$(INTDIR)\EnvironSoundPage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\EventEditPage.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\EventEditPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\EventEditPage.obj"	"$(INTDIR)\EventEditPage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\FloorDlg.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\FloorDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\FloorDlg.obj"	"$(INTDIR)\FloorDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MakeNpcPathPage.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\MakeNpcPathPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\MakeNpcPathPage.obj"	"$(INTDIR)\MakeNpcPathPage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ShapeTypeDlg.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\ShapeTypeDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\ShapeTypeDlg.obj"	"$(INTDIR)\ShapeTypeDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TotalToolSheet.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\TotalToolSheet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\TotalToolSheet.obj"	"$(INTDIR)\TotalToolSheet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\UserRegenPage.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\UserRegenPage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\UserRegenPage.obj"	"$(INTDIR)\UserRegenPage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Indoor.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Indoor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Indoor.obj"	"$(INTDIR)\N3Indoor.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3IndoorDoc.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3IndoorDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3IndoorDoc.obj"	"$(INTDIR)\N3IndoorDoc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3IndoorView.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3IndoorView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3IndoorView.obj"	"$(INTDIR)\N3IndoorView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\OrganizeView.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\OrganizeView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\OrganizeView.obj"	"$(INTDIR)\OrganizeView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE="..\Widget\PropertyList.cpp"

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\PropertyList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\PropertyList.obj"	"$(INTDIR)\PropertyList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ShellTree.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\ShellTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\ShellTree.obj"	"$(INTDIR)\ShellTree.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3INDOOR" /Fp"$(INTDIR)\N3Indoor.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\N3Indoor.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3INDOOR" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3Indoor.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\N3Indoor.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\engine\N3Base\BitMapFile.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\BitMapFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\DFont.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\DFont.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\DFont.obj"	"$(INTDIR)\DFont.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\LogWriter.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\LogWriter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\LogWriter.obj"	"$(INTDIR)\LogWriter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AlphaPrimitiveManager.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3AlphaPrimitiveManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimControl.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3AnimControl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimKey.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3AnimKey.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Base.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3BaseFileAccess.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3BaseFileAccess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Board.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Board.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Board.obj"	"$(INTDIR)\N3Board.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Camera.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Camera.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Chr.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Chr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Chr.obj"	"$(INTDIR)\N3Chr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Cloak.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Cloak.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Cloak.obj"	"$(INTDIR)\N3Cloak.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Cloud.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Cloud.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Cloud.obj"	"$(INTDIR)\N3Cloud.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3ColorChange.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3ColorChange.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3ColorChange.obj"	"$(INTDIR)\N3ColorChange.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Eng.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3EngTool.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3EngTool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXBundle.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXBundle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXBundle.obj"	"$(INTDIR)\N3FXBundle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXGroup.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXGroup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXGroup.obj"	"$(INTDIR)\N3FXGroup.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBase.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXPartBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXPartBase.obj"	"$(INTDIR)\N3FXPartBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBillBoard.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXPartBillBoard.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXPartBillBoard.obj"	"$(INTDIR)\N3FXPartBillBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBottomBoard.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXPartBottomBoard.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXPartBottomBoard.obj"	"$(INTDIR)\N3FXPartBottomBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXParticle.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXParticle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXParticle.obj"	"$(INTDIR)\N3FXParticle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartMesh.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXPartMesh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXPartMesh.obj"	"$(INTDIR)\N3FXPartMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartParticles.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXPartParticles.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXPartParticles.obj"	"$(INTDIR)\N3FXPartParticles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPlug.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXPlug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXPlug.obj"	"$(INTDIR)\N3FXPlug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPMesh.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXPMesh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXPMesh.obj"	"$(INTDIR)\N3FXPMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPMeshInstance.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXPMeshInstance.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXPMeshInstance.obj"	"$(INTDIR)\N3FXPMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXShape.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3FXShape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3FXShape.obj"	"$(INTDIR)\N3FXShape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3GlobalEffect.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3GlobalEffect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3GlobalEffect.obj"	"$(INTDIR)\N3GlobalEffect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3IMesh.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3IMesh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Joint.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Joint.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Joint.obj"	"$(INTDIR)\N3Joint.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Light.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Light.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Mesh.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Mesh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Mesh.obj"	"$(INTDIR)\N3Mesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Moon.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Moon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Moon.obj"	"$(INTDIR)\N3Moon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Scene.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Scene.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Scene.obj"	"$(INTDIR)\N3Scene.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Skin.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Skin.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Skin.obj"	"$(INTDIR)\N3Skin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Texture.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Texture.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3TransformCollision.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3TransformCollision.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\Pick.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\Pick.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\Pick.obj"	"$(INTDIR)\Pick.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\PortalFactory.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\PortalFactory.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\PortalFactory.obj"	"$(INTDIR)\PortalFactory.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PortalVolume.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\PortalVolume.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\PortalVolume.obj"	"$(INTDIR)\PortalVolume.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PVSManager.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\PVSManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\PVSManager.obj"	"$(INTDIR)\PVSManager.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PosDummy.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\PosDummy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\PosDummy.obj"	"$(INTDIR)\PosDummy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RotDummy.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\RotDummy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\RotDummy.obj"	"$(INTDIR)\RotDummy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ScaleDummy.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\ScaleDummy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\ScaleDummy.obj"	"$(INTDIR)\ScaleDummy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SwappedDummy.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\SwappedDummy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\SwappedDummy.obj"	"$(INTDIR)\SwappedDummy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TransDummy.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\TransDummy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\TransDummy.obj"	"$(INTDIR)\TransDummy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMesh.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3PMesh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshCreate.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3PMeshCreate.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3PMeshCreate.obj"	"$(INTDIR)\N3PMeshCreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshInstance.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3PMeshInstance.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Shape.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Shape.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3ShapeExtra.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3ShapeExtra.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3ShapeExtra.obj"	"$(INTDIR)\N3ShapeExtra.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3ShapeMod.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3ShapeMod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3ShapeMod.obj"	"$(INTDIR)\N3ShapeMod.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Transform.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3Transform.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3VMesh.cpp

!IF  "$(CFG)" == "N3Indoor - Win32 Release"


"$(INTDIR)\N3VMesh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\N3Indoor.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"


"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

