# Microsoft Developer Studio Project File - Name="N3Export2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=N3Export2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "N3E2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3E2.mak" CFG="N3Export2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3Export2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "N3Export2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/PlugIn_Maya", PPEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "N3Export2 - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I "..\include" /I "..\..\..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NT_ENV" /D "NT_PLUGIN" /D "_N3TOOL" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /D "_N3TOOL" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"C:\AW\Maya4.0\bin\plug-ins\N3Export2.mll" /libpath:"..\lib" /libpath:"..\..\..\lib" /export:initializePlugin /export:uninitializePlugin
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\include" /I "..\..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NT_ENV" /D "NT_PLUGIN" /D "_N3TOOL" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /D "_N3TOOL" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib nafxcwd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"g:\N3Export2.mll" /pdbtype:sept /libpath:"..\lib" /libpath:"..\..\..\lib" /export:initializePlugin /export:uninitializePlugin
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "N3Export2 - Win32 Release"
# Name "N3Export2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;c++"
# Begin Source File

SOURCE=..\N3BASE\BitMapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\iffreader.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimControl.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimKey.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.cpp
# End Source File
# Begin Source File

SOURCE=.\N3E2Translator.cpp
# End Source File
# Begin Source File

SOURCE=.\N3E2Wrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartBase.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartBillBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartBottomBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXParticle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartParticles.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPlug.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXShape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3IMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Joint.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Light.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshCreate.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\N3BASE\BitMapFile.h
# End Source File
# Begin Source File

SOURCE=.\iffreader.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\LogWriter.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimControl.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimKey.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Camera.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.h
# End Source File
# Begin Source File

SOURCE=.\N3E2Translator.h
# End Source File
# Begin Source File

SOURCE=.\N3E2Wrapper.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXBundle.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXDef.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXGroup.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartBase.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartBillBoard.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartBottomBoard.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXParticle.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPartParticles.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPlug.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXShape.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3IMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Joint.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Light.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshCreate.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\StdAfxBase.h
# End Source File
# End Group
# Begin Group "Mel Scripts"

# PROP Default_Filter "mel"
# Begin Source File

SOURCE=.\N3E2MatAttr.mel
# End Source File
# Begin Source File

SOURCE=.\N3E2tags.mel
# End Source File
# Begin Source File

