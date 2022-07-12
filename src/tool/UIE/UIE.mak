# Microsoft Developer Studio Generated NMAKE File, Based on UIE.dsp
!IF "$(CFG)" == ""
CFG=UIE - Win32 Debug
!MESSAGE No configuration specified. Defaulting to UIE - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "UIE - Win32 Release" && "$(CFG)" != "UIE - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UIE.mak" CFG="UIE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UIE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "UIE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "UIE - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\UIE.exe" "$(OUTDIR)\UIE.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\DFont.obj"
	-@erase "$(INTDIR)\DFont.sbr"
	-@erase "$(INTDIR)\DlgAnimate.obj"
	-@erase "$(INTDIR)\DlgAnimate.sbr"
	-@erase "$(INTDIR)\DlgBar.obj"
	-@erase "$(INTDIR)\DlgBar.sbr"
	-@erase "$(INTDIR)\DlgChangeImage.obj"
	-@erase "$(INTDIR)\DlgChangeImage.sbr"
	-@erase "$(INTDIR)\DlgReplace.obj"
	-@erase "$(INTDIR)\DlgReplace.sbr"
	-@erase "$(INTDIR)\DlgTexture.obj"
	-@erase "$(INTDIR)\DlgTexture.sbr"
	-@erase "$(INTDIR)\DlgUnusedFileList.obj"
	-@erase "$(INTDIR)\DlgUnusedFileList.sbr"
	-@erase "$(INTDIR)\HierarchyView.obj"
	-@erase "$(INTDIR)\HierarchyView.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.sbr"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3Base.sbr"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3EngTool.sbr"
	-@erase "$(INTDIR)\N3SndMgr.obj"
	-@erase "$(INTDIR)\N3SndMgr.sbr"
	-@erase "$(INTDIR)\N3SndObj.obj"
	-@erase "$(INTDIR)\N3SndObj.sbr"
	-@erase "$(INTDIR)\N3SndObjStream.obj"
	-@erase "$(INTDIR)\N3SndObjStream.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3UIArea.obj"
	-@erase "$(INTDIR)\N3UIArea.sbr"
	-@erase "$(INTDIR)\N3UIBase.obj"
	-@erase "$(INTDIR)\N3UIBase.sbr"
	-@erase "$(INTDIR)\N3UIButton.obj"
	-@erase "$(INTDIR)\N3UIButton.sbr"
	-@erase "$(INTDIR)\N3UIEdit.obj"
	-@erase "$(INTDIR)\N3UIEdit.sbr"
	-@erase "$(INTDIR)\N3UIImage.obj"
	-@erase "$(INTDIR)\N3UIImage.sbr"
	-@erase "$(INTDIR)\N3UIList.obj"
	-@erase "$(INTDIR)\N3UIList.sbr"
	-@erase "$(INTDIR)\N3UIManager.obj"
	-@erase "$(INTDIR)\N3UIManager.sbr"
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
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\PropertyView.obj"
	-@erase "$(INTDIR)\PropertyView.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TexViewer.obj"
	-@erase "$(INTDIR)\TexViewer.sbr"
	-@erase "$(INTDIR)\UIE.obj"
	-@erase "$(INTDIR)\UIE.res"
	-@erase "$(INTDIR)\UIE.sbr"
	-@erase "$(INTDIR)\UIEDoc.obj"
	-@erase "$(INTDIR)\UIEDoc.sbr"
	-@erase "$(INTDIR)\UIEView.obj"
	-@erase "$(INTDIR)\UIEView.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WaveFile.obj"
	-@erase "$(INTDIR)\WaveFile.sbr"
	-@erase "$(OUTDIR)\UIE.bsc"
	-@erase ".\UIE.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3UIE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\UIE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\UIE.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UIE.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DlgChangeImage.sbr" \
	"$(INTDIR)\DlgUnusedFileList.sbr" \
	"$(INTDIR)\HierarchyView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3UIList.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\PropertyView.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\UIE.sbr" \
	"$(INTDIR)\UIEDoc.sbr" \
	"$(INTDIR)\UIEView.sbr" \
	"$(INTDIR)\N3UIArea.sbr" \
	"$(INTDIR)\N3UIBase.sbr" \
	"$(INTDIR)\N3UIButton.sbr" \
	"$(INTDIR)\N3UIEdit.sbr" \
	"$(INTDIR)\N3UIImage.sbr" \
	"$(INTDIR)\N3UIManager.sbr" \
	"$(INTDIR)\N3UIProgress.sbr" \
	"$(INTDIR)\N3UIScrollBar.sbr" \
	"$(INTDIR)\N3UIStatic.sbr" \
	"$(INTDIR)\N3UIString.sbr" \
	"$(INTDIR)\N3UITooltip.sbr" \
	"$(INTDIR)\N3UITrackBar.sbr" \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\DFont.sbr" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3SndMgr.sbr" \
	"$(INTDIR)\N3SndObj.sbr" \
	"$(INTDIR)\N3SndObjStream.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\WaveFile.sbr" \
	"$(INTDIR)\DlgTexture.sbr" \
	"$(INTDIR)\TexViewer.sbr" \
	"$(INTDIR)\DlgAnimate.sbr" \
	"$(INTDIR)\DlgBar.sbr" \
	"$(INTDIR)\DlgReplace.sbr"

