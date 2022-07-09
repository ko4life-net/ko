# Microsoft Developer Studio Project File - Name="N3Indoor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=N3Indoor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "N3Indoor.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/N3Indoor", GVEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "N3Indoor - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "N3Indoor - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3INDOOR" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib d3d8.lib d3dx8.lib dsound.lib dxguid.lib strmiids.lib dxerr8.lib /nologo /subsystem:windows /debug /machine:I386 /out:"N3Indoor_debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "N3Indoor - Win32 Release"
# Name "N3Indoor - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ARW09DN.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ARW09UP.ICO
# End Source File
# Begin Source File

SOURCE=.\res\HANDSHAK.ICO
# End Source File
# Begin Source File

SOURCE=.\res\HEART.ICO
# End Source File
# Begin Source File

SOURCE=.\res\MOVE4WAY.CUR
# End Source File
# Begin Source File

SOURCE=.\res\N3Indoor.ico
# End Source File
# Begin Source File

SOURCE=.\N3Indoor.rc
# End Source File
# Begin Source File

SOURCE=.\res\N3Indoor.rc2
# End Source File
# Begin Source File

SOURCE=.\N3Indoor.reg
# End Source File
# Begin Source File

SOURCE=.\res\N3IndoorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\POINT02.ICO
# End Source File
# Begin Source File

SOURCE=.\res\POINT03.ICO
# End Source File
# Begin Source File

SOURCE=.\res\POINT04.ICO
# End Source File
# Begin Source File

SOURCE=.\res\POINT05.ICO
# End Source File
# Begin Source File

SOURCE=.\res\POINT06.ICO
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\transform.cur
# End Source File
# End Group
# Begin Group "N3Indoor"

# PROP Default_Filter ""
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ComDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ComDialog.h
# End Source File
# Begin Source File

SOURCE=.\DlgBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgBrowsePath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBrowsePath.h
# End Source File
# Begin Source File

SOURCE=.\DlgShapeList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShapeList.h
# End Source File
# Begin Source File

SOURCE=.\DlgUnusedFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUnusedFiles.h
# End Source File
# Begin Source File

SOURCE=.\EnvironSoundPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EnvironSoundPage.h
# End Source File
# Begin Source File

SOURCE=.\EventEditPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EventEditPage.h
# End Source File
# Begin Source File

SOURCE=.\FloorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FloorDlg.h
# End Source File
# Begin Source File

SOURCE=.\MakeNpcPathPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeNpcPathPage.h
# End Source File
# Begin Source File

SOURCE=.\ShapeTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\TotalToolSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\TotalToolSheet.h
# End Source File
# Begin Source File

SOURCE=.\UserRegenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\UserRegenPage.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\N3Indoor.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Indoor.h
# End Source File
# Begin Source File

SOURCE=.\N3IndoorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\N3IndoorDoc.h
# End Source File
# Begin Source File

SOURCE=.\N3IndoorView.cpp
# End Source File
# Begin Source File

SOURCE=.\N3IndoorView.h
# End Source File
# Begin Source File

SOURCE=.\OrganizeView.cpp
# End Source File
# Begin Source File

SOURCE=.\OrganizeView.h
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.h"
# End Source File
# Begin Source File

SOURCE=.\ShellTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellTree.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "N3Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3Base\BitMapFile.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\BitMapFile.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\DFont.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\DFont.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\LogWriter.h
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

SOURCE=..\N3Base\N3Chr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Cloak.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Cloak.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Cloud.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Cloud.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ColorChange.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ColorChange.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXBundle.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXDef.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXGroup.h
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

SOURCE=..\N3Base\N3FXPartBottomBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBottomBoard.h
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

SOURCE=..\N3Base\N3FXPlug.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPlug.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffect.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffect.h
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

SOURCE=..\N3Base\N3Moon.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Moon.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TableBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\Pick.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\Pick.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\StdAfxBase.h
# End Source File
# End Group
# Begin Group "PVS New"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PortalFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\PortalFactory.h
# End Source File
# Begin Source File

SOURCE=.\PortalVolume.cpp
# End Source File
# Begin Source File

SOURCE=.\PortalVolume.h
# End Source File
# Begin Source File

SOURCE=.\PVSManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PVSManager.h
# End Source File
# End Group
# Begin Group "CDummy Series"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PosDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\PosDummy.h
# End Source File
# Begin Source File

SOURCE=.\RotDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\RotDummy.h
# End Source File
# Begin Source File

SOURCE=.\ScaleDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\ScaleDummy.h
# End Source File
# Begin Source File

SOURCE=.\SwappedDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\SwappedDummy.h
# End Source File
# Begin Source File

SOURCE=.\TransDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\TransDummy.h
# End Source File
# End Group
# Begin Group "N3Shape"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3Base\N3PMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshCreate.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshCreate.h
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

SOURCE=..\N3Base\N3ShapeMod.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMod.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.h
# End Source File
# End Group
# End Target
# End Project