SOURCE=.\N3E2TranslatorOpts.mel
# End Source File
# End Group
# Begin Group "Reference Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=D:\MSSDK\include\d3d8.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\flib.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\ilib.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\M3dView.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAngle.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAnimControl.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAnimCurveChange.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAnimCurveClipboard.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAnimCurveClipboardItem.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAnimCurveClipboardItemArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAnimMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAnimUtil.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MApiVersion.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MArgDatabase.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MArgList.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MArgParser.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MArrayDataBuilder.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MArrayDataHandle.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAttributeIndex.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAttributeSpec.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MAttributeSpecArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MBoundingBox.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MColor.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MColorArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MCommandResult.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MComputation.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MConditionMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MCursor.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDagModifier.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDagPath.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDagPathArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDataBlock.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDataHandle.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDeviceChannel.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDeviceState.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDGContext.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDGMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDGModifier.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDistance.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDoubleArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDrawData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDrawInfo.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDrawRequest.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDrawRequestQueue.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDynSweptLine.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MDynSweptTriangle.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MEulerRotation.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MEvent.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MEventMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFeedbackLine.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFileIO.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFileObject.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFloatArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFloatMatrix.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFloatPoint.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFloatPointArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFloatVector.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFloatVectorArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFn.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnAirField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnAmbientLight.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnAnimCurve.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnAreaLight.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnArrayAttrsData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnBase.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnBlendShapeDeformer.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnBlinnShader.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnCamera.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnCharacter.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnCircleSweepManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnClip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnComponent.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnComponentListData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnCompoundAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnCurveSegmentManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDagNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDependencyNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDirectionalLight.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDirectionManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDiscManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDistanceManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDoubleArrayData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDoubleIndexedComponent.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDragField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnDynSweptGeometryData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnEnumAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnExpression.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnFreePointTriadManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnGenericAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnGeometryData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnGeometryFilter.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnGravityField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnIkEffector.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnIkHandle.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnIkJoint.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnIkSolver.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnIntArrayData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnLambertShader.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnLattice.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnLatticeData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnLatticeDeformer.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnLight.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnLightDataAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnManip3D.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnMatrixAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnMatrixData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnMesh.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnMeshData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnMessageAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnMotionPath.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNewtonField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNonAmbientLight.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNonExtendedLight.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNumericAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNumericData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNurbsCurve.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNurbsCurveData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNurbsSurface.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnNurbsSurfaceData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnPartition.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnPhongShader.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnPlugin.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnPluginData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnPointArrayData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnPointLight.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnPointOnCurveManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnPointOnSurfaceManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnRadialField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnReflectShader.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnSet.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnSingleIndexedComponent.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnSkinCluster.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnSphereData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnSpotLight.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnStateManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnStringArrayData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnStringData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnSubd.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnSubdData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnSubdNames.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnToggleManip.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnTransform.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnTripleIndexedComponent.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnTurbulenceField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnTypedAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnUInt64ArrayData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnUniformField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnUnitAttribute.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnVectorArrayData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnVolumeAxisField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnVortexField.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnWeightGeometryFilter.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MFnWireDeformer.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MGlobal.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MIkHandleGroup.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MIkSystem.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MIntArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItCurveCV.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItDag.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItDependencyGraph.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItDependencyNodes.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItGeometry.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItKeyframe.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItMeshEdge.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItMeshPolygon.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItMeshVertex.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItSelectionList.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MItSurfaceCV.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MLibrary.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MManipData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MMaterial.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MMatrix.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MModelMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MNodeMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MObject.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MObjectArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\mocapserial.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\mocapserver.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\mocaptcp.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPlug.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPlugArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPoint.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPointArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxCommand.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxContext.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxContextCommand.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxDeformerNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxEmitterNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxFieldNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxFileTranslator.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxGeometryData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxGeometryIterator.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxGlBuffer.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxHwShaderNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxIkSolver.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxIkSolverNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxLocatorNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxManipContainer.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxMidiInputDevice.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxSelectionContext.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxSpringNode.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxSurfaceShape.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxSurfaceShapeUI.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MPxToolCommand.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MQuaternion.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MRenderCallback.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MRenderData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MRenderShadowData.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MRenderUtil.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MSceneMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MSelectInfo.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MSelectionList.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MSelectionMask.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MSimple.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MStatus.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MString.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MStringArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MSyntax.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MTesselationParams.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MTime.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MTimeArray.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MToolsInfo.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MTransformationMatrix.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MTypeId.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MTypes.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MUiMessage.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MUint64Array.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MVector.h
# End Source File
# Begin Source File

SOURCE=C:\AW\Maya4.0\include\maya\MVectorArray.h
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# End Group
# Begin Group "Maya Library"

# PROP Default_Filter "lib;"
# Begin Source File

SOURCE=E:\Program\Maya\lib\Foundation.lib
# End Source File
# Begin Source File

SOURCE=E:\Program\Maya\lib\Image.lib
# End Source File
# Begin Source File

SOURCE=E:\Program\Maya\lib\libMDtAPI.lib
# End Source File
# Begin Source File

SOURCE=E:\Program\Maya\lib\libMocap.lib
# End Source File
# Begin Source File

SOURCE=E:\Program\Maya\lib\OpenMaya.lib
# End Source File
# Begin Source File

SOURCE=E:\Program\Maya\lib\OpenMayaAnim.lib
# End Source File
# Begin Source File

SOURCE=E:\Program\Maya\lib\OpenMayaFX.lib
# End Source File
# Begin Source File

SOURCE=E:\Program\Maya\lib\OpenMayaRender.lib
# End Source File
# Begin Source File

SOURCE=E:\Program\Maya\lib\OpenMayaUI.lib
# End Source File
# End Group
# End Target
# End Project