"$(OUTDIR)\UIE.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\UIE.pdb" /machine:I386 /out:"UIE.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DlgChangeImage.obj" \
	"$(INTDIR)\DlgUnusedFileList.obj" \
	"$(INTDIR)\HierarchyView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3UIList.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\PropertyView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UIE.obj" \
	"$(INTDIR)\UIEDoc.obj" \
	"$(INTDIR)\UIEView.obj" \
	"$(INTDIR)\N3UIArea.obj" \
	"$(INTDIR)\N3UIBase.obj" \
	"$(INTDIR)\N3UIButton.obj" \
	"$(INTDIR)\N3UIEdit.obj" \
	"$(INTDIR)\N3UIImage.obj" \
	"$(INTDIR)\N3UIManager.obj" \
	"$(INTDIR)\N3UIProgress.obj" \
	"$(INTDIR)\N3UIScrollBar.obj" \
	"$(INTDIR)\N3UIStatic.obj" \
	"$(INTDIR)\N3UIString.obj" \
	"$(INTDIR)\N3UITooltip.obj" \
	"$(INTDIR)\N3UITrackBar.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DFont.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3SndMgr.obj" \
	"$(INTDIR)\N3SndObj.obj" \
	"$(INTDIR)\N3SndObjStream.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\WaveFile.obj" \
	"$(INTDIR)\DlgTexture.obj" \
	"$(INTDIR)\TexViewer.obj" \
	"$(INTDIR)\DlgAnimate.obj" \
	"$(INTDIR)\DlgBar.obj" \
	"$(INTDIR)\DlgReplace.obj" \
	"$(INTDIR)\UIE.res"

".\UIE.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "UIE - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\UIE_Debug.exe" "$(OUTDIR)\UIE.bsc"


