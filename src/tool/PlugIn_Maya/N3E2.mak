# Microsoft Developer Studio Generated NMAKE File, Based on N3E2.dsp
!IF "$(CFG)" == ""
CFG=N3Export2 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to N3Export2 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "N3Export2 - Win32 Release" && "$(CFG)" != "N3Export2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "N3Export2 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\..\AW\Maya4.0\bin\plug-ins\N3Export2.mll"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\iffreader.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AnimControl.obj"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Chr.obj"
	-@erase "$(INTDIR)\N3E2Translator.obj"
	-@erase "$(INTDIR)\N3E2Wrapper.obj"
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
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3Joint.obj"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\Resource.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\N3Export2.exp"
	-@erase "$(OUTDIR)\N3Export2.lib"
	-@erase "..\..\..\..\AW\Maya4.0\bin\plug-ins\N3Export2.mll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "." /I "..\include" /I "..\..\..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NT_ENV" /D "NT_PLUGIN" /D "_N3TOOL" /Fp"$(INTDIR)\N3E2.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /D "_N3TOOL" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Resource.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3E2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib d3d9.lib d3dx9.lib dsound.lib dxguid.lib winmm.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\N3Export2.pdb" /machine:I386 /out:"C:\AW\Maya4.0\bin\plug-ins\N3Export2.mll" /implib:"$(OUTDIR)\N3Export2.lib" /libpath:"..\lib" /libpath:"..\..\..\lib" /export:initializePlugin /export:uninitializePlugin 
LINK32_OBJS= \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\iffreader.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3E2Translator.obj" \
	"$(INTDIR)\N3E2Wrapper.obj" \
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
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Resource.res" \
	"E:\Program\Maya\lib\Foundation.lib" \
	"E:\Program\Maya\lib\Image.lib" \
	"E:\Program\Maya\lib\libMDtAPI.lib" \
	"E:\Program\Maya\lib\libMocap.lib" \
	"E:\Program\Maya\lib\OpenMaya.lib" \
	"E:\Program\Maya\lib\OpenMayaAnim.lib" \
	"E:\Program\Maya\lib\OpenMayaFX.lib" \
	"E:\Program\Maya\lib\OpenMayaRender.lib" \
	"E:\Program\Maya\lib\OpenMayaUI.lib"

"..\..\..\..\AW\Maya4.0\bin\plug-ins\N3Export2.mll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "g:\N3Export2.mll" "$(OUTDIR)\N3E2.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\iffreader.obj"
	-@erase "$(INTDIR)\iffreader.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
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
	-@erase "$(INTDIR)\N3E2Translator.obj"
	-@erase "$(INTDIR)\N3E2Translator.sbr"
	-@erase "$(INTDIR)\N3E2Wrapper.obj"
	-@erase "$(INTDIR)\N3E2Wrapper.sbr"
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
	-@erase "$(INTDIR)\Resource.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\N3E2.bsc"
	-@erase "$(OUTDIR)\N3Export2.exp"
	-@erase "$(OUTDIR)\N3Export2.lib"
	-@erase "$(OUTDIR)\N3Export2.pdb"
	-@erase "g:\N3Export2.ilk"
	-@erase "g:\N3Export2.mll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\include" /I "..\..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NT_ENV" /D "NT_PLUGIN" /D "_N3TOOL" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\N3E2.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /D "_N3TOOL" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3E2.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\iffreader.sbr" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Chr.sbr" \
	"$(INTDIR)\N3E2Translator.sbr" \
	"$(INTDIR)\N3E2Wrapper.sbr" \
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
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Joint.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Mesh.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshCreate.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3Scene.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr"

