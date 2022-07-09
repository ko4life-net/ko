# Microsoft Developer Studio Project File - Name="WarFare" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WarFare - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WarFare.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/WarFare", DXBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WarFare - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3GAME" /Fr /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "_N3GAME" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 implode.lib kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib winmm.lib imm32.lib ddraw.lib d3d8.lib d3dx8.lib dsound.lib dxguid.lib dxerr8.lib wsock32.lib DInput8.lib ..\JPEG\jpegLib.lib /nologo /version:0.1 /subsystem:windows /pdb:none /map /machine:I386 /out:"KnightOnLine.exe"
# SUBTRACT LINK32 /debug /force

!ELSEIF  "$(CFG)" == "WarFare - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3GAME" /Fr /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /D "_N3GAME" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 implode.lib winmm.lib imm32.lib ddraw.lib d3d8.lib d3dx8.lib dsound.lib dxguid.lib dxerr8.lib wsock32.lib DInput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NAFXCWD.LIB LIBCMTD.LIB ..\JPEG\jpegLib.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"msvcrt" /out:"NetCatOnLine_Debug.exe"
# SUBTRACT LINK32 /force

!ENDIF 

# Begin Target

# Name "WarFare - Win32 Release"
# Name "WarFare - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Cursor_Attack.cur
# End Source File
# Begin Source File

SOURCE=.\Cursor_Click.cur
# End Source File
# Begin Source File

SOURCE=.\cursor_click1.cur
# End Source File
# Begin Source File

SOURCE=.\Cursor_Normal.cur
# End Source File
# Begin Source File

SOURCE=.\cursor_normal1.cur
# End Source File
# Begin Source File

SOURCE=.\repair0.cur
# End Source File
# Begin Source File

SOURCE=.\repair1.cur
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# Begin Source File

SOURCE=.\WarFare.ico
# End Source File
# Begin Source File

SOURCE=.\Warfare_tw.ico
# End Source File
# End Group
# Begin Group "N3Base"

# PROP Default_Filter ""
# Begin Group "N3UI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3Base\N3UIBase.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIButton.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIButton.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIDef.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIEdit.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIImage.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIImage.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIList.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIList.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIProgress.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIProgress.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIStatic.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIStatic.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIString.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UIString.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UITooltip.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UITooltip.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UITrackBar.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3UITrackBar.h
# End Source File
# End Group
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3Base\BitMapFile.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\BitMapFile.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\DFont.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\DFont.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\JPEG.CPP
# End Source File
# Begin Source File

SOURCE=..\N3Base\JPEG.H
# End Source File
# Begin Source File

SOURCE=..\N3BASE\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\LogWriter.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3AnimatedTexures.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3AnimatedTexures.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimControl.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimControl.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimKey.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimKey.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Board.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Board.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Camera.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Cloak.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Cloak.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPlug.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPlug.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3IMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3IMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Joint.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Joint.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Light.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Light.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeExtra.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeExtra.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3TableBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\StdAfxBase.h
# End Source File
# End Group
# End Group
# Begin Group "Reference Header"

# PROP Default_Filter ""
# End Group
# Begin Group "SoundMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3BASE\N3SndDef.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SndMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SndMgr.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SndObj.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SndObj.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SndObjStream.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SndObjStream.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\WaveFile.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\WaveFile.h
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\APISocket.cpp
# End Source File
# Begin Source File

SOURCE=.\APISocket.h
# End Source File
# Begin Source File

SOURCE=.\Compress.cpp
# End Source File
# Begin Source File

SOURCE=.\Compress.h
# End Source File
# Begin Source File

SOURCE=.\IMPLODE.H
# End Source File
# Begin Source File

SOURCE=.\JvCryption.h
# End Source File
# Begin Source File

SOURCE=.\PacketDef.h
# End Source File
# Begin Source File

SOURCE=.\JvCryption.lib
# End Source File
# Begin Source File

SOURCE=.\Implode.lib
# End Source File
# End Group
# Begin Group "Procedure - Character Create"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameProcCharacterCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\GameProcCharacterCreate.h
# End Source File
# Begin Source File

SOURCE=.\UICharacterCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\UICharacterCreate.h
# End Source File
# End Group
# Begin Group "Procedure - Character Select"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameProcCharacterSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\GameProcCharacterSelect.h
# End Source File
# Begin Source File

