# Microsoft Developer Studio Generated NMAKE File, Based on WarFare.dsp
!IF "$(CFG)" == ""
CFG=WarFare - Win32 Debug
!MESSAGE No configuration specified. Defaulting to WarFare - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WarFare - Win32 Release" && "$(CFG)" != "WarFare - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WarFare.mak" CFG="WarFare - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WarFare - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WarFare - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "WarFare - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\KnightOnLine.exe" "$(OUTDIR)\WarFare.bsc"


CLEAN :
	-@erase "$(INTDIR)\APISocket.obj"
	-@erase "$(INTDIR)\APISocket.sbr"
	-@erase "$(INTDIR)\Bird.obj"
	-@erase "$(INTDIR)\Bird.sbr"
	-@erase "$(INTDIR)\BirdMng.obj"
	-@erase "$(INTDIR)\BirdMng.sbr"
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\Catapult.obj"
	-@erase "$(INTDIR)\Catapult.sbr"
	-@erase "$(INTDIR)\Compress.obj"
	-@erase "$(INTDIR)\Compress.sbr"
	-@erase "$(INTDIR)\CountableItemEditDlg.obj"
	-@erase "$(INTDIR)\CountableItemEditDlg.sbr"
	-@erase "$(INTDIR)\DFont.obj"
	-@erase "$(INTDIR)\DFont.sbr"
	-@erase "$(INTDIR)\DungeonManager.obj"
	-@erase "$(INTDIR)\DungeonManager.sbr"
	-@erase "$(INTDIR)\EventManager.obj"
	-@erase "$(INTDIR)\EventManager.sbr"
	-@erase "$(INTDIR)\GameBase.obj"
	-@erase "$(INTDIR)\GameBase.sbr"
	-@erase "$(INTDIR)\GameCursor.obj"
	-@erase "$(INTDIR)\GameCursor.sbr"
	-@erase "$(INTDIR)\GameEng.obj"
	-@erase "$(INTDIR)\GameEng.sbr"
	-@erase "$(INTDIR)\GameProcCharacterCreate.obj"
	-@erase "$(INTDIR)\GameProcCharacterCreate.sbr"
	-@erase "$(INTDIR)\GameProcCharacterSelect.obj"
	-@erase "$(INTDIR)\GameProcCharacterSelect.sbr"
	-@erase "$(INTDIR)\GameProcedure.obj"
	-@erase "$(INTDIR)\GameProcedure.sbr"
	-@erase "$(INTDIR)\GameProcLogIn.obj"
	-@erase "$(INTDIR)\GameProcLogIn.sbr"
	-@erase "$(INTDIR)\GameProcMain.obj"
	-@erase "$(INTDIR)\GameProcMain.sbr"
	-@erase "$(INTDIR)\GameProcNationSelect.obj"
	-@erase "$(INTDIR)\GameProcNationSelect.sbr"
	-@erase "$(INTDIR)\GameProcOption.obj"
	-@erase "$(INTDIR)\GameProcOption.sbr"
	-@erase "$(INTDIR)\GrassBoard.obj"
	-@erase "$(INTDIR)\GrassBoard.sbr"
	-@erase "$(INTDIR)\ItemRepairMgr.obj"
	-@erase "$(INTDIR)\ItemRepairMgr.sbr"
	-@erase "$(INTDIR)\JPEG.OBJ"
	-@erase "$(INTDIR)\JPEG.SBR"
	-@erase "$(INTDIR)\JpegFile.obj"
	-@erase "$(INTDIR)\JpegFile.sbr"
	-@erase "$(INTDIR)\KnightChrMgr.obj"
	-@erase "$(INTDIR)\KnightChrMgr.sbr"
	-@erase "$(INTDIR)\LightMgr.obj"
	-@erase "$(INTDIR)\LightMgr.sbr"
	-@erase "$(INTDIR)\LocalInput.obj"
	-@erase "$(INTDIR)\LocalInput.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
	-@erase "$(INTDIR)\MachineBase.obj"
	-@erase "$(INTDIR)\MachineBase.sbr"
	-@erase "$(INTDIR)\MachineMng.obj"
	-@erase "$(INTDIR)\MachineMng.sbr"
	-@erase "$(INTDIR)\MagicSkillMng.obj"
	-@erase "$(INTDIR)\MagicSkillMng.sbr"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.sbr"
	-@erase "$(INTDIR)\N3AnimatedTexures.obj"
	-@erase "$(INTDIR)\N3AnimatedTexures.sbr"
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
	-@erase "$(INTDIR)\N3FXBundle.obj"
	-@erase "$(INTDIR)\N3FXBundle.sbr"
	-@erase "$(INTDIR)\N3FXBundleGame.obj"
	-@erase "$(INTDIR)\N3FXBundleGame.sbr"
	-@erase "$(INTDIR)\N3FXMgr.obj"
	-@erase "$(INTDIR)\N3FXMgr.sbr"
	-@erase "$(INTDIR)\N3FXPartBase.obj"
	-@erase "$(INTDIR)\N3FXPartBase.sbr"
	-@erase "$(INTDIR)\N3FXPartBillBoard.obj"
	-@erase "$(INTDIR)\N3FXPartBillBoard.sbr"
	-@erase "$(INTDIR)\N3FXPartBillBoardGame.obj"
	-@erase "$(INTDIR)\N3FXPartBillBoardGame.sbr"
	-@erase "$(INTDIR)\N3FXPartBottomBoard.obj"
	-@erase "$(INTDIR)\N3FXPartBottomBoard.sbr"
	-@erase "$(INTDIR)\N3FXPartBottomBoardGame.obj"
	-@erase "$(INTDIR)\N3FXPartBottomBoardGame.sbr"
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
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.sbr"
	-@erase "$(INTDIR)\N3Pond.obj"
	-@erase "$(INTDIR)\N3Pond.sbr"
	-@erase "$(INTDIR)\N3River.obj"
	-@erase "$(INTDIR)\N3River.sbr"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Shape.sbr"
	-@erase "$(INTDIR)\N3ShapeExtra.obj"
	-@erase "$(INTDIR)\N3ShapeExtra.sbr"
	-@erase "$(INTDIR)\N3ShapeMgr.obj"
	-@erase "$(INTDIR)\N3ShapeMgr.sbr"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Skin.sbr"
	-@erase "$(INTDIR)\N3Sky.obj"
	-@erase "$(INTDIR)\N3Sky.sbr"
	-@erase "$(INTDIR)\N3SkyMng.obj"
	-@erase "$(INTDIR)\N3SkyMng.sbr"
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
	-@erase "$(INTDIR)\N3Terrain.obj"
	-@erase "$(INTDIR)\N3Terrain.sbr"
	-@erase "$(INTDIR)\N3TerrainManager.obj"
	-@erase "$(INTDIR)\N3TerrainManager.sbr"
	-@erase "$(INTDIR)\N3TerrainPatch.obj"
	-@erase "$(INTDIR)\N3TerrainPatch.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3TransformCollision.sbr"
	-@erase "$(INTDIR)\N3UIArea.obj"
	-@erase "$(INTDIR)\N3UIArea.sbr"
	-@erase "$(INTDIR)\N3UIBase.obj"
	-@erase "$(INTDIR)\N3UIBase.sbr"
	-@erase "$(INTDIR)\N3UIButton.obj"
	-@erase "$(INTDIR)\N3UIButton.sbr"
	-@erase "$(INTDIR)\N3UIDBCLButton.obj"
	-@erase "$(INTDIR)\N3UIDBCLButton.sbr"
	-@erase "$(INTDIR)\N3UIEdit.obj"
	-@erase "$(INTDIR)\N3UIEdit.sbr"
	-@erase "$(INTDIR)\N3UIIcon.obj"
	-@erase "$(INTDIR)\N3UIIcon.sbr"
	-@erase "$(INTDIR)\N3UIImage.obj"
	-@erase "$(INTDIR)\N3UIImage.sbr"
	-@erase "$(INTDIR)\N3UIList.obj"
	-@erase "$(INTDIR)\N3UIList.sbr"
	-@erase "$(INTDIR)\N3UIProgress.obj"
	-@erase "$(INTDIR)\N3UIProgress.sbr"
	-@erase "$(INTDIR)\N3UIScrollBar.obj"
	-@erase "$(INTDIR)\N3UIScrollBar.sbr"
	-@erase "$(INTDIR)\N3UIStatic.obj"
	-@erase "$(INTDIR)\N3UIStatic.sbr"
	-@erase "$(INTDIR)\N3UIString.obj"
	-@erase "$(INTDIR)\N3UIString.sbr"
	-@erase "$(INTDIR)\N3UITooltip.obj"
	-@erase "$(INTDIR)\N3UITooltip.sbr"
	-@erase "$(INTDIR)\N3UITrackBar.obj"
	-@erase "$(INTDIR)\N3UITrackBar.sbr"
	-@erase "$(INTDIR)\N3UIWndBase.obj"
	-@erase "$(INTDIR)\N3UIWndBase.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\N3WorldBase.obj"
	-@erase "$(INTDIR)\N3WorldBase.sbr"
	-@erase "$(INTDIR)\N3WorldManager.obj"
	-@erase "$(INTDIR)\N3WorldManager.sbr"
	-@erase "$(INTDIR)\PlayerBase.obj"
	-@erase "$(INTDIR)\PlayerBase.sbr"
	-@erase "$(INTDIR)\PlayerMySelf.obj"
	-@erase "$(INTDIR)\PlayerMySelf.sbr"
	-@erase "$(INTDIR)\PlayerNPC.obj"
	-@erase "$(INTDIR)\PlayerNPC.sbr"
	-@erase "$(INTDIR)\PlayerOther.obj"
	-@erase "$(INTDIR)\PlayerOther.sbr"
	-@erase "$(INTDIR)\PlayerOtherMgr.obj"
	-@erase "$(INTDIR)\PlayerOtherMgr.sbr"
	-@erase "$(INTDIR)\PortalVolume.obj"
	-@erase "$(INTDIR)\PortalVolume.sbr"
	-@erase "$(INTDIR)\PvsMgr.obj"
	-@erase "$(INTDIR)\PvsMgr.sbr"
	-@erase "$(INTDIR)\Resource.res"
	-@erase "$(INTDIR)\ServerMesh.obj"
	-@erase "$(INTDIR)\ServerMesh.sbr"
	-@erase "$(INTDIR)\SharedMem.obj"
	-@erase "$(INTDIR)\SharedMem.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SubProcPerTrade.obj"
	-@erase "$(INTDIR)\SubProcPerTrade.sbr"
	-@erase "$(INTDIR)\UICharacterCreate.obj"
	-@erase "$(INTDIR)\UICharacterCreate.sbr"
	-@erase "$(INTDIR)\UICharacterSelect.obj"
	-@erase "$(INTDIR)\UICharacterSelect.sbr"
	-@erase "$(INTDIR)\UIChat.obj"
	-@erase "$(INTDIR)\UIChat.sbr"
	-@erase "$(INTDIR)\UIClassChange.obj"
	-@erase "$(INTDIR)\UIClassChange.sbr"
	-@erase "$(INTDIR)\UICmd.obj"
	-@erase "$(INTDIR)\UICmd.sbr"
	-@erase "$(INTDIR)\UICreateClanName.obj"
	-@erase "$(INTDIR)\UICreateClanName.sbr"
	-@erase "$(INTDIR)\UIDead.obj"
	-@erase "$(INTDIR)\UIDead.sbr"
	-@erase "$(INTDIR)\UIDroppedItemDlg.obj"
	-@erase "$(INTDIR)\UIDroppedItemDlg.sbr"
	-@erase "$(INTDIR)\UIEndingDisplay.obj"
	-@erase "$(INTDIR)\UIEndingDisplay.sbr"
	-@erase "$(INTDIR)\UIHelp.obj"
	-@erase "$(INTDIR)\UIHelp.sbr"
	-@erase "$(INTDIR)\UIHotKeyDlg.obj"
	-@erase "$(INTDIR)\UIHotKeyDlg.sbr"
	-@erase "$(INTDIR)\UIImageTooltipDlg.obj"
	-@erase "$(INTDIR)\UIImageTooltipDlg.sbr"
	-@erase "$(INTDIR)\UIInn.obj"
	-@erase "$(INTDIR)\UIInn.sbr"
	-@erase "$(INTDIR)\UIInventory.obj"
	-@erase "$(INTDIR)\UIInventory.sbr"
	-@erase "$(INTDIR)\UIItemExchange.obj"
	-@erase "$(INTDIR)\UIItemExchange.sbr"
	-@erase "$(INTDIR)\UIKnightsOperation.obj"
	-@erase "$(INTDIR)\UIKnightsOperation.sbr"
	-@erase "$(INTDIR)\UILoading.obj"
	-@erase "$(INTDIR)\UILoading.sbr"
	-@erase "$(INTDIR)\UILogin.obj"
	-@erase "$(INTDIR)\UILogin.sbr"
	-@erase "$(INTDIR)\UIManager.obj"
	-@erase "$(INTDIR)\UIManager.sbr"
	-@erase "$(INTDIR)\UIMessageBox.obj"
	-@erase "$(INTDIR)\UIMessageBox.sbr"
	-@erase "$(INTDIR)\UIMessageBoxManager.obj"
	-@erase "$(INTDIR)\UIMessageBoxManager.sbr"
	-@erase "$(INTDIR)\UIMessageWnd.obj"
	-@erase "$(INTDIR)\UIMessageWnd.sbr"
	-@erase "$(INTDIR)\UINationSelectDlg.obj"
	-@erase "$(INTDIR)\UINationSelectDlg.sbr"
	-@erase "$(INTDIR)\UINotice.obj"
	-@erase "$(INTDIR)\UINotice.sbr"
	-@erase "$(INTDIR)\UINPCChangeEvent.obj"
	-@erase "$(INTDIR)\UINPCChangeEvent.sbr"
	-@erase "$(INTDIR)\UINPCEvent.obj"
	-@erase "$(INTDIR)\UINPCEvent.sbr"
	-@erase "$(INTDIR)\UINpcTalk.obj"
	-@erase "$(INTDIR)\UINpcTalk.sbr"
	-@erase "$(INTDIR)\UIPartyBBS.obj"
	-@erase "$(INTDIR)\UIPartyBBS.sbr"
	-@erase "$(INTDIR)\UIPartyBBSSelector.obj"
	-@erase "$(INTDIR)\UIPartyBBSSelector.sbr"
	-@erase "$(INTDIR)\UIPartyOrForce.obj"
	-@erase "$(INTDIR)\UIPartyOrForce.sbr"
	-@erase "$(INTDIR)\UIPerTradeDlg.obj"
	-@erase "$(INTDIR)\UIPerTradeDlg.sbr"
	-@erase "$(INTDIR)\UIPointInitDlg.obj"
	-@erase "$(INTDIR)\UIPointInitDlg.sbr"
	-@erase "$(INTDIR)\UIQuestMenu.obj"
	-@erase "$(INTDIR)\UIQuestMenu.sbr"
	-@erase "$(INTDIR)\UIQuestTalk.obj"
	-@erase "$(INTDIR)\UIQuestTalk.sbr"
	-@erase "$(INTDIR)\UIRepairTooltipDlg.obj"
	-@erase "$(INTDIR)\UIRepairTooltipDlg.sbr"
	-@erase "$(INTDIR)\UISkillTreeDlg.obj"
	-@erase "$(INTDIR)\UISkillTreeDlg.sbr"
	-@erase "$(INTDIR)\UIStateBar.obj"
	-@erase "$(INTDIR)\UIStateBar.sbr"
	-@erase "$(INTDIR)\UITargetBar.obj"
	-@erase "$(INTDIR)\UITargetBar.sbr"
	-@erase "$(INTDIR)\UITradeBBSEditDlg.obj"
	-@erase "$(INTDIR)\UITradeBBSEditDlg.sbr"
	-@erase "$(INTDIR)\UITradeBBSSelector.obj"
	-@erase "$(INTDIR)\UITradeBBSSelector.sbr"
	-@erase "$(INTDIR)\UITradeEditDlg.obj"
	-@erase "$(INTDIR)\UITradeEditDlg.sbr"
	-@erase "$(INTDIR)\UITradeExplanation.obj"
	-@erase "$(INTDIR)\UITradeExplanation.sbr"
	-@erase "$(INTDIR)\UITradeSellBBS.obj"
	-@erase "$(INTDIR)\UITradeSellBBS.sbr"
	-@erase "$(INTDIR)\UITransactionDlg.obj"
	-@erase "$(INTDIR)\UITransactionDlg.sbr"
	-@erase "$(INTDIR)\UIVarious.obj"
	-@erase "$(INTDIR)\UIVarious.sbr"
	-@erase "$(INTDIR)\UIWareHouseDlg.obj"
	-@erase "$(INTDIR)\UIWareHouseDlg.sbr"
	-@erase "$(INTDIR)\UIWarp.obj"
	-@erase "$(INTDIR)\UIWarp.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WarFareMain.obj"
	-@erase "$(INTDIR)\WarFareMain.sbr"
	-@erase "$(INTDIR)\WarMessage.obj"
	-@erase "$(INTDIR)\WarMessage.sbr"
	-@erase "$(INTDIR)\WaveFile.obj"
	-@erase "$(INTDIR)\WaveFile.sbr"
	-@erase "$(OUTDIR)\KnightOnLine.map"
	-@erase "$(OUTDIR)\WarFare.bsc"
	-@erase ".\KnightOnLine.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3GAME" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\WarFare.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /D "_N3GAME" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Resource.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WarFare.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\N3UIBase.sbr" \
	"$(INTDIR)\N3UIButton.sbr" \
	"$(INTDIR)\N3UIEdit.sbr" \
	"$(INTDIR)\N3UIImage.sbr" \
	"$(INTDIR)\N3UIList.sbr" \
	"$(INTDIR)\N3UIProgress.sbr" \
	"$(INTDIR)\N3UIScrollBar.sbr" \
	"$(INTDIR)\N3UIStatic.sbr" \
	"$(INTDIR)\N3UIString.sbr" \
	"$(INTDIR)\N3UITooltip.sbr" \
	"$(INTDIR)\N3UITrackBar.sbr" \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\DFont.sbr" \
	"$(INTDIR)\JPEG.SBR" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimatedTexures.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Board.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Cloak.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3FXPlug.sbr" \
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Joint.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Mesh.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3ShapeExtra.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr" \
	"$(INTDIR)\N3SndMgr.sbr" \
	"$(INTDIR)\N3SndObj.sbr" \
	"$(INTDIR)\N3SndObjStream.sbr" \
	"$(INTDIR)\WaveFile.sbr" \
	"$(INTDIR)\APISocket.sbr" \
	"$(INTDIR)\Compress.sbr" \
	"$(INTDIR)\GameProcCharacterCreate.sbr" \
	"$(INTDIR)\UICharacterCreate.sbr" \
	"$(INTDIR)\GameProcCharacterSelect.sbr" \
	"$(INTDIR)\UICharacterSelect.sbr" \
	"$(INTDIR)\N3Chr.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\PlayerBase.sbr" \
	"$(INTDIR)\PlayerNPC.sbr" \
	"$(INTDIR)\Catapult.sbr" \
	"$(INTDIR)\MachineBase.sbr" \
	"$(INTDIR)\MachineMng.sbr" \
	"$(INTDIR)\PlayerMySelf.sbr" \
	"$(INTDIR)\PlayerOther.sbr" \
	"$(INTDIR)\PlayerOtherMgr.sbr" \
	"$(INTDIR)\N3FXBundle.sbr" \
	"$(INTDIR)\N3FXBundleGame.sbr" \
	"$(INTDIR)\N3FXMgr.sbr" \
	"$(INTDIR)\N3FXPartBase.sbr" \
	"$(INTDIR)\N3FXPartBillBoard.sbr" \
	"$(INTDIR)\N3FXPartBillBoardGame.sbr" \
	"$(INTDIR)\N3FXPartBottomBoard.sbr" \
	"$(INTDIR)\N3FXPartBottomBoardGame.sbr" \
	"$(INTDIR)\N3FXParticle.sbr" \
	"$(INTDIR)\N3FXPartMesh.sbr" \
	"$(INTDIR)\N3FXPartParticles.sbr" \
	"$(INTDIR)\N3FXPMesh.sbr" \
	"$(INTDIR)\N3FXPMeshInstance.sbr" \
	"$(INTDIR)\N3FXShape.sbr" \
	"$(INTDIR)\CountableItemEditDlg.sbr" \
	"$(INTDIR)\UIDroppedItemDlg.sbr" \
	"$(INTDIR)\UIImageTooltipDlg.sbr" \
	"$(INTDIR)\UIInventory.sbr" \
	"$(INTDIR)\UITransactionDlg.sbr" \
	"$(INTDIR)\UIWareHouseDlg.sbr" \
	"$(INTDIR)\N3UIArea.sbr" \
	"$(INTDIR)\N3UIIcon.sbr" \
	"$(INTDIR)\N3UIWndBase.sbr" \
	"$(INTDIR)\UINPCEvent.sbr" \
	"$(INTDIR)\UINpcTalk.sbr" \
	"$(INTDIR)\UIHotKeyDlg.sbr" \
	"$(INTDIR)\UISkillTreeDlg.sbr" \
	"$(INTDIR)\ItemRepairMgr.sbr" \
	"$(INTDIR)\UIItemExchange.sbr" \
	"$(INTDIR)\UIRepairTooltipDlg.sbr" \
	"$(INTDIR)\UIClassChange.sbr" \
	"$(INTDIR)\UINPCChangeEvent.sbr" \
	"$(INTDIR)\UIPointInitDlg.sbr" \
	"$(INTDIR)\UICreateClanName.sbr" \
	"$(INTDIR)\UIInn.sbr" \
	"$(INTDIR)\N3UIDBCLButton.sbr" \
	"$(INTDIR)\UIChat.sbr" \
	"$(INTDIR)\UICmd.sbr" \
	"$(INTDIR)\UIDead.sbr" \
	"$(INTDIR)\UIEndingDisplay.sbr" \
	"$(INTDIR)\UIHelp.sbr" \
	"$(INTDIR)\UIKnightsOperation.sbr" \
	"$(INTDIR)\UILoading.sbr" \
	"$(INTDIR)\UIManager.sbr" \
	"$(INTDIR)\UIMessageBox.sbr" \
	"$(INTDIR)\UIMessageBoxManager.sbr" \
	"$(INTDIR)\UIMessageWnd.sbr" \
	"$(INTDIR)\UINotice.sbr" \
	"$(INTDIR)\UIPartyBBS.sbr" \
	"$(INTDIR)\UIPartyBBSSelector.sbr" \
	"$(INTDIR)\UIPartyOrForce.sbr" \
	"$(INTDIR)\UIQuestMenu.sbr" \
	"$(INTDIR)\UIQuestTalk.sbr" \
	"$(INTDIR)\UIStateBar.sbr" \
	"$(INTDIR)\UITargetBar.sbr" \
	"$(INTDIR)\UITradeBBSEditDlg.sbr" \
	"$(INTDIR)\UITradeBBSSelector.sbr" \
	"$(INTDIR)\UITradeExplanation.sbr" \
	"$(INTDIR)\UITradeSellBBS.sbr" \
	"$(INTDIR)\UIVarious.sbr" \
	"$(INTDIR)\UIWarp.sbr" \
	"$(INTDIR)\SubProcPerTrade.sbr" \
	"$(INTDIR)\UIPerTradeDlg.sbr" \
	"$(INTDIR)\UITradeEditDlg.sbr" \
	"$(INTDIR)\MagicSkillMng.sbr" \
	"$(INTDIR)\N3Cloud.sbr" \
	"$(INTDIR)\N3ColorChange.sbr" \
	"$(INTDIR)\N3GERain.sbr" \
	"$(INTDIR)\N3GESnow.sbr" \
	"$(INTDIR)\N3GlobalEffect.sbr" \
	"$(INTDIR)\N3Moon.sbr" \
	"$(INTDIR)\N3Sky.sbr" \
	"$(INTDIR)\N3SkyMng.sbr" \
	"$(INTDIR)\N3Star.sbr" \
	"$(INTDIR)\N3Sun.sbr" \
	"$(INTDIR)\N3Terrain.sbr" \
	"$(INTDIR)\N3TerrainPatch.sbr" \
	"$(INTDIR)\N3ShapeMgr.sbr" \
	"$(INTDIR)\Bird.sbr" \
	"$(INTDIR)\BirdMng.sbr" \
	"$(INTDIR)\GrassBoard.sbr" \
	"$(INTDIR)\LightMgr.sbr" \
	"$(INTDIR)\N3Pond.sbr" \
	"$(INTDIR)\N3River.sbr" \
	"$(INTDIR)\ServerMesh.sbr" \
	"$(INTDIR)\N3TerrainManager.sbr" \
	"$(INTDIR)\PortalVolume.sbr" \
	"$(INTDIR)\PvsMgr.sbr" \
	"$(INTDIR)\DungeonManager.sbr" \
	"$(INTDIR)\N3WorldBase.sbr" \
	"$(INTDIR)\N3WorldManager.sbr" \
	"$(INTDIR)\GameProcMain.sbr" \
	"$(INTDIR)\GameProcOption.sbr" \
	"$(INTDIR)\GameProcNationSelect.sbr" \
	"$(INTDIR)\UINationSelectDlg.sbr" \
	"$(INTDIR)\KnightChrMgr.sbr" \
	"$(INTDIR)\SharedMem.sbr" \
	"$(INTDIR)\GameProcLogIn.sbr" \
	"$(INTDIR)\UILogin.sbr" \
	"$(INTDIR)\EventManager.sbr" \
	"$(INTDIR)\GameBase.sbr" \
	"$(INTDIR)\GameCursor.sbr" \
	"$(INTDIR)\GameEng.sbr" \
	"$(INTDIR)\GameProcedure.sbr" \
	"$(INTDIR)\JpegFile.sbr" \
	"$(INTDIR)\LocalInput.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\WarFareMain.sbr" \
	"$(INTDIR)\WarMessage.sbr"

