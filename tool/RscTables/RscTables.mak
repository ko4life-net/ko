# Microsoft Developer Studio Generated NMAKE File, Based on RscTables.dsp
!IF "$(CFG)" == ""
CFG=RscTables - Win32 Debug
!MESSAGE No configuration specified. Defaulting to RscTables - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "RscTables - Win32 Release" && "$(CFG)" != "RscTables - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RscTables.mak" CFG="RscTables - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RscTables - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RscTables - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "RscTables - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\RscTables.exe" "$(OUTDIR)\RscTables.pch"


CLEAN :
	-@erase "$(INTDIR)\DlgDataCount.obj"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\RscTables.obj"
	-@erase "$(INTDIR)\RscTables.pch"
	-@erase "$(INTDIR)\RscTables.res"
	-@erase "$(INTDIR)\RscTablesDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TableGenerator.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\RscTables.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\RscTables.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\RscTables.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RscTables.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\RscTables.pdb" /machine:I386 /out:"RscTables.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DlgDataCount.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\RscTables.obj" \
	"$(INTDIR)\RscTablesDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TableGenerator.obj" \
	"$(INTDIR)\RscTables.res"

".\RscTables.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RscTables - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\RscTables_Debug.exe" "$(OUTDIR)\RscTables.pch" "$(OUTDIR)\RscTables.bsc"


CLEAN :
	-@erase "$(INTDIR)\DlgDataCount.obj"
	-@erase "$(INTDIR)\DlgDataCount.sbr"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertyList.sbr"
	-@erase "$(INTDIR)\RscTables.obj"
	-@erase "$(INTDIR)\RscTables.pch"
	-@erase "$(INTDIR)\RscTables.res"
	-@erase "$(INTDIR)\RscTables.sbr"
	-@erase "$(INTDIR)\RscTablesDlg.obj"
	-@erase "$(INTDIR)\RscTablesDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TableGenerator.obj"
	-@erase "$(INTDIR)\TableGenerator.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\RscTables.bsc"
	-@erase "$(OUTDIR)\RscTables_Debug.pdb"
	-@erase ".\RscTables_Debug.exe"
	-@erase ".\RscTables_Debug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RscTables.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\RscTables.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RscTables.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DlgDataCount.sbr" \
	"$(INTDIR)\PropertyList.sbr" \
	"$(INTDIR)\RscTables.sbr" \
	"$(INTDIR)\RscTablesDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TableGenerator.sbr"

"$(OUTDIR)\RscTables.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\RscTables_Debug.pdb" /debug /machine:I386 /out:"RscTables_Debug.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\DlgDataCount.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\RscTables.obj" \
	"$(INTDIR)\RscTablesDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TableGenerator.obj" \
	"$(INTDIR)\RscTables.res"

".\RscTables_Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RscTables.dep")
!INCLUDE "RscTables.dep"
!ELSE 
!MESSAGE Warning: cannot find "RscTables.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RscTables - Win32 Release" || "$(CFG)" == "RscTables - Win32 Debug"
SOURCE=.\DlgDataCount.cpp

!IF  "$(CFG)" == "RscTables - Win32 Release"


"$(INTDIR)\DlgDataCount.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RscTables - Win32 Debug"


"$(INTDIR)\DlgDataCount.obj"	"$(INTDIR)\DlgDataCount.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE="..\Common Control\PropertyList.cpp"

!IF  "$(CFG)" == "RscTables - Win32 Release"


"$(INTDIR)\PropertyList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "RscTables - Win32 Debug"


"$(INTDIR)\PropertyList.obj"	"$(INTDIR)\PropertyList.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\RscTables.cpp

!IF  "$(CFG)" == "RscTables - Win32 Release"


"$(INTDIR)\RscTables.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RscTables - Win32 Debug"


"$(INTDIR)\RscTables.obj"	"$(INTDIR)\RscTables.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RscTables.rc

"$(INTDIR)\RscTables.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\RscTablesDlg.cpp

!IF  "$(CFG)" == "RscTables - Win32 Release"


"$(INTDIR)\RscTablesDlg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RscTables - Win32 Debug"


"$(INTDIR)\RscTablesDlg.obj"	"$(INTDIR)\RscTablesDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "RscTables - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\RscTables.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\RscTables.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "RscTables - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RscTables.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\RscTables.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TableGenerator.cpp

!IF  "$(CFG)" == "RscTables - Win32 Release"


"$(INTDIR)\TableGenerator.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RscTables - Win32 Debug"


"$(INTDIR)\TableGenerator.obj"	"$(INTDIR)\TableGenerator.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

