# Microsoft Developer Studio Generated NMAKE File, Based on N3FE.dsp
!IF "$(CFG)" == ""
CFG=N3FE - Win32 Debug
!MESSAGE No configuration specified. Defaulting to N3FE - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "N3FE - Win32 Release" && "$(CFG)" != "N3FE - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3FE.mak" CFG="N3FE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3FE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3FE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "N3FE - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\N3FE.exe"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\DlgBrowsePath.obj"
	-@erase "$(INTDIR)\DlgRename.obj"
	-@erase "$(INTDIR)\FormViewControl.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AnimControl.obj"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3FE.obj"
	-@erase "$(INTDIR)\N3FE.res"
	-@erase "$(INTDIR)\N3FEView.obj"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\ShellTree.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\N3FE.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /Fp"$(INTDIR)\N3FE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3FE.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3FE.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\N3FE.pdb" /machine:I386 /out:"N3FE.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DlgBrowsePath.obj" \
	"$(INTDIR)\DlgRename.obj" \
	"$(INTDIR)\FormViewControl.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3FE.obj" \
	"$(INTDIR)\N3FEView.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\ShellTree.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\N3FE.res"

".\N3FE.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\N3FE_Debug.exe" "$(OUTDIR)\N3FE.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\DlgBrowsePath.obj"
	-@erase "$(INTDIR)\DlgBrowsePath.sbr"
	-@erase "$(INTDIR)\DlgRename.obj"
	-@erase "$(INTDIR)\DlgRename.sbr"
	-@erase "$(INTDIR)\FormViewControl.obj"
	-@erase "$(INTDIR)\FormViewControl.sbr"
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
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Camera.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3EngTool.sbr"
	-@erase "$(INTDIR)\N3FE.obj"
	-@erase "$(INTDIR)\N3FE.res"
	-@erase "$(INTDIR)\N3FE.sbr"
	-@erase "$(INTDIR)\N3FEView.obj"
	-@erase "$(INTDIR)\N3FEView.sbr"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3IMesh.sbr"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Light.sbr"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMesh.sbr"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.sbr"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.sbr"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Shape.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3TransformCollision.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\ShellTree.obj"
	-@erase "$(INTDIR)\ShellTree.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\N3FE.bsc"
	-@erase "$(OUTDIR)\N3FE_Debug.pdb"
	-@erase ".\N3FE_Debug.exe"
	-@erase ".\N3FE_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_N3TOOL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3FE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\N3FE.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3FE.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\DlgBrowsePath.sbr" \
	"$(INTDIR)\DlgRename.sbr" \
	"$(INTDIR)\FormViewControl.sbr" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3FE.sbr" \
	"$(INTDIR)\N3FEView.sbr" \
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshCreate.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\ShellTree.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\N3FE.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib d3d8.lib d3dx8.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\N3FE_Debug.pdb" /debug /machine:I386 /out:"N3FE_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DlgBrowsePath.obj" \
	"$(INTDIR)\DlgRename.obj" \
	"$(INTDIR)\FormViewControl.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3FE.obj" \
	"$(INTDIR)\N3FEView.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\ShellTree.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\N3FE.res"

".\N3FE_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("N3FE.dep")
!INCLUDE "N3FE.dep"
!ELSE 
!MESSAGE Warning: cannot find "N3FE.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "N3FE - Win32 Release" || "$(CFG)" == "N3FE - Win32 Debug"
SOURCE=..\..\engine\N3Base\BitMapFile.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\BitMapFile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\DlgBrowsePath.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\DlgBrowsePath.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\DlgBrowsePath.obj"	"$(INTDIR)\DlgBrowsePath.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgRename.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\DlgRename.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\DlgRename.obj"	"$(INTDIR)\DlgRename.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\FormViewControl.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\FormViewControl.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\FormViewControl.obj"	"$(INTDIR)\FormViewControl.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\LogWriter.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\LogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\LogWriter.obj"	"$(INTDIR)\LogWriter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AlphaPrimitiveManager.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3AlphaPrimitiveManager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimControl.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3AnimControl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3AnimKey.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3AnimKey.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Base.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3BaseFileAccess.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3BaseFileAccess.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Camera.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3Camera.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Eng.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3EngTool.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3EngTool.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\N3FE.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3FE.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3FE.obj"	"$(INTDIR)\N3FE.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3FE.rc

"$(INTDIR)\N3FE.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\N3FEView.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3FEView.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3FEView.obj"	"$(INTDIR)\N3FEView.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\..\engine\N3Base\N3IMesh.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3IMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Light.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3Light.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMesh.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3PMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshCreate.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3PMeshCreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3PMeshCreate.obj"	"$(INTDIR)\N3PMeshCreate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3PMeshInstance.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3PMeshInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Shape.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3Shape.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Texture.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3Texture.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3Transform.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3Transform.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3TransformCollision.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3TransformCollision.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\engine\N3Base\N3VMesh.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\N3VMesh.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE="..\Widget\PropertyList.cpp"

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\PropertyList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\PropertyList.obj"	"$(INTDIR)\PropertyList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ShellTree.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\ShellTree.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\ShellTree.obj"	"$(INTDIR)\ShellTree.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "N3FE - Win32 Release"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3FE - Win32 Debug"


"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