SOURCE=.\UICharacterSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\UICharacterSelect.h
# End Source File
# End Group
# Begin Group "Procedure - Main"

# PROP Default_Filter ""
# Begin Group "Player"

# PROP Default_Filter ""
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Bitset.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.h
# End Source File
# Begin Source File

SOURCE=.\PlayerBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerBase.h
# End Source File
# Begin Source File

SOURCE=.\PlayerNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerNPC.h
# End Source File
# End Group
# Begin Group "Machine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Catapult.cpp
# End Source File
# Begin Source File

SOURCE=.\Catapult.h
# End Source File
# Begin Source File

SOURCE=.\MachineBase.cpp
# End Source File
# Begin Source File

SOURCE=.\MachineBase.h
# End Source File
# Begin Source File

SOURCE=.\MachineMng.cpp
# End Source File
# Begin Source File

SOURCE=.\MachineMng.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\PlayerMySelf.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerMySelf.h
# End Source File
# Begin Source File

SOURCE=.\PlayerOther.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerOther.h
# End Source File
# Begin Source File

SOURCE=.\PlayerOtherMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerOtherMgr.h
# End Source File
# End Group
# Begin Group "FX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3Base\N3FXBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXBundle.h
# End Source File
# Begin Source File

SOURCE=.\N3FXBundleGame.cpp
# End Source File
# Begin Source File

SOURCE=.\N3FXBundleGame.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXDef.h
# End Source File
# Begin Source File

SOURCE=.\N3FXMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\N3FXMgr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBase.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBillBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBillBoard.h
# End Source File
# Begin Source File

SOURCE=.\N3FXPartBillBoardGame.cpp
# End Source File
# Begin Source File

SOURCE=.\N3FXPartBillBoardGame.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBottomBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBottomBoard.h
# End Source File
# Begin Source File

SOURCE=.\N3FXPartBottomBoardGame.cpp
# End Source File
# Begin Source File

SOURCE=.\N3FXPartBottomBoardGame.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXParticle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXParticle.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartParticles.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartParticles.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.h
# End Source File
# End Group
# Begin Group "UI"

# PROP Default_Filter ""
# Begin Group "UI(Item Relative)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CountableItemEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CountableItemEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\UIDroppedItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UIDroppedItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\UIImageTooltipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UIImageTooltipDlg.h
# End Source File
# Begin Source File

SOURCE=.\UIInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIInventory.h
# End Source File
# Begin Source File

SOURCE=.\UITransactionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UITransactionDlg.h
# End Source File
# Begin Source File

SOURCE=.\UIWareHouseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UIWareHouseDlg.h
# End Source File
# End Group
# Begin Group "UI(Icon Basic)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3BASE\N3UIArea.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIArea.h
# End Source File
# Begin Source File

SOURCE=.\N3UIIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\N3UIIcon.h
# End Source File
# Begin Source File

SOURCE=.\N3UIWndBase.cpp
# End Source File
# Begin Source File

SOURCE=.\N3UIWndBase.h
# End Source File
# Begin Source File

SOURCE=.\UINPCEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\UINPCEvent.h
# End Source File
# Begin Source File

SOURCE=.\UINpcTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\UINpcTalk.h
# End Source File
# End Group
# Begin Group "UI(Skill Relative)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UIHotKeyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UIHotKeyDlg.h
# End Source File
# Begin Source File

SOURCE=.\UISkillTreeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UISkillTreeDlg.h
# End Source File
# End Group
# Begin Group "UI(Exchange & Repair)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ItemRepairMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemRepairMgr.h
# End Source File
# Begin Source File

SOURCE=.\UIItemExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\UIItemExchange.h
# End Source File
# Begin Source File

SOURCE=.\UIRepairTooltipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UIRepairTooltipDlg.h
# End Source File
# End Group
# Begin Group "UI(Change & Init)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UIClassChange.cpp
# End Source File
# Begin Source File

SOURCE=.\UIClassChange.h
# End Source File
# Begin Source File

SOURCE=.\UINPCChangeEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\UINPCChangeEvent.h
# End Source File
# Begin Source File