"$(OUTDIR)\WarFare.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=implode.lib kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib winmm.lib imm32.lib ddraw.lib d3d9.lib d3dx9.lib dsound.lib dxguid.lib  wsock32.lib dinput8.lib ..\engine\JPEG\jpegLib.lib /nologo /version:0.1 /subsystem:windows /pdb:none /map:"$(INTDIR)\KnightOnLine.map" /machine:I386 /out:"KnightOnLine.exe" 
LINK32_OBJS= \
	"$(INTDIR)\N3UIBase.obj" \
	"$(INTDIR)\N3UIButton.obj" \
	"$(INTDIR)\N3UIEdit.obj" \
	"$(INTDIR)\N3UIImage.obj" \
	"$(INTDIR)\N3UIList.obj" \
	"$(INTDIR)\N3UIProgress.obj" \
	"$(INTDIR)\N3UIScrollBar.obj" \
	"$(INTDIR)\N3UIStatic.obj" \
	"$(INTDIR)\N3UIString.obj" \
	"$(INTDIR)\N3UITooltip.obj" \
	"$(INTDIR)\N3UITrackBar.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DFont.obj" \
	"$(INTDIR)\JPEG.OBJ" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimatedTexures.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Board.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Cloak.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3FXPlug.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3ShapeExtra.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\N3SndMgr.obj" \
	"$(INTDIR)\N3SndObj.obj" \
	"$(INTDIR)\N3SndObjStream.obj" \
	"$(INTDIR)\WaveFile.obj" \
	"$(INTDIR)\APISocket.obj" \
	"$(INTDIR)\Compress.obj" \
	"$(INTDIR)\GameProcCharacterCreate.obj" \
	"$(INTDIR)\UICharacterCreate.obj" \
	"$(INTDIR)\GameProcCharacterSelect.obj" \
	"$(INTDIR)\UICharacterSelect.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\PlayerBase.obj" \
	"$(INTDIR)\PlayerNPC.obj" \
	"$(INTDIR)\Catapult.obj" \
	"$(INTDIR)\MachineBase.obj" \
	"$(INTDIR)\MachineMng.obj" \
	"$(INTDIR)\PlayerMySelf.obj" \
	"$(INTDIR)\PlayerOther.obj" \
	"$(INTDIR)\PlayerOtherMgr.obj" \
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXBundleGame.obj" \
	"$(INTDIR)\N3FXMgr.obj" \
	"$(INTDIR)\N3FXPartBase.obj" \
	"$(INTDIR)\N3FXPartBillBoard.obj" \
	"$(INTDIR)\N3FXPartBillBoardGame.obj" \
	"$(INTDIR)\N3FXPartBottomBoard.obj" \
	"$(INTDIR)\N3FXPartBottomBoardGame.obj" \
	"$(INTDIR)\N3FXParticle.obj" \
	"$(INTDIR)\N3FXPartMesh.obj" \
	"$(INTDIR)\N3FXPartParticles.obj" \
	"$(INTDIR)\N3FXPMesh.obj" \
	"$(INTDIR)\N3FXPMeshInstance.obj" \
	"$(INTDIR)\N3FXShape.obj" \
	"$(INTDIR)\CountableItemEditDlg.obj" \
	"$(INTDIR)\UIDroppedItemDlg.obj" \
	"$(INTDIR)\UIImageTooltipDlg.obj" \
	"$(INTDIR)\UIInventory.obj" \
	"$(INTDIR)\UITransactionDlg.obj" \
	"$(INTDIR)\UIWareHouseDlg.obj" \
	"$(INTDIR)\N3UIArea.obj" \
	"$(INTDIR)\N3UIIcon.obj" \
	"$(INTDIR)\N3UIWndBase.obj" \
	"$(INTDIR)\UINPCEvent.obj" \
	"$(INTDIR)\UINpcTalk.obj" \
	"$(INTDIR)\UIHotKeyDlg.obj" \
	"$(INTDIR)\UISkillTreeDlg.obj" \
	"$(INTDIR)\ItemRepairMgr.obj" \
	"$(INTDIR)\UIItemExchange.obj" \
	"$(INTDIR)\UIRepairTooltipDlg.obj" \
	"$(INTDIR)\UIClassChange.obj" \
	"$(INTDIR)\UINPCChangeEvent.obj" \
	"$(INTDIR)\UIPointInitDlg.obj" \
	"$(INTDIR)\UICreateClanName.obj" \
	"$(INTDIR)\UIInn.obj" \
	"$(INTDIR)\N3UIDBCLButton.obj" \
	"$(INTDIR)\UIChat.obj" \
	"$(INTDIR)\UICmd.obj" \
	"$(INTDIR)\UIDead.obj" \
	"$(INTDIR)\UIEndingDisplay.obj" \
	"$(INTDIR)\UIHelp.obj" \
	"$(INTDIR)\UIKnightsOperation.obj" \
	"$(INTDIR)\UILoading.obj" \
	"$(INTDIR)\UIManager.obj" \
	"$(INTDIR)\UIMessageBox.obj" \
	"$(INTDIR)\UIMessageBoxManager.obj" \
	"$(INTDIR)\UIMessageWnd.obj" \
	"$(INTDIR)\UINotice.obj" \
	"$(INTDIR)\UIPartyBBS.obj" \
	"$(INTDIR)\UIPartyBBSSelector.obj" \
	"$(INTDIR)\UIPartyOrForce.obj" \
	"$(INTDIR)\UIQuestMenu.obj" \
	"$(INTDIR)\UIQuestTalk.obj" \
	"$(INTDIR)\UIStateBar.obj" \
	"$(INTDIR)\UITargetBar.obj" \
	"$(INTDIR)\UITradeBBSEditDlg.obj" \
	"$(INTDIR)\UITradeBBSSelector.obj" \
	"$(INTDIR)\UITradeExplanation.obj" \
	"$(INTDIR)\UITradeSellBBS.obj" \
	"$(INTDIR)\UIVarious.obj" \
	"$(INTDIR)\UIWarp.obj" \
	"$(INTDIR)\SubProcPerTrade.obj" \
	"$(INTDIR)\UIPerTradeDlg.obj" \
	"$(INTDIR)\UITradeEditDlg.obj" \
	"$(INTDIR)\MagicSkillMng.obj" \
	"$(INTDIR)\N3Cloud.obj" \
	"$(INTDIR)\N3ColorChange.obj" \
	"$(INTDIR)\N3GERain.obj" \
	"$(INTDIR)\N3GESnow.obj" \
	"$(INTDIR)\N3GlobalEffect.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3Sky.obj" \
	"$(INTDIR)\N3SkyMng.obj" \
	"$(INTDIR)\N3Star.obj" \
	"$(INTDIR)\N3Sun.obj" \
	"$(INTDIR)\N3Terrain.obj" \
	"$(INTDIR)\N3TerrainPatch.obj" \
	"$(INTDIR)\N3ShapeMgr.obj" \
	"$(INTDIR)\Bird.obj" \
	"$(INTDIR)\BirdMng.obj" \
	"$(INTDIR)\GrassBoard.obj" \
	"$(INTDIR)\LightMgr.obj" \
	"$(INTDIR)\N3Pond.obj" \
	"$(INTDIR)\N3River.obj" \
	"$(INTDIR)\ServerMesh.obj" \
	"$(INTDIR)\N3TerrainManager.obj" \
	"$(INTDIR)\PortalVolume.obj" \
	"$(INTDIR)\PvsMgr.obj" \
	"$(INTDIR)\DungeonManager.obj" \
	"$(INTDIR)\N3WorldBase.obj" \
	"$(INTDIR)\N3WorldManager.obj" \
	"$(INTDIR)\GameProcMain.obj" \
	"$(INTDIR)\GameProcOption.obj" \
	"$(INTDIR)\GameProcNationSelect.obj" \
	"$(INTDIR)\UINationSelectDlg.obj" \
	"$(INTDIR)\KnightChrMgr.obj" \
	"$(INTDIR)\SharedMem.obj" \
	"$(INTDIR)\GameProcLogIn.obj" \
	"$(INTDIR)\UILogin.obj" \
	"$(INTDIR)\EventManager.obj" \
	"$(INTDIR)\GameBase.obj" \
	"$(INTDIR)\GameCursor.obj" \
	"$(INTDIR)\GameEng.obj" \
	"$(INTDIR)\GameProcedure.obj" \
	"$(INTDIR)\JpegFile.obj" \
	"$(INTDIR)\LocalInput.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WarFareMain.obj" \
	"$(INTDIR)\WarMessage.obj" \
	"$(INTDIR)\Resource.res" \
	".\JvCryption.lib" \
	".\Implode.lib"