CLEAN :
	-@erase "$(INTDIR)\BitMapFile.obj"
	-@erase "$(INTDIR)\BitMapFile.sbr"
	-@erase "$(INTDIR)\DFont.obj"
	-@erase "$(INTDIR)\DFont.sbr"
	-@erase "$(INTDIR)\DlgAnimate.obj"
	-@erase "$(INTDIR)\DlgAnimate.sbr"
	-@erase "$(INTDIR)\DlgBar.obj"
	-@erase "$(INTDIR)\DlgBar.sbr"
	-@erase "$(INTDIR)\DlgChangeImage.obj"
	-@erase "$(INTDIR)\DlgChangeImage.sbr"
	-@erase "$(INTDIR)\DlgReplace.obj"
	-@erase "$(INTDIR)\DlgReplace.sbr"
	-@erase "$(INTDIR)\DlgTexture.obj"
	-@erase "$(INTDIR)\DlgTexture.sbr"
	-@erase "$(INTDIR)\DlgUnusedFileList.obj"
	-@erase "$(INTDIR)\DlgUnusedFileList.sbr"
	-@erase "$(INTDIR)\HierarchyView.obj"
	-@erase "$(INTDIR)\HierarchyView.sbr"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\LogWriter.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.obj"
	-@erase "$(INTDIR)\N3AlphaPrimitiveManager.sbr"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3Base.sbr"
	-@erase "$(INTDIR)\N3BaseFileAccess.obj"
	-@erase "$(INTDIR)\N3BaseFileAccess.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3EngTool.obj"
	-@erase "$(INTDIR)\N3EngTool.sbr"
	-@erase "$(INTDIR)\N3SndMgr.obj"
	-@erase "$(INTDIR)\N3SndMgr.sbr"
	-@erase "$(INTDIR)\N3SndObj.obj"
	-@erase "$(INTDIR)\N3SndObj.sbr"
	-@erase "$(INTDIR)\N3SndObjStream.obj"
	-@erase "$(INTDIR)\N3SndObjStream.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3UIArea.obj"
	-@erase "$(INTDIR)\N3UIArea.sbr"
	-@erase "$(INTDIR)\N3UIBase.obj"
	-@erase "$(INTDIR)\N3UIBase.sbr"
	-@erase "$(INTDIR)\N3UIButton.obj"
	-@erase "$(INTDIR)\N3UIButton.sbr"
	-@erase "$(INTDIR)\N3UIEdit.obj"
	-@erase "$(INTDIR)\N3UIEdit.sbr"
	-@erase "$(INTDIR)\N3UIImage.obj"
	-@erase "$(INTDIR)\N3UIImage.sbr"
	-@erase "$(INTDIR)\N3UIList.obj"
	-@erase "$(INTDIR)\N3UIList.sbr"
	-@erase "$(INTDIR)\N3UIManager.obj"
	-@erase "$(INTDIR)\N3UIManager.sbr"
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
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\PropertyView.obj"
	-@erase "$(INTDIR)\PropertyView.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TexViewer.obj"
	-@erase "$(INTDIR)\TexViewer.sbr"
	-@erase "$(INTDIR)\UIE.obj"
	-@erase "$(INTDIR)\UIE.res"
	-@erase "$(INTDIR)\UIE.sbr"
	-@erase "$(INTDIR)\UIEDoc.obj"
	-@erase "$(INTDIR)\UIEDoc.sbr"
	-@erase "$(INTDIR)\UIEView.obj"
	-@erase "$(INTDIR)\UIEView.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WaveFile.obj"
	-@erase "$(INTDIR)\WaveFile.sbr"
	-@erase "$(OUTDIR)\UIE.bsc"
	-@erase "$(OUTDIR)\UIE_Debug.pdb"
	-@erase ".\UIE_Debug.exe"
	-@erase ".\UIE_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3UIE" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\UIE.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\UIE.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\UIE.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DlgChangeImage.sbr" \
	"$(INTDIR)\DlgUnusedFileList.sbr" \
	"$(INTDIR)\HierarchyView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\N3UIList.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\PropertyView.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\UIE.sbr" \
	"$(INTDIR)\UIEDoc.sbr" \
	"$(INTDIR)\UIEView.sbr" \
	"$(INTDIR)\N3UIArea.sbr" \
	"$(INTDIR)\N3UIBase.sbr" \
	"$(INTDIR)\N3UIButton.sbr" \
	"$(INTDIR)\N3UIEdit.sbr" \
	"$(INTDIR)\N3UIImage.sbr" \
	"$(INTDIR)\N3UIManager.sbr" \
	"$(INTDIR)\N3UIProgress.sbr" \
	"$(INTDIR)\N3UIScrollBar.sbr" \
	"$(INTDIR)\N3UIStatic.sbr" \
	"$(INTDIR)\N3UIString.sbr" \
	"$(INTDIR)\N3UITooltip.sbr" \
	"$(INTDIR)\N3UITrackBar.sbr" \
	"$(INTDIR)\BitMapFile.sbr" \
	"$(INTDIR)\DFont.sbr" \
	"$(INTDIR)\LogWriter.sbr" \
	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3BaseFileAccess.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3EngTool.sbr" \
	"$(INTDIR)\N3SndMgr.sbr" \
	"$(INTDIR)\N3SndObj.sbr" \
	"$(INTDIR)\N3SndObjStream.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\WaveFile.sbr" \
	"$(INTDIR)\DlgTexture.sbr" \
	"$(INTDIR)\TexViewer.sbr" \
	"$(INTDIR)\DlgAnimate.sbr" \
	"$(INTDIR)\DlgBar.sbr" \
	"$(INTDIR)\DlgReplace.sbr"