SOURCE=.\UIPointInitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPointInitDlg.h
# End Source File
# End Group
# Begin Group "UI(Inn)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UICreateClanName.cpp
# End Source File
# Begin Source File

SOURCE=.\UICreateClanName.h
# End Source File
# Begin Source File

SOURCE=.\UIInn.cpp
# End Source File
# Begin Source File

SOURCE=.\UIInn.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\N3UIDBCLButton.cpp
# End Source File
# Begin Source File

SOURCE=.\N3UIDBCLButton.h
# End Source File
# Begin Source File

SOURCE=.\UIChat.cpp
# End Source File
# Begin Source File

SOURCE=.\UIChat.h
# End Source File
# Begin Source File

SOURCE=.\UICmd.cpp
# End Source File
# Begin Source File

SOURCE=.\UICmd.h
# End Source File
# Begin Source File

SOURCE=.\UIDead.cpp
# End Source File
# Begin Source File

SOURCE=.\UIDead.h
# End Source File
# Begin Source File

SOURCE=.\UIEndingDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\UIEndingDisplay.h
# End Source File
# Begin Source File

SOURCE=.\UIHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\UIHelp.h
# End Source File
# Begin Source File

SOURCE=.\UIKnightsOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\UIKnightsOperation.h
# End Source File
# Begin Source File

SOURCE=.\UILoading.cpp
# End Source File
# Begin Source File

SOURCE=.\UILoading.h
# End Source File
# Begin Source File

SOURCE=.\UIManager.cpp
# End Source File
# Begin Source File

SOURCE=.\UIManager.h
# End Source File
# Begin Source File

SOURCE=.\UIMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\UIMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\UIMessageBoxManager.cpp
# End Source File
# Begin Source File

SOURCE=.\UIMessageBoxManager.h
# End Source File
# Begin Source File

SOURCE=.\UIMessageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UIMessageWnd.h
# End Source File
# Begin Source File

SOURCE=.\UINotice.cpp
# End Source File
# Begin Source File

SOURCE=.\UINotice.h
# End Source File
# Begin Source File

SOURCE=.\UIPartyBBS.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPartyBBS.h
# End Source File
# Begin Source File

SOURCE=.\UIPartyBBSSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPartyBBSSelector.h
# End Source File
# Begin Source File

SOURCE=.\UIPartyOrForce.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPartyOrForce.h
# End Source File
# Begin Source File

SOURCE=.\UIQuestMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\UIQuestMenu.h
# End Source File
# Begin Source File

SOURCE=.\UIQuestTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\UIQuestTalk.h
# End Source File
# Begin Source File

SOURCE=.\UIStateBar.cpp
# End Source File
# Begin Source File

SOURCE=.\UIStateBar.h
# End Source File
# Begin Source File

SOURCE=.\UITargetBar.cpp
# End Source File
# Begin Source File

SOURCE=.\UITargetBar.h
# End Source File
# Begin Source File

SOURCE=.\UITradeBBSEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UITradeBBSEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\UITradeBBSSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\UITradeBBSSelector.h
# End Source File
# Begin Source File

SOURCE=.\UITradeExplanation.cpp
# End Source File
# Begin Source File

SOURCE=.\UITradeExplanation.h
# End Source File
# Begin Source File

SOURCE=.\UITradeSellBBS.cpp
# End Source File
# Begin Source File

SOURCE=.\UITradeSellBBS.h
# End Source File
# Begin Source File

SOURCE=.\UIVarious.cpp
# End Source File
# Begin Source File

SOURCE=.\UIVarious.h
# End Source File
# Begin Source File

SOURCE=.\UIWarp.cpp
# End Source File
# Begin Source File

SOURCE=.\UIWarp.h
# End Source File
# End Group
# Begin Group "Procedure[Sub] - PerTrade"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SubProcPerTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\SubProcPerTrade.h
# End Source File
# Begin Source File

SOURCE=.\UIPerTradeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UIPerTradeDlg.h
# End Source File
# Begin Source File

SOURCE=.\UITradeEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UITradeEditDlg.h
# End Source File
# End Group
# Begin Group "MagicSkill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MagicSkillMng.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSkillMng.h
# End Source File
# End Group
# Begin Group "World Manager"

# PROP Default_Filter ""
# Begin Group "Terrain Manager"