".\KnightOnLine.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WarFare - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\KnightOnLine_Debug.exe" "$(OUTDIR)\WarFare.bsc"


CLEAN :
	-@erase "$(INTDIR)\APISocket.obj"
	-@erase "$(INTDIR)\APISocket.sbr"
	-@erase "$(INTDIR)\Bird.obj"
	-@erase "$(INTDIR)\Bird.sbr"
	-@erase "$(INTDIR)\BirdMng.obj"
	-@erase "$(INTDIR)\BirdMng.sbr"
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\Catapult.obj"
	-@erase "$(INTDIR)\Catapult.sbr"
	-@erase "$(INTDIR)\Compress.obj"
	-@erase "$(INTDIR)\Compress.sbr"
	-@erase "$(INTDIR)\CountableItemEditDlg.obj"
	-@erase "$(INTDIR)\CountableItemEditDlg.sbr"
	-@erase "$(INTDIR)\DFont.obj"
	-@erase "$(INTDIR)\DFont.sbr"
	-@erase "$(INTDIR)\DungeonManager.obj"
	-@erase "$(INTDIR)\DungeonManager.sbr"
	-@erase "$(INTDIR)\EventManager.obj"
	-@erase "$(INTDIR)\EventManager.sbr"
	-@erase "$(INTDIR)\GameBase.obj"
	-@erase "$(INTDIR)\GameBase.sbr"
	-@erase "$(INTDIR)\GameCursor.obj"
	-@erase "$(INTDIR)\GameCursor.sbr"
	-@erase "$(INTDIR)\GameEng.obj"
	-@erase "$(INTDIR)\GameEng.sbr"
	-@erase "$(INTDIR)\GameProcCharacterCreate.obj"
	-@erase "$(INTDIR)\GameProcCharacterCreate.sbr"
	-@erase "$(INTDIR)\GameProcCharacterSelect.obj"
	-@erase "$(INTDIR)\GameProcCharacterSelect.sbr"
	-@erase "$(INTDIR)\GameProcedure.obj"
	-@erase "$(INTDIR)\GameProcedure.sbr"
	-@erase "$(INTDIR)\GameProcLogIn.obj"
	-@erase "$(INTDIR)\GameProcLogIn.sbr"
	-@erase "$(INTDIR)\GameProcMain.obj"
	-@erase "$(INTDIR)\GameProcMain.sbr"
	-@erase "$(INTDIR)\GameProcNationSelect.obj"
	-@erase "$(INTDIR)\GameProcNationSelect.sbr"
	-@erase "$(INTDIR)\GameProcOption.obj"
	-@erase "$(INTDIR)\GameProcOption.sbr"
	-@erase "$(INTDIR)\GrassBoard.obj"
	-@erase "$(INTDIR)\GrassBoard.sbr"
	-@erase "$(INTDIR)\ItemRepairMgr.obj"
	-@erase "$(INTDIR)\ItemRepairMgr.sbr"
	-@erase "$(INTDIR)\JPEG.OBJ"
	-@erase "$(INTDIR)\JPEG.SBR"
	-@erase "$(INTDIR)\JpegFile.obj"
	-@erase "$(INTDIR)\JpegFile.sbr"
	-@erase "$(INTDIR)\KnightChrMgr.obj"
	-@erase "$(INTDIR)\KnightChrMgr.sbr"
	-@erase "$(INTDIR)\LightMgr.obj"
	-@erase "$(INTDIR)\LightMgr.sbr"
	-@erase "$(INTDIR)\LocalInput.obj"
	-@erase "$(INTDIR)\LocalInput.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
	-@erase "$(INTDIR)\MachineBase.obj"
	-@erase "$(INTDIR)\MachineBase.sbr"
	-@erase "$(INTDIR)\MachineMng.obj"
	-@erase "$(INTDIR)\MachineMng.sbr"
	-@erase "$(INTDIR)\MagicSkillMng.obj"
	-@erase "$(INTDIR)\MagicSkillMng.sbr"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.sbr"
	-@erase "$(INTDIR)\N3AnimatedTexures.obj"
	-@erase "$(INTDIR)\N3AnimatedTexures.sbr"
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
	-@erase "$(INTDIR)\N3FXBundle.obj"
	-@erase "$(INTDIR)\N3FXBundle.sbr"
	-@erase "$(INTDIR)\N3FXBundleGame.obj"
	-@erase "$(INTDIR)\N3FXBundleGame.sbr"
	-@erase "$(INTDIR)\N3FXMgr.obj"
	-@erase "$(INTDIR)\N3FXMgr.sbr"
	-@erase "$(INTDIR)\N3FXPartBase.obj"
	-@erase "$(INTDIR)\N3FXPartBase.sbr"
	-@erase "$(INTDIR)\N3FXPartBillBoard.obj"
	-@erase "$(INTDIR)\N3FXPartBillBoard.sbr"
	-@erase "$(INTDIR)\N3FXPartBillBoardGame.obj"
	-@erase "$(INTDIR)\N3FXPartBillBoardGame.sbr"
	-@erase "$(INTDIR)\N3FXPartBottomBoard.obj"
	-@erase "$(INTDIR)\N3FXPartBottomBoard.sbr"
	-@erase "$(INTDIR)\N3FXPartBottomBoardGame.obj"
	-@erase "$(INTDIR)\N3FXPartBottomBoardGame.sbr"
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
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.sbr"
	-@erase "$(INTDIR)\N3Pond.obj"
	-@erase "$(INTDIR)\N3Pond.sbr"
	-@erase "$(INTDIR)\N3River.obj"
	-@erase "$(INTDIR)\N3River.sbr"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Shape.sbr"
	-@erase "$(INTDIR)\N3ShapeExtra.obj"
	-@erase "$(INTDIR)\N3ShapeExtra.sbr"
	-@erase "$(INTDIR)\N3ShapeMgr.obj"
	-@erase "$(INTDIR)\N3ShapeMgr.sbr"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Skin.sbr"
	-@erase "$(INTDIR)\N3Sky.obj"
	-@erase "$(INTDIR)\N3Sky.sbr"
	-@erase "$(INTDIR)\N3SkyMng.obj"
	-@erase "$(INTDIR)\N3SkyMng.sbr"
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
	-@erase "$(INTDIR)\N3Terrain.obj"
	-@erase "$(INTDIR)\N3Terrain.sbr"
	-@erase "$(INTDIR)\N3TerrainManager.obj"
	-@erase "$(INTDIR)\N3TerrainManager.sbr"
	-@erase "$(INTDIR)\N3TerrainPatch.obj"
	-@erase "$(INTDIR)\N3TerrainPatch.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3TransformCollision.sbr"
	-@erase "$(INTDIR)\N3UIArea.obj"
	-@erase "$(INTDIR)\N3UIArea.sbr"
	-@erase "$(INTDIR)\N3UIBase.obj"
	-@erase "$(INTDIR)\N3UIBase.sbr"
	-@erase "$(INTDIR)\N3UIButton.obj"
	-@erase "$(INTDIR)\N3UIButton.sbr"
	-@erase "$(INTDIR)\N3UIDBCLButton.obj"
	-@erase "$(INTDIR)\N3UIDBCLButton.sbr"
	-@erase "$(INTDIR)\N3UIEdit.obj"
	-@erase "$(INTDIR)\N3UIEdit.sbr"
	-@erase "$(INTDIR)\N3UIIcon.obj"
	-@erase "$(INTDIR)\N3UIIcon.sbr"
	-@erase "$(INTDIR)\N3UIImage.obj"
	-@erase "$(INTDIR)\N3UIImage.sbr"
	-@erase "$(INTDIR)\N3UIList.obj"
	-@erase "$(INTDIR)\N3UIList.sbr"
	-@erase "$(INTDIR)\N3UIProgress.obj"
	-@erase "$(INTDIR)\N3UIProgress.sbr"
	-@erase "$(INTDIR)\N3UIScrollBar.obj"
	-@erase "$(INTDIR)\N3UIScrollBar.sbr"
	-@erase "$(INTDIR)\N3UIStatic.obj"
	-@erase "$(INTDIR)\N3UIStatic.sbr"
	-@erase "$(INTDIR)\N3UIString.obj"
	-@erase "$(INTDIR)\N3UIString.sbr"
	-@erase "$(INTDIR)\N3UITooltip.obj"
	-@erase "$(INTDIR)\N3UITooltip.sbr"
	-@erase "$(INTDIR)\N3UITrackBar.obj"
	-@erase "$(INTDIR)\N3UITrackBar.sbr"
	-@erase "$(INTDIR)\N3UIWndBase.obj"
	-@erase "$(INTDIR)\N3UIWndBase.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\N3WorldBase.obj"
	-@erase "$(INTDIR)\N3WorldBase.sbr"
	-@erase "$(INTDIR)\N3WorldManager.obj"
	-@erase "$(INTDIR)\N3WorldManager.sbr"
	-@erase "$(INTDIR)\PlayerBase.obj"
	-@erase "$(INTDIR)\PlayerBase.sbr"
	-@erase "$(INTDIR)\PlayerMySelf.obj"
	-@erase "$(INTDIR)\PlayerMySelf.sbr"
	-@erase "$(INTDIR)\PlayerNPC.obj"
	-@erase "$(INTDIR)\PlayerNPC.sbr"
	-@erase "$(INTDIR)\PlayerOther.obj"
	-@erase "$(INTDIR)\PlayerOther.sbr"
	-@erase "$(INTDIR)\PlayerOtherMgr.obj"
	-@erase "$(INTDIR)\PlayerOtherMgr.sbr"
	-@erase "$(INTDIR)\PortalVolume.obj"
	-@erase "$(INTDIR)\PortalVolume.sbr"
	-@erase "$(INTDIR)\PvsMgr.obj"
	-@erase "$(INTDIR)\PvsMgr.sbr"
	-@erase "$(INTDIR)\Resource.res"
	-@erase "$(INTDIR)\ServerMesh.obj"
	-@erase "$(INTDIR)\ServerMesh.sbr"
	-@erase "$(INTDIR)\SharedMem.obj"
	-@erase "$(INTDIR)\SharedMem.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SubProcPerTrade.obj"
	-@erase "$(INTDIR)\SubProcPerTrade.sbr"
	-@erase "$(INTDIR)\UICharacterCreate.obj"
	-@erase "$(INTDIR)\UICharacterCreate.sbr"
	-@erase "$(INTDIR)\UICharacterSelect.obj"
	-@erase "$(INTDIR)\UICharacterSelect.sbr"
	-@erase "$(INTDIR)\UIChat.obj"
	-@erase "$(INTDIR)\UIChat.sbr"
	-@erase "$(INTDIR)\UIClassChange.obj"
	-@erase "$(INTDIR)\UIClassChange.sbr"
	-@erase "$(INTDIR)\UICmd.obj"
	-@erase "$(INTDIR)\UICmd.sbr"
	-@erase "$(INTDIR)\UICreateClanName.obj"
	-@erase "$(INTDIR)\UICreateClanName.sbr"
	-@erase "$(INTDIR)\UIDead.obj"
	-@erase "$(INTDIR)\UIDead.sbr"
	-@erase "$(INTDIR)\UIDroppedItemDlg.obj"
	-@erase "$(INTDIR)\UIDroppedItemDlg.sbr"
	-@erase "$(INTDIR)\UIEndingDisplay.obj"
	-@erase "$(INTDIR)\UIEndingDisplay.sbr"
	-@erase "$(INTDIR)\UIHelp.obj"
	-@erase "$(INTDIR)\UIHelp.sbr"
	-@erase "$(INTDIR)\UIHotKeyDlg.obj"
	-@erase "$(INTDIR)\UIHotKeyDlg.sbr"
	-@erase "$(INTDIR)\UIImageTooltipDlg.obj"
	-@erase "$(INTDIR)\UIImageTooltipDlg.sbr"
	-@erase "$(INTDIR)\UIInn.obj"
	-@erase "$(INTDIR)\UIInn.sbr"
	-@erase "$(INTDIR)\UIInventory.obj"
	-@erase "$(INTDIR)\UIInventory.sbr"
	-@erase "$(INTDIR)\UIItemExchange.obj"
	-@erase "$(INTDIR)\UIItemExchange.sbr"
	-@erase "$(INTDIR)\UIKnightsOperation.obj"
	-@erase "$(INTDIR)\UIKnightsOperation.sbr"
	-@erase "$(INTDIR)\UILoading.obj"
	-@erase "$(INTDIR)\UILoading.sbr"
	-@erase "$(INTDIR)\UILogin.obj"
	-@erase "$(INTDIR)\UILogin.sbr"
	-@erase "$(INTDIR)\UIManager.obj"
	-@erase "$(INTDIR)\UIManager.sbr"
	-@erase "$(INTDIR)\UIMessageBox.obj"
	-@erase "$(INTDIR)\UIMessageBox.sbr"
	-@erase "$(INTDIR)\UIMessageBoxManager.obj"
	-@erase "$(INTDIR)\UIMessageBoxManager.sbr"
	-@erase "$(INTDIR)\UIMessageWnd.obj"
	-@erase "$(INTDIR)\UIMessageWnd.sbr"
	-@erase "$(INTDIR)\UINationSelectDlg.obj"
	-@erase "$(INTDIR)\UINationSelectDlg.sbr"
	-@erase "$(INTDIR)\UINotice.obj"
	-@erase "$(INTDIR)\UINotice.sbr"
	-@erase "$(INTDIR)\UINPCChangeEvent.obj"
	-@erase "$(INTDIR)\UINPCChangeEvent.sbr"
	-@erase "$(INTDIR)\UINPCEvent.obj"
	-@erase "$(INTDIR)\UINPCEvent.sbr"
	-@erase "$(INTDIR)\UINpcTalk.obj"
	-@erase "$(INTDIR)\UINpcTalk.sbr"
	-@erase "$(INTDIR)\UIPartyBBS.obj"
	-@erase "$(INTDIR)\UIPartyBBS.sbr"
	-@erase "$(INTDIR)\UIPartyBBSSelector.obj"
	-@erase "$(INTDIR)\UIPartyBBSSelector.sbr"
	-@erase "$(INTDIR)\UIPartyOrForce.obj"
	-@erase "$(INTDIR)\UIPartyOrForce.sbr"
	-@erase "$(INTDIR)\UIPerTradeDlg.obj"
	-@erase "$(INTDIR)\UIPerTradeDlg.sbr"
	-@erase "$(INTDIR)\UIPointInitDlg.obj"
	-@erase "$(INTDIR)\UIPointInitDlg.sbr"
	-@erase "$(INTDIR)\UIQuestMenu.obj"
	-@erase "$(INTDIR)\UIQuestMenu.sbr"
	-@erase "$(INTDIR)\UIQuestTalk.obj"
	-@erase "$(INTDIR)\UIQuestTalk.sbr"
	-@erase "$(INTDIR)\UIRepairTooltipDlg.obj"
	-@erase "$(INTDIR)\UIRepairTooltipDlg.sbr"
	-@erase "$(INTDIR)\UISkillTreeDlg.obj"
	-@erase "$(INTDIR)\UISkillTreeDlg.sbr"
	-@erase "$(INTDIR)\UIStateBar.obj"
	-@erase "$(INTDIR)\UIStateBar.sbr"
	-@erase "$(INTDIR)\UITargetBar.obj"
	-@erase "$(INTDIR)\UITargetBar.sbr"
	-@erase "$(INTDIR)\UITradeBBSEditDlg.obj"
	-@erase "$(INTDIR)\UITradeBBSEditDlg.sbr"
	-@erase "$(INTDIR)\UITradeBBSSelector.obj"
	-@erase "$(INTDIR)\UITradeBBSSelector.sbr"
	-@erase "$(INTDIR)\UITradeEditDlg.obj"
	-@erase "$(INTDIR)\UITradeEditDlg.sbr"
	-@erase "$(INTDIR)\UITradeExplanation.obj"
	-@erase "$(INTDIR)\UITradeExplanation.sbr"
	-@erase "$(INTDIR)\UITradeSellBBS.obj"
	-@erase "$(INTDIR)\UITradeSellBBS.sbr"
	-@erase "$(INTDIR)\UITransactionDlg.obj"
	-@erase "$(INTDIR)\UITransactionDlg.sbr"
	-@erase "$(INTDIR)\UIVarious.obj"
	-@erase "$(INTDIR)\UIVarious.sbr"
	-@erase "$(INTDIR)\UIWareHouseDlg.obj"
	-@erase "$(INTDIR)\UIWareHouseDlg.sbr"
	-@erase "$(INTDIR)\UIWarp.obj"
	-@erase "$(INTDIR)\UIWarp.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WarFareMain.obj"
	-@erase "$(INTDIR)\WarFareMain.sbr"
	-@erase "$(INTDIR)\WarMessage.obj"
	-@erase "$(INTDIR)\WarMessage.sbr"
	-@erase "$(INTDIR)\WaveFile.obj"
	-@erase "$(INTDIR)\WaveFile.sbr"
	-@erase "$(OUTDIR)\KnightOnLine_Debug.map"
	-@erase "$(OUTDIR)\WarFare.bsc"
	-@erase ".\KnightOnLine_Debug.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3GAME" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\WarFare.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /D "_N3GAME" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WarFare.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\N3UIBase.sbr" \
	"$(INTDIR)\N3UIButton.sbr" \
	"$(INTDIR)\N3UIEdit.sbr" \
	"$(INTDIR)\N3UIImage.sbr" \
	"$(INTDIR)\N3UIList.sbr" \
	"$(INTDIR)\N3UIProgress.sbr" \
	"$(INTDIR)\N3UIScrollBar.sbr" \
	"$(INTDIR)\N3UIStatic.sbr" \
	"$(INTDIR)\N3UIString.sbr" \
	"$(INTDIR)\N3UITooltip.sbr" \
	"$(INTDIR)\N3UITrackBar.sbr" \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\DFont.sbr" \
	"$(INTDIR)\JPEG.SBR" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimatedTexures.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Board.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Cloak.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3FXPlug.sbr" \
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Joint.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Mesh.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3ShapeExtra.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr" \
	"$(INTDIR)\N3SndMgr.sbr" \
	"$(INTDIR)\N3SndObj.sbr" \
	"$(INTDIR)\N3SndObjStream.sbr" \
	"$(INTDIR)\WaveFile.sbr" \
	"$(INTDIR)\APISocket.sbr" \
	"$(INTDIR)\Compress.sbr" \
	"$(INTDIR)\GameProcCharacterCreate.sbr" \
	"$(INTDIR)\UICharacterCreate.sbr" \
	"$(INTDIR)\GameProcCharacterSelect.sbr" \
	"$(INTDIR)\UICharacterSelect.sbr" \
	"$(INTDIR)\N3Chr.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\PlayerBase.sbr" \
	"$(INTDIR)\PlayerNPC.sbr" \
	"$(INTDIR)\Catapult.sbr" \
	"$(INTDIR)\MachineBase.sbr" \
	"$(INTDIR)\MachineMng.sbr" \
	"$(INTDIR)\PlayerMySelf.sbr" \
	"$(INTDIR)\PlayerOther.sbr" \
	"$(INTDIR)\PlayerOtherMgr.sbr" \
	"$(INTDIR)\N3FXBundle.sbr" \
	"$(INTDIR)\N3FXBundleGame.sbr" \
	"$(INTDIR)\N3FXMgr.sbr" \
	"$(INTDIR)\N3FXPartBase.sbr" \
	"$(INTDIR)\N3FXPartBillBoard.sbr" \
	"$(INTDIR)\N3FXPartBillBoardGame.sbr" \
	"$(INTDIR)\N3FXPartBottomBoard.sbr" \
	"$(INTDIR)\N3FXPartBottomBoardGame.sbr" \
	"$(INTDIR)\N3FXParticle.sbr" \
	"$(INTDIR)\N3FXPartMesh.sbr" \
	"$(INTDIR)\N3FXPartParticles.sbr" \
	"$(INTDIR)\N3FXPMesh.sbr" \
	"$(INTDIR)\N3FXPMeshInstance.sbr" \
	"$(INTDIR)\N3FXShape.sbr" \
	"$(INTDIR)\CountableItemEditDlg.sbr" \
	"$(INTDIR)\UIDroppedItemDlg.sbr" \
	"$(INTDIR)\UIImageTooltipDlg.sbr" \
	"$(INTDIR)\UIInventory.sbr" \
	"$(INTDIR)\UITransactionDlg.sbr" \
	"$(INTDIR)\UIWareHouseDlg.sbr" \
	"$(INTDIR)\N3UIArea.sbr" \
	"$(INTDIR)\N3UIIcon.sbr" \
	"$(INTDIR)\N3UIWndBase.sbr" \
	"$(INTDIR)\UINPCEvent.sbr" \
	"$(INTDIR)\UINpcTalk.sbr" \
	"$(INTDIR)\UIHotKeyDlg.sbr" \
	"$(INTDIR)\UISkillTreeDlg.sbr" \
	"$(INTDIR)\ItemRepairMgr.sbr" \
	"$(INTDIR)\UIItemExchange.sbr" \
	"$(INTDIR)\UIRepairTooltipDlg.sbr" \
	"$(INTDIR)\UIClassChange.sbr" \
	"$(INTDIR)\UINPCChangeEvent.sbr" \
	"$(INTDIR)\UIPointInitDlg.sbr" \
	"$(INTDIR)\UICreateClanName.sbr" \
	"$(INTDIR)\UIInn.sbr" \
	"$(INTDIR)\N3UIDBCLButton.sbr" \
	"$(INTDIR)\UIChat.sbr" \
	"$(INTDIR)\UICmd.sbr" \
	"$(INTDIR)\UIDead.sbr" \
	"$(INTDIR)\UIEndingDisplay.sbr" \
	"$(INTDIR)\UIHelp.sbr" \
	"$(INTDIR)\UIKnightsOperation.sbr" \
	"$(INTDIR)\UILoading.sbr" \
	"$(INTDIR)\UIManager.sbr" \
	"$(INTDIR)\UIMessageBox.sbr" \
	"$(INTDIR)\UIMessageBoxManager.sbr" \
	"$(INTDIR)\UIMessageWnd.sbr" \
	"$(INTDIR)\UINotice.sbr" \
	"$(INTDIR)\UIPartyBBS.sbr" \
	"$(INTDIR)\UIPartyBBSSelector.sbr" \
	"$(INTDIR)\UIPartyOrForce.sbr" \
	"$(INTDIR)\UIQuestMenu.sbr" \
	"$(INTDIR)\UIQuestTalk.sbr" \
	"$(INTDIR)\UIStateBar.sbr" \
	"$(INTDIR)\UITargetBar.sbr" \
	"$(INTDIR)\UITradeBBSEditDlg.sbr" \
	"$(INTDIR)\UITradeBBSSelector.sbr" \
	"$(INTDIR)\UITradeExplanation.sbr" \
	"$(INTDIR)\UITradeSellBBS.sbr" \
	"$(INTDIR)\UIVarious.sbr" \
	"$(INTDIR)\UIWarp.sbr" \
	"$(INTDIR)\SubProcPerTrade.sbr" \
	"$(INTDIR)\UIPerTradeDlg.sbr" \
	"$(INTDIR)\UITradeEditDlg.sbr" \
	"$(INTDIR)\MagicSkillMng.sbr" \
	"$(INTDIR)\N3Cloud.sbr" \
	"$(INTDIR)\N3ColorChange.sbr" \
	"$(INTDIR)\N3GERain.sbr" \
	"$(INTDIR)\N3GESnow.sbr" \
	"$(INTDIR)\N3GlobalEffect.sbr" \
	"$(INTDIR)\N3Moon.sbr" \
	"$(INTDIR)\N3Sky.sbr" \
	"$(INTDIR)\N3SkyMng.sbr" \
	"$(INTDIR)\N3Star.sbr" \
	"$(INTDIR)\N3Sun.sbr" \
	"$(INTDIR)\N3Terrain.sbr" \
	"$(INTDIR)\N3TerrainPatch.sbr" \
	"$(INTDIR)\N3ShapeMgr.sbr" \
	"$(INTDIR)\Bird.sbr" \
	"$(INTDIR)\BirdMng.sbr" \
	"$(INTDIR)\GrassBoard.sbr" \
	"$(INTDIR)\LightMgr.sbr" \
	"$(INTDIR)\N3Pond.sbr" \
	"$(INTDIR)\N3River.sbr" \
	"$(INTDIR)\ServerMesh.sbr" \
	"$(INTDIR)\N3TerrainManager.sbr" \
	"$(INTDIR)\PortalVolume.sbr" \
	"$(INTDIR)\PvsMgr.sbr" \
	"$(INTDIR)\DungeonManager.sbr" \
	"$(INTDIR)\N3WorldBase.sbr" \
	"$(INTDIR)\N3WorldManager.sbr" \
	"$(INTDIR)\GameProcMain.sbr" \
	"$(INTDIR)\GameProcOption.sbr" \
	"$(INTDIR)\GameProcNationSelect.sbr" \
	"$(INTDIR)\UINationSelectDlg.sbr" \
	"$(INTDIR)\KnightChrMgr.sbr" \
	"$(INTDIR)\SharedMem.sbr" \
	"$(INTDIR)\GameProcLogIn.sbr" \
	"$(INTDIR)\UILogin.sbr" \
	"$(INTDIR)\EventManager.sbr" \
	"$(INTDIR)\GameBase.sbr" \
	"$(INTDIR)\GameCursor.sbr" \
	"$(INTDIR)\GameEng.sbr" \
	"$(INTDIR)\GameProcedure.sbr" \
	"$(INTDIR)\JpegFile.sbr" \
	"$(INTDIR)\LocalInput.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\WarFareMain.sbr" \
	"$(INTDIR)\WarMessage.sbr"

