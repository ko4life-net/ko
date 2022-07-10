# Microsoft Developer Studio Generated NMAKE File, Based on ItemManager.dsp
!IF "$(CFG)" == ""
CFG=ItemManager - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ItemManager - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ItemManager - Win32 Release" && "$(CFG)" != "ItemManager - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ItemManager.mak" CFG="ItemManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ItemManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ItemManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ItemManager - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ItemManager.exe"


CLEAN :
	-@erase "$(INTDIR)\DBAgent.obj"
	-@erase "$(INTDIR)\ItemManager.obj"
	-@erase "$(INTDIR)\ItemManager.pch"
	-@erase "$(INTDIR)\ItemManager.res"
	-@erase "$(INTDIR)\ItemManagerDlg.obj"
	-@erase "$(INTDIR)\SharedMem.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ItemManager.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ItemManager.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ItemManager.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ItemManager.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ItemManager.pdb" /machine:I386 /out:"$(OUTDIR)\ItemManager.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DBAgent.obj" \
	"$(INTDIR)\ItemManager.obj" \
	"$(INTDIR)\ItemManagerDlg.obj" \
	"$(INTDIR)\SharedMem.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ItemManager.res"

"$(OUTDIR)\ItemManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ItemManager - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ItemManager.exe"


CLEAN :
	-@erase "$(INTDIR)\DBAgent.obj"
	-@erase "$(INTDIR)\ItemManager.obj"
	-@erase "$(INTDIR)\ItemManager.pch"
	-@erase "$(INTDIR)\ItemManager.res"
	-@erase "$(INTDIR)\ItemManagerDlg.obj"
	-@erase "$(INTDIR)\SharedMem.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ItemManager.exe"
	-@erase "$(OUTDIR)\ItemManager.ilk"
	-@erase "$(OUTDIR)\ItemManager.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ItemManager.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ItemManager.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ItemManager.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ItemManager.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ItemManager.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\DBAgent.obj" \
	"$(INTDIR)\ItemManager.obj" \
	"$(INTDIR)\ItemManagerDlg.obj" \
	"$(INTDIR)\SharedMem.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ItemManager.res"

"$(OUTDIR)\ItemManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ItemManager.dep")
!INCLUDE "ItemManager.dep"
!ELSE 
!MESSAGE Warning: cannot find "ItemManager.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ItemManager - Win32 Release" || "$(CFG)" == "ItemManager - Win32 Debug"
SOURCE=.\DBAgent.cpp

"$(INTDIR)\DBAgent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ItemManager.pch"


SOURCE=.\ItemManager.cpp

"$(INTDIR)\ItemManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ItemManager.pch"


SOURCE=.\ItemManager.rc

"$(INTDIR)\ItemManager.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ItemManagerDlg.cpp

"$(INTDIR)\ItemManagerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ItemManager.pch"


SOURCE=.\SharedMem.cpp

"$(INTDIR)\SharedMem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ItemManager.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ItemManager - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ItemManager.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ItemManager.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ItemManager - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ItemManager.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ItemManager.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