"$(OUTDIR)\UIE.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\UIE_Debug.pdb" /debug /machine:I386 /out:"UIE_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\DlgChangeImage.obj" \
	"$(INTDIR)\DlgUnusedFileList.obj" \
	"$(INTDIR)\HierarchyView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\N3UIList.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\PropertyView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UIE.obj" \
	"$(INTDIR)\UIEDoc.obj" \
	"$(INTDIR)\UIEView.obj" \
	"$(INTDIR)\N3UIArea.obj" \
	"$(INTDIR)\N3UIBase.obj" \
	"$(INTDIR)\N3UIButton.obj" \
	"$(INTDIR)\N3UIEdit.obj" \
	"$(INTDIR)\N3UIImage.obj" \
	"$(INTDIR)\N3UIManager.obj" \
	"$(INTDIR)\N3UIProgress.obj" \
	"$(INTDIR)\N3UIScrollBar.obj" \
	"$(INTDIR)\N3UIStatic.obj" \
	"$(INTDIR)\N3UIString.obj" \
	"$(INTDIR)\N3UITooltip.obj" \
	"$(INTDIR)\N3UITrackBar.obj" \
	"$(INTDIR)\BitMapFile.obj" \
	"$(INTDIR)\DFont.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\N3AlphaPrimitiveManager.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3BaseFileAccess.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3EngTool.obj" \
	"$(INTDIR)\N3SndMgr.obj" \
	"$(INTDIR)\N3SndObj.obj" \
	"$(INTDIR)\N3SndObjStream.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\WaveFile.obj" \
	"$(INTDIR)\DlgTexture.obj" \
	"$(INTDIR)\TexViewer.obj" \
	"$(INTDIR)\DlgAnimate.obj" \
	"$(INTDIR)\DlgBar.obj" \
	"$(INTDIR)\DlgReplace.obj" \
	"$(INTDIR)\UIE.res"

".\UIE_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("UIE.dep")
!INCLUDE "UIE.dep"
!ELSE 
!MESSAGE Warning: cannot find "UIE.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "UIE - Win32 Release" || "$(CFG)" == "UIE - Win32 Debug"
SOURCE=.\DlgChangeImage.cpp

"$(INTDIR)\DlgChangeImage.obj"	"$(INTDIR)\DlgChangeImage.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgUnusedFileList.cpp

"$(INTDIR)\DlgUnusedFileList.obj"	"$(INTDIR)\DlgUnusedFileList.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\HierarchyView.cpp

"$(INTDIR)\HierarchyView.obj"	"$(INTDIR)\HierarchyView.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\engine\N3Base\N3UIList.cpp

"$(INTDIR)\N3UIList.obj"	"$(INTDIR)\N3UIList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE="..\Widget\PropertyList.cpp"

"$(INTDIR)\PropertyList.obj"	"$(INTDIR)\PropertyList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\PropertyView.cpp

"$(INTDIR)\PropertyView.obj"	"$(INTDIR)\PropertyView.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIE.cpp