"$(OUTDIR)\WarFare.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=implode.lib winmm.lib imm32.lib ddraw.lib d3d9.lib d3dx9.lib dsound.lib dxguid.lib  wsock32.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NAFXCWD.LIB LIBCMTD.LIB ..\engine\JPEG\jpegLib.lib /nologo /subsystem:windows /profile /map:"$(INTDIR)\KnightOnLine_Debug.map" /debug /machine:I386 /nodefaultlib:"msvcrt" /out:"KnightOnLine_Debug.exe" 
LINK32_OBJS= \
	"$(INTDIR)\N3UIBase.obj" \
	"$(INTDIR)\N3UIButton.obj" \
	"$(INTDIR)\N3UIEdit.obj" \
	"$(INTDIR)\N3UIImage.obj" \
	"$(INTDIR)\N3UIList.obj" \
	"$(INTDIR)\N3UIProgress.obj" \
	"$(INTDIR)\N3UIScrollBar.obj" \
	"$(INTDIR)\N3UIStatic.obj" \
	"$(INTDIR)\N3UIString.obj" \
	"$(INTDIR)\N3UITooltip.obj" \
	"$(INTDIR)\N3UITrackBar.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DFont.obj" \
	"$(INTDIR)\JPEG.OBJ" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimatedTexures.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Board.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Cloak.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3FXPlug.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3ShapeExtra.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\N3SndMgr.obj" \
	"$(INTDIR)\N3SndObj.obj" \
	"$(INTDIR)\N3SndObjStream.obj" \
	"$(INTDIR)\WaveFile.obj" \
	"$(INTDIR)\APISocket.obj" \
	"$(INTDIR)\Compress.obj" \
	"$(INTDIR)\GameProcCharacterCreate.obj" \
	"$(INTDIR)\UICharacterCreate.obj" \
	"$(INTDIR)\GameProcCharacterSelect.obj" \
	"$(INTDIR)\UICharacterSelect.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\PlayerBase.obj" \
	"$(INTDIR)\PlayerNPC.obj" \
	"$(INTDIR)\Catapult.obj" \
	"$(INTDIR)\MachineBase.obj" \
	"$(INTDIR)\MachineMng.obj" \
	"$(INTDIR)\PlayerMySelf.obj" \
	"$(INTDIR)\PlayerOther.obj" \
	"$(INTDIR)\PlayerOtherMgr.obj" \
	"$(INTDIR)\N3FXBundle.obj" \
	"$(INTDIR)\N3FXBundleGame.obj" \
	"$(INTDIR)\N3FXMgr.obj" \
	"$(INTDIR)\N3FXPartBase.obj" \
	"$(INTDIR)\N3FXPartBillBoard.obj" \
	"$(INTDIR)\N3FXPartBillBoardGame.obj" \
	"$(INTDIR)\N3FXPartBottomBoard.obj" \
	"$(INTDIR)\N3FXPartBottomBoardGame.obj" \
	"$(INTDIR)\N3FXParticle.obj" \
	"$(INTDIR)\N3FXPartMesh.obj" \
	"$(INTDIR)\N3FXPartParticles.obj" \
	"$(INTDIR)\N3FXPMesh.obj" \
	"$(INTDIR)\N3FXPMeshInstance.obj" \
	"$(INTDIR)\N3FXShape.obj" \
	"$(INTDIR)\CountableItemEditDlg.obj" \
	"$(INTDIR)\UIDroppedItemDlg.obj" \
	"$(INTDIR)\UIImageTooltipDlg.obj" \
	"$(INTDIR)\UIInventory.obj" \
	"$(INTDIR)\UITransactionDlg.obj" \
	"$(INTDIR)\UIWareHouseDlg.obj" \
	"$(INTDIR)\N3UIArea.obj" \
	"$(INTDIR)\N3UIIcon.obj" \
	"$(INTDIR)\N3UIWndBase.obj" \
	"$(INTDIR)\UINPCEvent.obj" \
	"$(INTDIR)\UINpcTalk.obj" \
	"$(INTDIR)\UIHotKeyDlg.obj" \
	"$(INTDIR)\UISkillTreeDlg.obj" \
	"$(INTDIR)\ItemRepairMgr.obj" \
	"$(INTDIR)\UIItemExchange.obj" \
	"$(INTDIR)\UIRepairTooltipDlg.obj" \
	"$(INTDIR)\UIClassChange.obj" \
	"$(INTDIR)\UINPCChangeEvent.obj" \
	"$(INTDIR)\UIPointInitDlg.obj" \
	"$(INTDIR)\UICreateClanName.obj" \
	"$(INTDIR)\UIInn.obj" \
	"$(INTDIR)\N3UIDBCLButton.obj" \
	"$(INTDIR)\UIChat.obj" \
	"$(INTDIR)\UICmd.obj" \
	"$(INTDIR)\UIDead.obj" \
	"$(INTDIR)\UIEndingDisplay.obj" \
	"$(INTDIR)\UIHelp.obj" \
	"$(INTDIR)\UIKnightsOperation.obj" \
	"$(INTDIR)\UILoading.obj" \
	"$(INTDIR)\UIManager.obj" \
	"$(INTDIR)\UIMessageBox.obj" \
	"$(INTDIR)\UIMessageBoxManager.obj" \
	"$(INTDIR)\UIMessageWnd.obj" \
	"$(INTDIR)\UINotice.obj" \
	"$(INTDIR)\UIPartyBBS.obj" \
	"$(INTDIR)\UIPartyBBSSelector.obj" \
	"$(INTDIR)\UIPartyOrForce.obj" \
	"$(INTDIR)\UIQuestMenu.obj" \
	"$(INTDIR)\UIQuestTalk.obj" \
	"$(INTDIR)\UIStateBar.obj" \
	"$(INTDIR)\UITargetBar.obj" \
	"$(INTDIR)\UITradeBBSEditDlg.obj" \
	"$(INTDIR)\UITradeBBSSelector.obj" \
	"$(INTDIR)\UITradeExplanation.obj" \
	"$(INTDIR)\UITradeSellBBS.obj" \
	"$(INTDIR)\UIVarious.obj" \
	"$(INTDIR)\UIWarp.obj" \
	"$(INTDIR)\SubProcPerTrade.obj" \
	"$(INTDIR)\UIPerTradeDlg.obj" \
	"$(INTDIR)\UITradeEditDlg.obj" \
	"$(INTDIR)\MagicSkillMng.obj" \
	"$(INTDIR)\N3Cloud.obj" \
	"$(INTDIR)\N3ColorChange.obj" \
	"$(INTDIR)\N3GERain.obj" \
	"$(INTDIR)\N3GESnow.obj" \
	"$(INTDIR)\N3GlobalEffect.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3Sky.obj" \
	"$(INTDIR)\N3SkyMng.obj" \
	"$(INTDIR)\N3Star.obj" \
	"$(INTDIR)\N3Sun.obj" \
	"$(INTDIR)\N3Terrain.obj" \
	"$(INTDIR)\N3TerrainPatch.obj" \
	"$(INTDIR)\N3ShapeMgr.obj" \
	"$(INTDIR)\Bird.obj" \
	"$(INTDIR)\BirdMng.obj" \
	"$(INTDIR)\GrassBoard.obj" \
	"$(INTDIR)\LightMgr.obj" \
	"$(INTDIR)\N3Pond.obj" \
	"$(INTDIR)\N3River.obj" \
	"$(INTDIR)\ServerMesh.obj" \
	"$(INTDIR)\N3TerrainManager.obj" \
	"$(INTDIR)\PortalVolume.obj" \
	"$(INTDIR)\PvsMgr.obj" \
	"$(INTDIR)\DungeonManager.obj" \
	"$(INTDIR)\N3WorldBase.obj" \
	"$(INTDIR)\N3WorldManager.obj" \
	"$(INTDIR)\GameProcMain.obj" \
	"$(INTDIR)\GameProcOption.obj" \
	"$(INTDIR)\GameProcNationSelect.obj" \
	"$(INTDIR)\UINationSelectDlg.obj" \
	"$(INTDIR)\KnightChrMgr.obj" \
	"$(INTDIR)\SharedMem.obj" \
	"$(INTDIR)\GameProcLogIn.obj" \
	"$(INTDIR)\UILogin.obj" \
	"$(INTDIR)\EventManager.obj" \
	"$(INTDIR)\GameBase.obj" \
	"$(INTDIR)\GameCursor.obj" \
	"$(INTDIR)\GameEng.obj" \
	"$(INTDIR)\GameProcedure.obj" \
	"$(INTDIR)\JpegFile.obj" \
	"$(INTDIR)\LocalInput.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WarFareMain.obj" \
	"$(INTDIR)\WarMessage.obj" \
	"$(INTDIR)\Resource.res" \
	".\JvCryption.lib" \
	".\Implode.lib"

