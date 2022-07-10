# Microsoft Developer Studio Generated NMAKE File, Based on Ebenezer.dsp
!IF "$(CFG)" == ""
CFG=Ebenezer - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Ebenezer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Ebenezer - Win32 Release" && "$(CFG)" != "Ebenezer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ebenezer.mak" CFG="Ebenezer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ebenezer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Ebenezer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Ebenezer.exe"


CLEAN :
	-@erase "$(INTDIR)\CircularBuffer.obj"
	-@erase "$(INTDIR)\Ebenezer.obj"
	-@erase "$(INTDIR)\Ebenezer.pch"
	-@erase "$(INTDIR)\Ebenezer.res"
	-@erase "$(INTDIR)\EbenezerDlg.obj"
	-@erase "$(INTDIR)\Global.obj"
	-@erase "$(INTDIR)\IOCPort.obj"
	-@erase "$(INTDIR)\IOCPSocket2.obj"
	-@erase "$(INTDIR)\Map.obj"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\Region.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Ebenezer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Ebenezer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Ebenezer.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ebenezer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ws2_32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Ebenezer.pdb" /machine:I386 /out:"$(OUTDIR)\Ebenezer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\CircularBuffer.obj" \
	"$(INTDIR)\Ebenezer.obj" \
	"$(INTDIR)\EbenezerDlg.obj" \
	"$(INTDIR)\Global.obj" \
	"$(INTDIR)\IOCPort.obj" \
	"$(INTDIR)\IOCPSocket2.obj" \
	"$(INTDIR)\Map.obj" \
	"$(INTDIR)\Region.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Ebenezer.res"

"$(OUTDIR)\Ebenezer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\Ebenezer.exe" "$(OUTDIR)\Ebenezer.bsc"


CLEAN :
	-@erase "$(INTDIR)\CircularBuffer.obj"
	-@erase "$(INTDIR)\CircularBuffer.sbr"
	-@erase "$(INTDIR)\Ebenezer.obj"
	-@erase "$(INTDIR)\Ebenezer.pch"
	-@erase "$(INTDIR)\Ebenezer.res"
	-@erase "$(INTDIR)\Ebenezer.sbr"
	-@erase "$(INTDIR)\EbenezerDlg.obj"
	-@erase "$(INTDIR)\EbenezerDlg.sbr"
	-@erase "$(INTDIR)\Global.obj"
	-@erase "$(INTDIR)\Global.sbr"
	-@erase "$(INTDIR)\IOCPort.obj"
	-@erase "$(INTDIR)\IOCPort.sbr"
	-@erase "$(INTDIR)\IOCPSocket2.obj"
	-@erase "$(INTDIR)\IOCPSocket2.sbr"
	-@erase "$(INTDIR)\Map.obj"
	-@erase "$(INTDIR)\Map.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\Region.obj"
	-@erase "$(INTDIR)\Region.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\User.obj"
	-@erase "$(INTDIR)\User.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Ebenezer.bsc"
	-@erase "$(OUTDIR)\Ebenezer.pdb"
	-@erase ".\Ebenezer.exe"
	-@erase ".\Ebenezer.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\Ebenezer.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ebenezer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CircularBuffer.sbr" \
	"$(INTDIR)\Ebenezer.sbr" \
	"$(INTDIR)\EbenezerDlg.sbr" \
	"$(INTDIR)\Global.sbr" \
	"$(INTDIR)\IOCPort.sbr" \
	"$(INTDIR)\IOCPSocket2.sbr" \
	"$(INTDIR)\Map.sbr" \
	"$(INTDIR)\Region.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\User.sbr" \
	"$(INTDIR)\N3VMesh.sbr"

"$(OUTDIR)\Ebenezer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ws2_32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Ebenezer.pdb" /debug /machine:I386 /out:"Ebenezer.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\CircularBuffer.obj" \
	"$(INTDIR)\Ebenezer.obj" \
	"$(INTDIR)\EbenezerDlg.obj" \
	"$(INTDIR)\Global.obj" \
	"$(INTDIR)\IOCPort.obj" \
	"$(INTDIR)\IOCPSocket2.obj" \
	"$(INTDIR)\Map.obj" \
	"$(INTDIR)\Region.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\User.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Ebenezer.res"

".\Ebenezer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Ebenezer.dep")
!INCLUDE "Ebenezer.dep"
!ELSE 
!MESSAGE Warning: cannot find "Ebenezer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Ebenezer - Win32 Release" || "$(CFG)" == "Ebenezer - Win32 Debug"
SOURCE=.\CircularBuffer.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\CircularBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\CircularBuffer.obj"	"$(INTDIR)\CircularBuffer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\Ebenezer.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\Ebenezer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\Ebenezer.obj"	"$(INTDIR)\Ebenezer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\Ebenezer.rc

"$(INTDIR)\Ebenezer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\EbenezerDlg.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\EbenezerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\EbenezerDlg.obj"	"$(INTDIR)\EbenezerDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\Global.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\Global.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\Global.obj"	"$(INTDIR)\Global.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\IOCPort.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\IOCPort.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\IOCPort.obj"	"$(INTDIR)\IOCPort.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\IOCPSocket2.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\IOCPSocket2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\IOCPSocket2.obj"	"$(INTDIR)\IOCPSocket2.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\Map.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\Map.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\Map.obj"	"$(INTDIR)\Map.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\Region.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\Region.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\Region.obj"	"$(INTDIR)\Region.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Ebenezer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Ebenezer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ebenezer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Ebenezer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\User.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\User.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\User.obj"	"$(INTDIR)\User.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"


!ENDIF 

SOURCE=.\N3Base\N3VMesh.cpp

!IF  "$(CFG)" == "Ebenezer - Win32 Release"


"$(INTDIR)\N3VMesh.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Ebenezer - Win32 Debug"


"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ebenezer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