"$(INTDIR)\UIE.obj"	"$(INTDIR)\UIE.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIE.rc

"$(INTDIR)\UIE.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\UIEDoc.cpp

"$(INTDIR)\UIEDoc.obj"	"$(INTDIR)\UIEDoc.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\UIEView.cpp

"$(INTDIR)\UIEView.obj"	"$(INTDIR)\UIEView.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\engine\N3Base\N3UIArea.cpp

"$(INTDIR)\N3UIArea.obj"	"$(INTDIR)\N3UIArea.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIBase.cpp

"$(INTDIR)\N3UIBase.obj"	"$(INTDIR)\N3UIBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIButton.cpp

"$(INTDIR)\N3UIButton.obj"	"$(INTDIR)\N3UIButton.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIEdit.cpp

"$(INTDIR)\N3UIEdit.obj"	"$(INTDIR)\N3UIEdit.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIImage.cpp

"$(INTDIR)\N3UIImage.obj"	"$(INTDIR)\N3UIImage.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIManager.cpp

"$(INTDIR)\N3UIManager.obj"	"$(INTDIR)\N3UIManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIProgress.cpp

"$(INTDIR)\N3UIProgress.obj"	"$(INTDIR)\N3UIProgress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIScrollBar.cpp

"$(INTDIR)\N3UIScrollBar.obj"	"$(INTDIR)\N3UIScrollBar.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIStatic.cpp

"$(INTDIR)\N3UIStatic.obj"	"$(INTDIR)\N3UIStatic.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UIString.cpp

"$(INTDIR)\N3UIString.obj"	"$(INTDIR)\N3UIString.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UITooltip.cpp

"$(INTDIR)\N3UITooltip.obj"	"$(INTDIR)\N3UITooltip.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3UITrackBar.cpp

"$(INTDIR)\N3UITrackBar.obj"	"$(INTDIR)\N3UITrackBar.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\BitMapFile.cpp

"$(INTDIR)\BitMapFile.obj"	"$(INTDIR)\BitMapFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\DFont.cpp

"$(INTDIR)\DFont.obj"	"$(INTDIR)\DFont.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\LogWriter.cpp

"$(INTDIR)\LogWriter.obj"	"$(INTDIR)\LogWriter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3AlphaPrimitiveManager.cpp

"$(INTDIR)\N3AlphaPrimitiveManager.obj"	"$(INTDIR)\N3AlphaPrimitiveManager.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3Base.cpp

"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3BaseFileAccess.cpp

"$(INTDIR)\N3BaseFileAccess.obj"	"$(INTDIR)\N3BaseFileAccess.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3Eng.cpp

"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3EngTool.cpp

"$(INTDIR)\N3EngTool.obj"	"$(INTDIR)\N3EngTool.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3SndMgr.cpp

"$(INTDIR)\N3SndMgr.obj"	"$(INTDIR)\N3SndMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3SndObj.cpp

"$(INTDIR)\N3SndObj.obj"	"$(INTDIR)\N3SndObj.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3SndObjStream.cpp

"$(INTDIR)\N3SndObjStream.obj"	"$(INTDIR)\N3SndObjStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\N3Texture.cpp

"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\engine\N3Base\WaveFile.cpp

"$(INTDIR)\WaveFile.obj"	"$(INTDIR)\WaveFile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\DlgTexture.cpp

"$(INTDIR)\DlgTexture.obj"	"$(INTDIR)\DlgTexture.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TexViewer.cpp

"$(INTDIR)\TexViewer.obj"	"$(INTDIR)\TexViewer.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgAnimate.cpp

"$(INTDIR)\DlgAnimate.obj"	"$(INTDIR)\DlgAnimate.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgBar.cpp

"$(INTDIR)\DlgBar.obj"	"$(INTDIR)\DlgBar.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DlgReplace.cpp

"$(INTDIR)\DlgReplace.obj"	"$(INTDIR)\DlgReplace.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