".\KnightOnLine_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("WarFare.dep")
!INCLUDE "WarFare.dep"
!ELSE 
!MESSAGE Warning: cannot find "WarFare.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WarFare - Win32 Release" || "$(CFG)" == "WarFare - Win32 Debug"
SOURCE=.\Resource.rc

"$(INTDIR)\Resource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIBase.cpp

"$(INTDIR)\N3UIBase.obj"	"$(INTDIR)\N3UIBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIButton.cpp

"$(INTDIR)\N3UIButton.obj"	"$(INTDIR)\N3UIButton.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIEdit.cpp

"$(INTDIR)\N3UIEdit.obj"	"$(INTDIR)\N3UIEdit.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIImage.cpp

"$(INTDIR)\N3UIImage.obj"	"$(INTDIR)\N3UIImage.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIList.cpp

"$(INTDIR)\N3UIList.obj"	"$(INTDIR)\N3UIList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIProgress.cpp

"$(INTDIR)\N3UIProgress.obj"	"$(INTDIR)\N3UIProgress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIScrollBar.cpp

"$(INTDIR)\N3UIScrollBar.obj"	"$(INTDIR)\N3UIScrollBar.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIStatic.cpp

"$(INTDIR)\N3UIStatic.obj"	"$(INTDIR)\N3UIStatic.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UIString.cpp

