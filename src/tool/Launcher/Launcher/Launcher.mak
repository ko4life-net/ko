# Microsoft Developer Studio Generated NMAKE File, Based on Launcher.dsp
!IF "$(CFG)" == ""
CFG=Launcher - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Launcher - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Launcher - Win32 Release" && "$(CFG)" != "Launcher - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Launcher.mak" CFG="Launcher - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Launcher - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Launcher - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Launcher - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\Launcher.exe" "$(OUTDIR)\Launcher.bsc"


CLEAN :
	-@erase "$(INTDIR)\APISocket.obj"
	-@erase "$(INTDIR)\APISocket.sbr"
	-@erase "$(INTDIR)\BackgroundUtil.obj"
	-@erase "$(INTDIR)\BackgroundUtil.sbr"
	-@erase "$(INTDIR)\BB_CircularBuffer.obj"
	-@erase "$(INTDIR)\BB_CircularBuffer.sbr"
	-@erase "$(INTDIR)\Launcher.obj"
	-@erase "$(INTDIR)\Launcher.pch"
	-@erase "$(INTDIR)\Launcher.res"
	-@erase "$(INTDIR)\Launcher.sbr"
	-@erase "$(INTDIR)\LauncherDlg.obj"
	-@erase "$(INTDIR)\LauncherDlg.sbr"
	-@erase "$(INTDIR)\MacProgressCtrl.obj"
	-@erase "$(INTDIR)\MacProgressCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Launcher.bsc"
	-@erase ".\Launcher.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Launcher.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Launcher.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Launcher.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\APISocket.sbr" \
	"$(INTDIR)\BackgroundUtil.sbr" \
	"$(INTDIR)\BB_CircularBuffer.sbr" \
	"$(INTDIR)\Launcher.sbr" \
	"$(INTDIR)\LauncherDlg.sbr" \
	"$(INTDIR)\MacProgressCtrl.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\Launcher.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ZipArchive.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Launcher.pdb" /machine:I386 /force /out:"Launcher.exe" 
LINK32_OBJS= \
	"$(INTDIR)\APISocket.obj" \
	"$(INTDIR)\BackgroundUtil.obj" \
	"$(INTDIR)\BB_CircularBuffer.obj" \
	"$(INTDIR)\Launcher.obj" \
	"$(INTDIR)\LauncherDlg.obj" \
	"$(INTDIR)\MacProgressCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Launcher.res"

".\Launcher.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Launcher - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\Launcher_Debug.exe" "$(OUTDIR)\Launcher.bsc"


CLEAN :
	-@erase "$(INTDIR)\APISocket.obj"
	-@erase "$(INTDIR)\APISocket.sbr"
	-@erase "$(INTDIR)\BackgroundUtil.obj"
	-@erase "$(INTDIR)\BackgroundUtil.sbr"
	-@erase "$(INTDIR)\BB_CircularBuffer.obj"
	-@erase "$(INTDIR)\BB_CircularBuffer.sbr"
	-@erase "$(INTDIR)\Launcher.obj"
	-@erase "$(INTDIR)\Launcher.pch"
	-@erase "$(INTDIR)\Launcher.res"
	-@erase "$(INTDIR)\Launcher.sbr"
	-@erase "$(INTDIR)\LauncherDlg.obj"
	-@erase "$(INTDIR)\LauncherDlg.sbr"
	-@erase "$(INTDIR)\MacProgressCtrl.obj"
	-@erase "$(INTDIR)\MacProgressCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Launcher.bsc"
	-@erase "$(OUTDIR)\Launcher_Debug.pdb"
	-@erase ".\Launcher_Debug.exe"
	-@erase ".\Launcher_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Launcher.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Launcher.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Launcher.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\APISocket.sbr" \
	"$(INTDIR)\BackgroundUtil.sbr" \
	"$(INTDIR)\BB_CircularBuffer.sbr" \
	"$(INTDIR)\Launcher.sbr" \
	"$(INTDIR)\LauncherDlg.sbr" \
	"$(INTDIR)\MacProgressCtrl.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\Launcher.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ZipArchive.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Launcher_Debug.pdb" /debug /machine:I386 /force /out:"Launcher_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\APISocket.obj" \
	"$(INTDIR)\BackgroundUtil.obj" \
	"$(INTDIR)\BB_CircularBuffer.obj" \
	"$(INTDIR)\Launcher.obj" \
	"$(INTDIR)\LauncherDlg.obj" \
	"$(INTDIR)\MacProgressCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Launcher.res"

".\Launcher_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Launcher.dep")
!INCLUDE "Launcher.dep"
!ELSE 
!MESSAGE Warning: cannot find "Launcher.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Launcher - Win32 Release" || "$(CFG)" == "Launcher - Win32 Debug"
SOURCE=.\APISocket.cpp

"$(INTDIR)\APISocket.obj"	"$(INTDIR)\APISocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Launcher.pch"


SOURCE=.\BackgroundUtil.cpp

"$(INTDIR)\BackgroundUtil.obj"	"$(INTDIR)\BackgroundUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Launcher.pch"


SOURCE=.\BB_CircularBuffer.cpp

"$(INTDIR)\BB_CircularBuffer.obj"	"$(INTDIR)\BB_CircularBuffer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Launcher.pch"


SOURCE=.\Launcher.cpp

"$(INTDIR)\Launcher.obj"	"$(INTDIR)\Launcher.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Launcher.pch"


SOURCE=.\Launcher.rc

"$(INTDIR)\Launcher.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\LauncherDlg.cpp

"$(INTDIR)\LauncherDlg.obj"	"$(INTDIR)\LauncherDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Launcher.pch"


SOURCE=.\MacProgressCtrl.cpp

"$(INTDIR)\MacProgressCtrl.obj"	"$(INTDIR)\MacProgressCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Launcher.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Launcher - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Launcher.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Launcher.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Launcher - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Launcher.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Launcher.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

