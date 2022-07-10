# Microsoft Developer Studio Generated NMAKE File, Based on VersionManager.dsp
!IF "$(CFG)" == ""
CFG=VersionManager - Win32 Debug
!MESSAGE No configuration specified. Defaulting to VersionManager - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "VersionManager - Win32 Release" && "$(CFG)" != "VersionManager - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VersionManager.mak" CFG="VersionManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VersionManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VersionManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "VersionManager - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\VersionManager.exe"


CLEAN :
	-@erase "$(INTDIR)\CircularBuffer.obj"
	-@erase "$(INTDIR)\DBProcess.obj"
	-@erase "$(INTDIR)\DlgBrowsePath.obj"
	-@erase "$(INTDIR)\IOCPort.obj"
	-@erase "$(INTDIR)\IOCPSocket2.obj"
	-@erase "$(INTDIR)\SettingDlg.obj"
	-@erase "$(INTDIR)\ShellTree.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VersionManager.obj"
	-@erase "$(INTDIR)\VersionManager.pch"
	-@erase "$(INTDIR)\VersionManager.res"
	-@erase "$(INTDIR)\VersionManagerDlg.obj"
	-@erase "$(INTDIR)\VersionSet.obj"
	-@erase "$(OUTDIR)\VersionManager.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\VersionManager.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\VersionManager.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VersionManager.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ws2_32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\VersionManager.pdb" /machine:I386 /force /out:"$(OUTDIR)\VersionManager.exe" 
LINK32_OBJS= \
	"$(INTDIR)\CircularBuffer.obj" \
	"$(INTDIR)\DBProcess.obj" \
	"$(INTDIR)\DlgBrowsePath.obj" \
	"$(INTDIR)\IOCPort.obj" \
	"$(INTDIR)\IOCPSocket2.obj" \
	"$(INTDIR)\SettingDlg.obj" \
	"$(INTDIR)\ShellTree.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\VersionManager.obj" \
	"$(INTDIR)\VersionManagerDlg.obj" \
	"$(INTDIR)\VersionSet.obj" \
	"$(INTDIR)\VersionManager.res"

"$(OUTDIR)\VersionManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "VersionManager - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\VersionManager.exe"


CLEAN :
	-@erase "$(INTDIR)\CircularBuffer.obj"
	-@erase "$(INTDIR)\DBProcess.obj"
	-@erase "$(INTDIR)\DlgBrowsePath.obj"
	-@erase "$(INTDIR)\IOCPort.obj"
	-@erase "$(INTDIR)\IOCPSocket2.obj"
	-@erase "$(INTDIR)\SettingDlg.obj"
	-@erase "$(INTDIR)\ShellTree.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VersionManager.obj"
	-@erase "$(INTDIR)\VersionManager.pch"
	-@erase "$(INTDIR)\VersionManager.res"
	-@erase "$(INTDIR)\VersionManagerDlg.obj"
	-@erase "$(INTDIR)\VersionSet.obj"
	-@erase "$(OUTDIR)\VersionManager.exe"
	-@erase "$(OUTDIR)\VersionManager.ilk"
	-@erase "$(OUTDIR)\VersionManager.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\VersionManager.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\VersionManager.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\VersionManager.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ws2_32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\VersionManager.pdb" /debug /machine:I386 /force /out:"$(OUTDIR)\VersionManager.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\CircularBuffer.obj" \
	"$(INTDIR)\DBProcess.obj" \
	"$(INTDIR)\DlgBrowsePath.obj" \
	"$(INTDIR)\IOCPort.obj" \
	"$(INTDIR)\IOCPSocket2.obj" \
	"$(INTDIR)\SettingDlg.obj" \
	"$(INTDIR)\ShellTree.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\VersionManager.obj" \
	"$(INTDIR)\VersionManagerDlg.obj" \
	"$(INTDIR)\VersionSet.obj" \
	"$(INTDIR)\VersionManager.res"

"$(OUTDIR)\VersionManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("VersionManager.dep")
!INCLUDE "VersionManager.dep"
!ELSE 
!MESSAGE Warning: cannot find "VersionManager.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "VersionManager - Win32 Release" || "$(CFG)" == "VersionManager - Win32 Debug"
SOURCE=.\CircularBuffer.cpp

"$(INTDIR)\CircularBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\DBProcess.cpp

"$(INTDIR)\DBProcess.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\DlgBrowsePath.cpp

"$(INTDIR)\DlgBrowsePath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\IOCPort.cpp

"$(INTDIR)\IOCPort.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\IOCPSocket2.cpp

"$(INTDIR)\IOCPSocket2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\SettingDlg.cpp

"$(INTDIR)\SettingDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\ShellTree.cpp

"$(INTDIR)\ShellTree.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "VersionManager - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\VersionManager.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\VersionManager.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "VersionManager - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\VersionManager.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\VersionManager.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\User.cpp

"$(INTDIR)\User.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\VersionManager.cpp

"$(INTDIR)\VersionManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\VersionManager.rc

"$(INTDIR)\VersionManager.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\VersionManagerDlg.cpp

"$(INTDIR)\VersionManagerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"


SOURCE=.\VersionSet.cpp

"$(INTDIR)\VersionSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\VersionManager.pch"



!ENDIF 

