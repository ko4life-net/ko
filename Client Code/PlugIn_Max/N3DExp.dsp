# Microsoft Developer Studio Project File - Name="N3DExp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=N3DExp - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "N3DExp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3DExp.mak" CFG="N3DExp - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3DExp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3DExp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/PlugIn_Max", RQAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "N3DExp - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GX /O2 /I "d:\3dsmax3_1\Maxsdk\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "NT_ENV" /D "NT_PLUGIN" /D "_N3TOOL" /YX"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "_N3TOOL" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comctl32.lib advapi32.lib d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib nafxcwd.lib libcmtd.lib core.lib geom.lib mesh.lib maxutil.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /machine:I386 /out:"N3DExp.dli" /release
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "N3DExp - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /ML /W3 /Gm /GX /ZI /Od /I "d:\3dsmax3_1\Maxsdk\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NT_ENV" /D "NT_PLUGIN" /D "_N3TOOL" /FR /YX"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /D "_N3TOOL" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comctl32.lib advapi32.lib d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib nafxcwd.lib libcmtd.lib core.lib geom.lib mesh.lib maxutil.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /debug /machine:I386 /out:"debug/N3DExp.dli" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\N3DExp.dli C:\3dsmax4\plugins\N3DExp42.dli
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "N3DExp - Win32 Release"
# Name "N3DExp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DllEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\N3DExp.cpp
# End Source File
# Begin Source File

SOURCE=.\N3DExp.def
# End Source File
# Begin Source File

SOURCE=.\N3DExp.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\N3DExp.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# Begin Group "Direct3D Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=C:\mssdk\include\d3d8.h
# End Source File
# Begin Source File

SOURCE=C:\mssdk\include\d3d8types.h
# End Source File
# Begin Source File

SOURCE=D:\MSSDK\include\d3dx8.h
# End Source File
# End Group
# Begin Group "Library"

# PROP Default_Filter "*.lib"
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

SOURCE=..\N3Base\My_Macro.h
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

SOURCE=..\N3Base\N3CompactSnd3dObj.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactSnd3dObj.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactSndDef.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactSndEng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactSndEng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactSndMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactSndMgr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactSndObj.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactSndObj.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactStreamSndObj.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3CompactStreamSndObj.h
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

SOURCE=..\N3Base\N3GlobalEffectMng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffectMng.h
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

SOURCE=..\N3Base\N3RiverPatch.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3RiverPatch.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeEx.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeEx.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeExtra.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeExtra.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMgr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMod.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMod.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Sky.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Sky.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndDef.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndMgr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndObj.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndObj.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndObjStream.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndObjStream.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Star.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Star.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Sun.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Sun.h
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

SOURCE=..\N3Base\Pick.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\Pick.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\StdAfxBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\WaveFile.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\WaveFile.h
# End Source File
# End Group
# Begin Group "Document"

# PROP Default_Filter "*.txt"
# Begin Source File

SOURCE=.\BipedExport.txt
# End Source File
# Begin Source File

SOURCE=.\PhysiqueExport.txt
# End Source File
# End Group
# End Target
# End Project
