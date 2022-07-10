# Microsoft Developer Studio Generated NMAKE File, Based on Option.dsp
!IF "$(CFG)" == ""
CFG=Option - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Option - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Option - Win32 Release" && "$(CFG)" != "Option - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Option.mak" CFG="Option - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Option - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Option - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Option - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\Option.exe"


CLEAN :
	-@erase "$(INTDIR)\Option.obj"
	-@erase "$(INTDIR)\Option.pch"
	-@erase "$(INTDIR)\Option.res"
	-@erase "$(INTDIR)\OptionDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\Option.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Option.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Option.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Option.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Option.pdb" /machine:I386 /out:"Option.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Option.obj" \
	"$(INTDIR)\OptionDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Option.res"

".\Option.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Option - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\Option_Debug.exe" "$(OUTDIR)\Option.bsc"


CLEAN :
	-@erase "$(INTDIR)\Option.obj"
	-@erase "$(INTDIR)\Option.pch"
	-@erase "$(INTDIR)\Option.res"
	-@erase "$(INTDIR)\Option.sbr"
	-@erase "$(INTDIR)\OptionDlg.obj"
	-@erase "$(INTDIR)\OptionDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Option.bsc"
	-@erase "$(OUTDIR)\Option_Debug.pdb"
	-@erase ".\Option_Debug.exe"
	-@erase ".\Option_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Option.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Option.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Option.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Option.sbr" \
	"$(INTDIR)\OptionDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\Option.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Option_Debug.pdb" /debug /machine:I386 /out:"Option_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Option.obj" \
	"$(INTDIR)\OptionDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Option.res"

".\Option_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Option.dep")
!INCLUDE "Option.dep"
!ELSE 
!MESSAGE Warning: cannot find "Option.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Option - Win32 Release" || "$(CFG)" == "Option - Win32 Debug"
SOURCE=.\Option.cpp

!IF  "$(CFG)" == "Option - Win32 Release"


"$(INTDIR)\Option.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Option.pch"


!ELSEIF  "$(CFG)" == "Option - Win32 Debug"


"$(INTDIR)\Option.obj"	"$(INTDIR)\Option.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Option.pch"


!ENDIF 

SOURCE=.\Option.rc

"$(INTDIR)\Option.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\OptionDlg.cpp

!IF  "$(CFG)" == "Option - Win32 Release"


"$(INTDIR)\OptionDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Option.pch"


!ELSEIF  "$(CFG)" == "Option - Win32 Debug"


"$(INTDIR)\OptionDlg.obj"	"$(INTDIR)\OptionDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Option.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Option - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Option.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Option.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Option - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Option.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Option.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