"$(OUTDIR)\N3E2.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=d3d9.lib d3dx9d.lib dsound.lib dxguidd.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib nafxcwd.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\N3Export2.pdb" /debug /machine:I386 /out:"g:\N3Export2.mll" /implib:"$(OUTDIR)\N3Export2.lib" /pdbtype:sept /libpath:"..\lib" /libpath:"..\..\..\lib" /export:initializePlugin /export:uninitializePlugin 
LINK32_OBJS= \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\iffreader.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3E2Translator.obj" \
	"$(INTDIR)\N3E2Wrapper.obj" \
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
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Resource.res" \
	"E:\Program\Maya\lib\Foundation.lib" \
	"E:\Program\Maya\lib\Image.lib" \
	"E:\Program\Maya\lib\libMDtAPI.lib" \
	"E:\Program\Maya\lib\libMocap.lib" \
	"E:\Program\Maya\lib\OpenMaya.lib" \
	"E:\Program\Maya\lib\OpenMayaAnim.lib" \
	"E:\Program\Maya\lib\OpenMayaFX.lib" \
	"E:\Program\Maya\lib\OpenMayaRender.lib" \
	"E:\Program\Maya\lib\OpenMayaUI.lib"

"g:\N3Export2.mll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("N3E2.dep")
!INCLUDE "N3E2.dep"
!ELSE 
!MESSAGE Warning: cannot find "N3E2.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "N3Export2 - Win32 Release" || "$(CFG)" == "N3Export2 - Win32 Debug"
SOURCE=..\..\engine\N3Base\BitMapFile.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\BitMapFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\iffreader.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\iffreader.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\iffreader.obj"	"$(INTDIR)\iffreader.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\LogWriter.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\LogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\LogWriter.obj"	"$(INTDIR)\LogWriter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AlphaPrimitiveManager.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3AlphaPrimitiveManager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimControl.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3AnimControl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimKey.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3AnimKey.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Base.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3BaseFileAccess.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3BaseFileAccess.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Camera.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Camera.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Chr.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Chr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Chr.obj"	"$(INTDIR)\N3Chr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\N3E2Translator.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3E2Translator.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3E2Translator.obj"	"$(INTDIR)\N3E2Translator.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3E2Wrapper.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3E2Wrapper.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3E2Wrapper.obj"	"$(INTDIR)\N3E2Wrapper.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Eng.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3EngTool.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3EngTool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXBundle.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXBundle.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXBundle.obj"	"$(INTDIR)\N3FXBundle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXGroup.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXGroup.obj"	"$(INTDIR)\N3FXGroup.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBase.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXPartBase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXPartBase.obj"	"$(INTDIR)\N3FXPartBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBillBoard.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXPartBillBoard.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXPartBillBoard.obj"	"$(INTDIR)\N3FXPartBillBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartBottomBoard.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXPartBottomBoard.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXPartBottomBoard.obj"	"$(INTDIR)\N3FXPartBottomBoard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXParticle.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXParticle.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXParticle.obj"	"$(INTDIR)\N3FXParticle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartMesh.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXPartMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXPartMesh.obj"	"$(INTDIR)\N3FXPartMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPartParticles.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXPartParticles.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXPartParticles.obj"	"$(INTDIR)\N3FXPartParticles.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPlug.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXPlug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXPlug.obj"	"$(INTDIR)\N3FXPlug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPMesh.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXPMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXPMesh.obj"	"$(INTDIR)\N3FXPMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXPMeshInstance.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXPMeshInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXPMeshInstance.obj"	"$(INTDIR)\N3FXPMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3FXShape.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3FXShape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3FXShape.obj"	"$(INTDIR)\N3FXShape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3IMesh.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3IMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Joint.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Joint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Joint.obj"	"$(INTDIR)\N3Joint.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Light.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Light.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Mesh.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Mesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Mesh.obj"	"$(INTDIR)\N3Mesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMesh.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3PMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshCreate.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3PMeshCreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3PMeshCreate.obj"	"$(INTDIR)\N3PMeshCreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshInstance.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3PMeshInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Scene.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Scene.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Scene.obj"	"$(INTDIR)\N3Scene.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Shape.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Shape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Skin.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Skin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Skin.obj"	"$(INTDIR)\N3Skin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Texture.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Texture.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Transform.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3Transform.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3TransformCollision.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3TransformCollision.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3VMesh.cpp

!IF  "$(CFG)" == "N3Export2 - Win32 Release"


"$(INTDIR)\N3VMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3Export2 - Win32 Debug"


"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Resource.rc

"$(INTDIR)\Resource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