# PROP Default_Filter ""
# Begin Group "N3Sky"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3BASE\N3Cloud.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Cloud.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3ColorChange.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3ColorChange.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GERain.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GERain.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GESnow.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GESnow.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffect.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffect.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Moon.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Moon.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Sky.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Sky.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SkyMng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SkyMng.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Star.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Star.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Sun.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Sun.h
# End Source File
# End Group
# Begin Group "N3Terrain"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\N3Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Terrain.h
# End Source File
# Begin Source File

SOURCE=.\N3TerrainDef.h
# End Source File
# Begin Source File

SOURCE=.\N3TerrainPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\N3TerrainPatch.h
# End Source File
# End Group
# Begin Group "N3ShapeMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3Base\N3ShapeMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMgr.h
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Bird.cpp
# End Source File
# Begin Source File

SOURCE=.\Bird.h
# End Source File
# Begin Source File

SOURCE=.\BirdMng.cpp
# End Source File
# Begin Source File

SOURCE=.\BirdMng.h
# End Source File
# Begin Source File

SOURCE=.\GrassBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\GrassBoard.h
# End Source File
# Begin Source File

SOURCE=.\LightMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\LightMgr.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Pond.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Pond.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3River.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3River.h
# End Source File
# Begin Source File

SOURCE=.\ServerMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerMesh.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\N3TerrainManager.cpp
# End Source File
# Begin Source File

SOURCE=.\N3TerrainManager.h
# End Source File
# End Group
# Begin Group "Dungeon Manager"

# PROP Default_Filter ""
# Begin Group "PVS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PortalVolume.cpp
# End Source File
# Begin Source File

SOURCE=.\PortalVolume.h
# End Source File
# Begin Source File

SOURCE=.\PvsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PvsMgr.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DungeonManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DungeonManager.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\N3WorldBase.cpp
# End Source File
# Begin Source File

SOURCE=.\N3WorldBase.h
# End Source File
# Begin Source File

SOURCE=.\N3WorldManager.cpp
# End Source File
# Begin Source File

SOURCE=.\N3WorldManager.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GameProcMain.cpp
# End Source File
# Begin Source File

SOURCE=.\GameProcMain.h
# End Source File
# End Group
# Begin Group "Procedure - Option"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameProcOption.cpp
# End Source File
# Begin Source File

SOURCE=.\GameProcOption.h
# End Source File
# End Group
# Begin Group "Procedure - Nation Select"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameProcNationSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\GameProcNationSelect.h
# End Source File
# Begin Source File

SOURCE=.\UINationSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UINationSelectDlg.h
# End Source File
# End Group
# Begin Group "Knight Charactor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KnightChrMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\KnightChrMgr.h
# End Source File
# Begin Source File

SOURCE=.\SharedMem.cpp
# End Source File
# Begin Source File

SOURCE=.\SharedMem.h
# End Source File
# End Group
# Begin Group "Procedure - LogIn"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameProcLogIn.cpp
# End Source File
# Begin Source File

SOURCE=.\GameProcLogIn.h
# End Source File
# Begin Source File

SOURCE=.\UILogin.cpp
# End Source File
# Begin Source File

SOURCE=.\UILogin.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManager.h
# End Source File
# Begin Source File

SOURCE=.\GameBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GameBase.h
# End Source File
# Begin Source File

SOURCE=.\GameCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\GameCursor.h
# End Source File
# Begin Source File

SOURCE=.\GameDef.h
# End Source File
# Begin Source File

SOURCE=.\GameEng.cpp
# End Source File
# Begin Source File

SOURCE=.\GameEng.h
# End Source File
# Begin Source File

SOURCE=.\GameProcedure.cpp
# End Source File
# Begin Source File

SOURCE=.\GameProcedure.h
# End Source File
# Begin Source File

SOURCE=..\JPEG\JpegFile.cpp
# End Source File
# Begin Source File

SOURCE=..\JPEG\JpegFile.h
# End Source File
# Begin Source File

SOURCE=.\LocalInput.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalInput.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WarFareMain.cpp
# End Source File
# Begin Source File

SOURCE=.\WarFareMain.h
# End Source File
# Begin Source File

SOURCE=.\WarMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\WarMessage.h
# End Source File
# End Target
# End Project