"$(INTDIR)\N3UIString.obj"	"$(INTDIR)\N3UIString.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UITooltip.cpp

"$(INTDIR)\N3UITooltip.obj"	"$(INTDIR)\N3UITooltip.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3UITrackBar.cpp

"$(INTDIR)\N3UITrackBar.obj"	"$(INTDIR)\N3UITrackBar.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\BitMapFile.cpp

"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\DFont.cpp

"$(INTDIR)\DFont.obj"	"$(INTDIR)\DFont.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\JPEG.CPP

"$(INTDIR)\JPEG.OBJ"	"$(INTDIR)\JPEG.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\LogWriter.cpp

"$(INTDIR)\LogWriter.obj"	"$(INTDIR)\LogWriter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3AlphaPrimitiveManager.cpp

"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3AnimatedTexures.cpp

"$(INTDIR)\N3AnimatedTexures.obj"	"$(INTDIR)\N3AnimatedTexures.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3AnimControl.cpp

"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3AnimKey.cpp

"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Base.cpp

"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3BaseFileAccess.cpp

"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Board.cpp

"$(INTDIR)\N3Board.obj"	"$(INTDIR)\N3Board.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Camera.cpp

"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Cloak.cpp

"$(INTDIR)\N3Cloak.obj"	"$(INTDIR)\N3Cloak.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Eng.cpp

