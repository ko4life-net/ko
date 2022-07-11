# Microsoft Developer Studio Generated NMAKE File, Based on Aujard.dsp
!IF "$(CFG)" == ""
CFG=Aujard - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Aujard - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Aujard - Win32 Release" && "$(CFG)" != "Aujard - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Aujard.mak" CFG="Aujard - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Aujard - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Aujard - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Aujard - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Aujard.exe"


CLEAN :
	-@erase "$(INTDIR)\Aujard.obj"
	-@erase "$(INTDIR)\Aujard.pch"
	-@erase "$(INTDIR)\Aujard.res"
	-@erase "$(INTDIR)\AujardDlg.obj"
	-@erase "$(INTDIR)\DBAgent.obj"
	-@erase "$(INTDIR)\ItemTableSet.obj"
	-@erase "$(INTDIR)\SharedMem.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Aujard.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__SAMMA" /Fp"$(INTDIR)\Aujard.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Aujard.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Aujard.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Aujard.pdb" /machine:I386 /out:"$(OUTDIR)\Aujard.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Aujard.obj" \
	"$(INTDIR)\AujardDlg.obj" \
	"$(INTDIR)\DBAgent.obj" \
	"$(INTDIR)\ItemTableSet.obj" \
	"$(INTDIR)\SharedMem.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Aujard.res"

"$(OUTDIR)\Aujard.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Aujard - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Aujard.exe"


CLEAN :
	-@erase "$(INTDIR)\Aujard.obj"
	-@erase "$(INTDIR)\Aujard.pch"
	-@erase "$(INTDIR)\Aujard.res"
	-@erase "$(INTDIR)\AujardDlg.obj"
	-@erase "$(INTDIR)\DBAgent.obj"
	-@erase "$(INTDIR)\ItemTableSet.obj"
	-@erase "$(INTDIR)\SharedMem.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Aujard.exe"
	-@erase "$(OUTDIR)\Aujard.ilk"
	-@erase "$(OUTDIR)\Aujard.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__SAMMA" /Fp"$(INTDIR)\Aujard.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Aujard.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Aujard.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Aujard.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Aujard.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Aujard.obj" \
	"$(INTDIR)\AujardDlg.obj" \
	"$(INTDIR)\DBAgent.obj" \
	"$(INTDIR)\ItemTableSet.obj" \
	"$(INTDIR)\SharedMem.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Aujard.res"

"$(OUTDIR)\Aujard.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Aujard.dep")
!INCLUDE "Aujard.dep"
!ELSE 
!MESSAGE Warning: cannot find "Aujard.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Aujard - Win32 Release" || "$(CFG)" == "Aujard - Win32 Debug"
SOURCE=.\Aujard.cpp

"$(INTDIR)\Aujard.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Aujard.pch"


SOURCE=.\Aujard.rc

"$(INTDIR)\Aujard.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\AujardDlg.cpp

"$(INTDIR)\AujardDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Aujard.pch"


SOURCE=.\DBAgent.cpp

"$(INTDIR)\DBAgent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Aujard.pch"


SOURCE=.\ItemTableSet.cpp

"$(INTDIR)\ItemTableSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Aujard.pch"


SOURCE=.\SharedMem.cpp

"$(INTDIR)\SharedMem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Aujard.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Aujard - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__SAMMA" /Fp"$(INTDIR)\Aujard.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Aujard.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Aujard - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__SAMMA" /Fp"$(INTDIR)\Aujard.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Aujard.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