"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3FXPlug.cpp

"$(INTDIR)\N3FXPlug.obj"	"$(INTDIR)\N3FXPlug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3IMesh.cpp

"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Joint.cpp

"$(INTDIR)\N3Joint.obj"	"$(INTDIR)\N3Joint.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Light.cpp

"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Mesh.cpp

"$(INTDIR)\N3Mesh.obj"	"$(INTDIR)\N3Mesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3PMesh.cpp

"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3PMeshInstance.cpp

"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Shape.cpp

"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3ShapeExtra.cpp

"$(INTDIR)\N3ShapeExtra.obj"	"$(INTDIR)\N3ShapeExtra.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Texture.cpp

"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Transform.cpp

"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3TransformCollision.cpp

"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3VMesh.cpp

"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3SndMgr.cpp

"$(INTDIR)\N3SndMgr.obj"	"$(INTDIR)\N3SndMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3SndObj.cpp

"$(INTDIR)\N3SndObj.obj"	"$(INTDIR)\N3SndObj.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3SndObjStream.cpp

"$(INTDIR)\N3SndObjStream.obj"	"$(INTDIR)\N3SndObjStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\WaveFile.cpp

"$(INTDIR)\WaveFile.obj"	"$(INTDIR)\WaveFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\APISocket.cpp

"$(INTDIR)\APISocket.obj"	"$(INTDIR)\APISocket.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Compress.cpp

"$(INTDIR)\Compress.obj"	"$(INTDIR)\Compress.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameProcCharacterCreate.cpp

"$(INTDIR)\GameProcCharacterCreate.obj"	"$(INTDIR)\GameProcCharacterCreate.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UICharacterCreate.cpp

"$(INTDIR)\UICharacterCreate.obj"	"$(INTDIR)\UICharacterCreate.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameProcCharacterSelect.cpp

"$(INTDIR)\GameProcCharacterSelect.obj"	"$(INTDIR)\GameProcCharacterSelect.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UICharacterSelect.cpp

"$(INTDIR)\UICharacterSelect.obj"	"$(INTDIR)\UICharacterSelect.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3Chr.cpp

"$(INTDIR)\N3Chr.obj"	"$(INTDIR)\N3Chr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Skin.cpp

"$(INTDIR)\N3Skin.obj"	"$(INTDIR)\N3Skin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\PlayerBase.cpp

"$(INTDIR)\PlayerBase.obj"	"$(INTDIR)\PlayerBase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PlayerNPC.cpp

"$(INTDIR)\PlayerNPC.obj"	"$(INTDIR)\PlayerNPC.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Catapult.cpp

"$(INTDIR)\Catapult.obj"	"$(INTDIR)\Catapult.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MachineBase.cpp

"$(INTDIR)\MachineBase.obj"	"$(INTDIR)\MachineBase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MachineMng.cpp

"$(INTDIR)\MachineMng.obj"	"$(INTDIR)\MachineMng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PlayerMySelf.cpp

"$(INTDIR)\PlayerMySelf.obj"	"$(INTDIR)\PlayerMySelf.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PlayerOther.cpp

"$(INTDIR)\PlayerOther.obj"	"$(INTDIR)\PlayerOther.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PlayerOtherMgr.cpp

"$(INTDIR)\PlayerOtherMgr.obj"	"$(INTDIR)\PlayerOtherMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3FXBundle.cpp

"$(INTDIR)\N3FXBundle.obj"	"$(INTDIR)\N3FXBundle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\N3FXBundleGame.cpp

"$(INTDIR)\N3FXBundleGame.obj"	"$(INTDIR)\N3FXBundleGame.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3FXMgr.cpp

"$(INTDIR)\N3FXMgr.obj"	"$(INTDIR)\N3FXMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3FXPartBase.cpp

"$(INTDIR)\N3FXPartBase.obj"	"$(INTDIR)\N3FXPartBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3FXPartBillBoard.cpp

"$(INTDIR)\N3FXPartBillBoard.obj"	"$(INTDIR)\N3FXPartBillBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\N3FXPartBillBoardGame.cpp

"$(INTDIR)\N3FXPartBillBoardGame.obj"	"$(INTDIR)\N3FXPartBillBoardGame.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3FXPartBottomBoard.cpp

"$(INTDIR)\N3FXPartBottomBoard.obj"	"$(INTDIR)\N3FXPartBottomBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\N3FXPartBottomBoardGame.cpp

"$(INTDIR)\N3FXPartBottomBoardGame.obj"	"$(INTDIR)\N3FXPartBottomBoardGame.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3FXParticle.cpp

"$(INTDIR)\N3FXParticle.obj"	"$(INTDIR)\N3FXParticle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3FXPartMesh.cpp

"$(INTDIR)\N3FXPartMesh.obj"	"$(INTDIR)\N3FXPartMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3FXPartParticles.cpp

"$(INTDIR)\N3FXPartParticles.obj"	"$(INTDIR)\N3FXPartParticles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3FXPMesh.cpp

"$(INTDIR)\N3FXPMesh.obj"	"$(INTDIR)\N3FXPMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3FXPMeshInstance.cpp

"$(INTDIR)\N3FXPMeshInstance.obj"	"$(INTDIR)\N3FXPMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3FXShape.cpp

"$(INTDIR)\N3FXShape.obj"	"$(INTDIR)\N3FXShape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\CountableItemEditDlg.cpp

"$(INTDIR)\CountableItemEditDlg.obj"	"$(INTDIR)\CountableItemEditDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIDroppedItemDlg.cpp

"$(INTDIR)\UIDroppedItemDlg.obj"	"$(INTDIR)\UIDroppedItemDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIImageTooltipDlg.cpp

"$(INTDIR)\UIImageTooltipDlg.obj"	"$(INTDIR)\UIImageTooltipDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIInventory.cpp

"$(INTDIR)\UIInventory.obj"	"$(INTDIR)\UIInventory.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UITransactionDlg.cpp

"$(INTDIR)\UITransactionDlg.obj"	"$(INTDIR)\UITransactionDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIWareHouseDlg.cpp

"$(INTDIR)\UIWareHouseDlg.obj"	"$(INTDIR)\UIWareHouseDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3UIArea.cpp

"$(INTDIR)\N3UIArea.obj"	"$(INTDIR)\N3UIArea.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\N3UIIcon.cpp

"$(INTDIR)\N3UIIcon.obj"	"$(INTDIR)\N3UIIcon.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3UIWndBase.cpp

"$(INTDIR)\N3UIWndBase.obj"	"$(INTDIR)\N3UIWndBase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UINPCEvent.cpp

"$(INTDIR)\UINPCEvent.obj"	"$(INTDIR)\UINPCEvent.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UINpcTalk.cpp

"$(INTDIR)\UINpcTalk.obj"	"$(INTDIR)\UINpcTalk.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIHotKeyDlg.cpp

"$(INTDIR)\UIHotKeyDlg.obj"	"$(INTDIR)\UIHotKeyDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UISkillTreeDlg.cpp

"$(INTDIR)\UISkillTreeDlg.obj"	"$(INTDIR)\UISkillTreeDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ItemRepairMgr.cpp

"$(INTDIR)\ItemRepairMgr.obj"	"$(INTDIR)\ItemRepairMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIItemExchange.cpp

"$(INTDIR)\UIItemExchange.obj"	"$(INTDIR)\UIItemExchange.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIRepairTooltipDlg.cpp

"$(INTDIR)\UIRepairTooltipDlg.obj"	"$(INTDIR)\UIRepairTooltipDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIClassChange.cpp

"$(INTDIR)\UIClassChange.obj"	"$(INTDIR)\UIClassChange.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UINPCChangeEvent.cpp

"$(INTDIR)\UINPCChangeEvent.obj"	"$(INTDIR)\UINPCChangeEvent.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIPointInitDlg.cpp

"$(INTDIR)\UIPointInitDlg.obj"	"$(INTDIR)\UIPointInitDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UICreateClanName.cpp

"$(INTDIR)\UICreateClanName.obj"	"$(INTDIR)\UICreateClanName.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIInn.cpp

"$(INTDIR)\UIInn.obj"	"$(INTDIR)\UIInn.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3UIDBCLButton.cpp

"$(INTDIR)\N3UIDBCLButton.obj"	"$(INTDIR)\N3UIDBCLButton.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIChat.cpp

"$(INTDIR)\UIChat.obj"	"$(INTDIR)\UIChat.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UICmd.cpp

"$(INTDIR)\UICmd.obj"	"$(INTDIR)\UICmd.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIDead.cpp

"$(INTDIR)\UIDead.obj"	"$(INTDIR)\UIDead.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIEndingDisplay.cpp

"$(INTDIR)\UIEndingDisplay.obj"	"$(INTDIR)\UIEndingDisplay.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIHelp.cpp

"$(INTDIR)\UIHelp.obj"	"$(INTDIR)\UIHelp.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIKnightsOperation.cpp

"$(INTDIR)\UIKnightsOperation.obj"	"$(INTDIR)\UIKnightsOperation.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UILoading.cpp

"$(INTDIR)\UILoading.obj"	"$(INTDIR)\UILoading.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIManager.cpp

"$(INTDIR)\UIManager.obj"	"$(INTDIR)\UIManager.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIMessageBox.cpp

"$(INTDIR)\UIMessageBox.obj"	"$(INTDIR)\UIMessageBox.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIMessageBoxManager.cpp

"$(INTDIR)\UIMessageBoxManager.obj"	"$(INTDIR)\UIMessageBoxManager.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIMessageWnd.cpp

"$(INTDIR)\UIMessageWnd.obj"	"$(INTDIR)\UIMessageWnd.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UINotice.cpp

"$(INTDIR)\UINotice.obj"	"$(INTDIR)\UINotice.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIPartyBBS.cpp

"$(INTDIR)\UIPartyBBS.obj"	"$(INTDIR)\UIPartyBBS.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIPartyBBSSelector.cpp

"$(INTDIR)\UIPartyBBSSelector.obj"	"$(INTDIR)\UIPartyBBSSelector.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIPartyOrForce.cpp

"$(INTDIR)\UIPartyOrForce.obj"	"$(INTDIR)\UIPartyOrForce.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIQuestMenu.cpp

"$(INTDIR)\UIQuestMenu.obj"	"$(INTDIR)\UIQuestMenu.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIQuestTalk.cpp

"$(INTDIR)\UIQuestTalk.obj"	"$(INTDIR)\UIQuestTalk.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIStateBar.cpp

"$(INTDIR)\UIStateBar.obj"	"$(INTDIR)\UIStateBar.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UITargetBar.cpp

"$(INTDIR)\UITargetBar.obj"	"$(INTDIR)\UITargetBar.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UITradeBBSEditDlg.cpp

"$(INTDIR)\UITradeBBSEditDlg.obj"	"$(INTDIR)\UITradeBBSEditDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UITradeBBSSelector.cpp

"$(INTDIR)\UITradeBBSSelector.obj"	"$(INTDIR)\UITradeBBSSelector.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UITradeExplanation.cpp

"$(INTDIR)\UITradeExplanation.obj"	"$(INTDIR)\UITradeExplanation.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UITradeSellBBS.cpp

"$(INTDIR)\UITradeSellBBS.obj"	"$(INTDIR)\UITradeSellBBS.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIVarious.cpp

"$(INTDIR)\UIVarious.obj"	"$(INTDIR)\UIVarious.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIWarp.cpp

"$(INTDIR)\UIWarp.obj"	"$(INTDIR)\UIWarp.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SubProcPerTrade.cpp

"$(INTDIR)\SubProcPerTrade.obj"	"$(INTDIR)\SubProcPerTrade.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIPerTradeDlg.cpp

"$(INTDIR)\UIPerTradeDlg.obj"	"$(INTDIR)\UIPerTradeDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UITradeEditDlg.cpp

"$(INTDIR)\UITradeEditDlg.obj"	"$(INTDIR)\UITradeEditDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MagicSkillMng.cpp

"$(INTDIR)\MagicSkillMng.obj"	"$(INTDIR)\MagicSkillMng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3Cloud.cpp

"$(INTDIR)\N3Cloud.obj"	"$(INTDIR)\N3Cloud.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3ColorChange.cpp

"$(INTDIR)\N3ColorChange.obj"	"$(INTDIR)\N3ColorChange.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3GERain.cpp

"$(INTDIR)\N3GERain.obj"	"$(INTDIR)\N3GERain.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3GESnow.cpp

"$(INTDIR)\N3GESnow.obj"	"$(INTDIR)\N3GESnow.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3GlobalEffect.cpp

"$(INTDIR)\N3GlobalEffect.obj"	"$(INTDIR)\N3GlobalEffect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Moon.cpp

"$(INTDIR)\N3Moon.obj"	"$(INTDIR)\N3Moon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Sky.cpp

"$(INTDIR)\N3Sky.obj"	"$(INTDIR)\N3Sky.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3SkyMng.cpp

"$(INTDIR)\N3SkyMng.obj"	"$(INTDIR)\N3SkyMng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Star.cpp

"$(INTDIR)\N3Star.obj"	"$(INTDIR)\N3Star.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3Sun.cpp

"$(INTDIR)\N3Sun.obj"	"$(INTDIR)\N3Sun.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\N3Terrain.cpp

"$(INTDIR)\N3Terrain.obj"	"$(INTDIR)\N3Terrain.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3TerrainPatch.cpp

"$(INTDIR)\N3TerrainPatch.obj"	"$(INTDIR)\N3TerrainPatch.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3ShapeMgr.cpp

"$(INTDIR)\N3ShapeMgr.obj"	"$(INTDIR)\N3ShapeMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Bird.cpp

"$(INTDIR)\Bird.obj"	"$(INTDIR)\Bird.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BirdMng.cpp

"$(INTDIR)\BirdMng.obj"	"$(INTDIR)\BirdMng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GrassBoard.cpp

"$(INTDIR)\GrassBoard.obj"	"$(INTDIR)\GrassBoard.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LightMgr.cpp

"$(INTDIR)\LightMgr.obj"	"$(INTDIR)\LightMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\N3Base\N3Pond.cpp

"$(INTDIR)\N3Pond.obj"	"$(INTDIR)\N3Pond.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\engine\N3Base\N3River.cpp

"$(INTDIR)\N3River.obj"	"$(INTDIR)\N3River.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ServerMesh.cpp

"$(INTDIR)\ServerMesh.obj"	"$(INTDIR)\ServerMesh.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3TerrainManager.cpp

"$(INTDIR)\N3TerrainManager.obj"	"$(INTDIR)\N3TerrainManager.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PortalVolume.cpp

"$(INTDIR)\PortalVolume.obj"	"$(INTDIR)\PortalVolume.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PvsMgr.cpp

"$(INTDIR)\PvsMgr.obj"	"$(INTDIR)\PvsMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DungeonManager.cpp

"$(INTDIR)\DungeonManager.obj"	"$(INTDIR)\DungeonManager.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3WorldBase.cpp

"$(INTDIR)\N3WorldBase.obj"	"$(INTDIR)\N3WorldBase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\N3WorldManager.cpp

"$(INTDIR)\N3WorldManager.obj"	"$(INTDIR)\N3WorldManager.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameProcMain.cpp

"$(INTDIR)\GameProcMain.obj"	"$(INTDIR)\GameProcMain.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameProcOption.cpp

"$(INTDIR)\GameProcOption.obj"	"$(INTDIR)\GameProcOption.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameProcNationSelect.cpp

"$(INTDIR)\GameProcNationSelect.obj"	"$(INTDIR)\GameProcNationSelect.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UINationSelectDlg.cpp

"$(INTDIR)\UINationSelectDlg.obj"	"$(INTDIR)\UINationSelectDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\KnightChrMgr.cpp

"$(INTDIR)\KnightChrMgr.obj"	"$(INTDIR)\KnightChrMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SharedMem.cpp

"$(INTDIR)\SharedMem.obj"	"$(INTDIR)\SharedMem.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameProcLogIn.cpp

"$(INTDIR)\GameProcLogIn.obj"	"$(INTDIR)\GameProcLogIn.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UILogin.cpp

"$(INTDIR)\UILogin.obj"	"$(INTDIR)\UILogin.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EventManager.cpp

"$(INTDIR)\EventManager.obj"	"$(INTDIR)\EventManager.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameBase.cpp

"$(INTDIR)\GameBase.obj"	"$(INTDIR)\GameBase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameCursor.cpp

"$(INTDIR)\GameCursor.obj"	"$(INTDIR)\GameCursor.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameEng.cpp

"$(INTDIR)\GameEng.obj"	"$(INTDIR)\GameEng.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameProcedure.cpp

"$(INTDIR)\GameProcedure.obj"	"$(INTDIR)\GameProcedure.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\engine\JPEG\JpegFile.cpp

"$(INTDIR)\JpegFile.obj"	"$(INTDIR)\JpegFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\LocalInput.cpp

"$(INTDIR)\LocalInput.obj"	"$(INTDIR)\LocalInput.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WarFareMain.cpp

"$(INTDIR)\WarFareMain.obj"	"$(INTDIR)\WarFareMain.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WarMessage.cpp

"$(INTDIR)\WarMessage.obj"	"$(INTDIR)\WarMessage.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

