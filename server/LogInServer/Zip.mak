# Microsoft Developer Studio Generated NMAKE File, Based on Zip.dsp
!IF "$(CFG)" == ""
CFG=Zip - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Zip - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Zip - Win32 Release" && "$(CFG)" != "Zip - Win32 Debug" && "$(CFG)" != "Zip - Win32 Static Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Zip.mak" CFG="Zip - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Zip - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Zip - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Zip - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Zip - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Zip.lib"


CLEAN :
	-@erase "$(INTDIR)\AutoBuffer.obj"
	-@erase "$(INTDIR)\BigFile.obj"
	-@erase "$(INTDIR)\CentralDir.obj"
	-@erase "$(INTDIR)\FileHeader.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Zip.pch"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase "$(OUTDIR)\Zip.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /D "_AFXDLL" /Fp"$(INTDIR)\Zip.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Zip.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Zip.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AutoBuffer.obj" \
	"$(INTDIR)\BigFile.obj" \
	"$(INTDIR)\CentralDir.obj" \
	"$(INTDIR)\FileHeader.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

"$(OUTDIR)\Zip.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Zip.lib" "$(OUTDIR)\Zip.bsc"


CLEAN :
	-@erase "$(INTDIR)\AutoBuffer.obj"
	-@erase "$(INTDIR)\AutoBuffer.sbr"
	-@erase "$(INTDIR)\BigFile.obj"
	-@erase "$(INTDIR)\BigFile.sbr"
	-@erase "$(INTDIR)\CentralDir.obj"
	-@erase "$(INTDIR)\CentralDir.sbr"
	-@erase "$(INTDIR)\FileHeader.obj"
	-@erase "$(INTDIR)\FileHeader.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Zip.pch"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipArchive.sbr"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipException.sbr"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.sbr"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase "$(INTDIR)\ZipStorage.sbr"
	-@erase "$(OUTDIR)\Zip.bsc"
	-@erase "$(OUTDIR)\Zip.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W4 /Gm /GX /ZI /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Zip.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Zip.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AutoBuffer.sbr" \
	"$(INTDIR)\BigFile.sbr" \
	"$(INTDIR)\CentralDir.sbr" \
	"$(INTDIR)\FileHeader.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\ZipArchive.sbr" \
	"$(INTDIR)\ZipException.sbr" \
	"$(INTDIR)\ZipInternalInfo.sbr" \
	"$(INTDIR)\ZipStorage.sbr"

"$(OUTDIR)\Zip.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Zip.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AutoBuffer.obj" \
	"$(INTDIR)\BigFile.obj" \
	"$(INTDIR)\CentralDir.obj" \
	"$(INTDIR)\FileHeader.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

"$(OUTDIR)\Zip.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"

OUTDIR=.\Static_Release
INTDIR=.\Static_Release
# Begin Custom Macros
OutDir=.\Static_Release
# End Custom Macros

ALL : "$(OUTDIR)\Zip.lib"


CLEAN :
	-@erase "$(INTDIR)\AutoBuffer.obj"
	-@erase "$(INTDIR)\BigFile.obj"
	-@erase "$(INTDIR)\CentralDir.obj"
	-@erase "$(INTDIR)\FileHeader.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Zip.pch"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase "$(OUTDIR)\Zip.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /ZI /D "NDEBUG" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ZLIB_DLL" /Fp"$(INTDIR)\Zip.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Zip.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Zip.lib" 
LIB32_OBJS= \
	"$(INTDIR)\AutoBuffer.obj" \
	"$(INTDIR)\BigFile.obj" \
	"$(INTDIR)\CentralDir.obj" \
	"$(INTDIR)\FileHeader.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

"$(OUTDIR)\Zip.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Zip.dep")
!INCLUDE "Zip.dep"
!ELSE 
!MESSAGE Warning: cannot find "Zip.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Zip - Win32 Release" || "$(CFG)" == "Zip - Win32 Debug" || "$(CFG)" == "Zip - Win32 Static Release"
SOURCE=.\AutoBuffer.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"


"$(INTDIR)\AutoBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"


"$(INTDIR)\AutoBuffer.obj"	"$(INTDIR)\AutoBuffer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"


"$(INTDIR)\AutoBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ENDIF 

SOURCE=.\BigFile.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"


"$(INTDIR)\BigFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"


"$(INTDIR)\BigFile.obj"	"$(INTDIR)\BigFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"


"$(INTDIR)\BigFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ENDIF 

SOURCE=.\CentralDir.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"


"$(INTDIR)\CentralDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"


"$(INTDIR)\CentralDir.obj"	"$(INTDIR)\CentralDir.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"


"$(INTDIR)\CentralDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ENDIF 

SOURCE=.\FileHeader.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"


"$(INTDIR)\FileHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"


"$(INTDIR)\FileHeader.obj"	"$(INTDIR)\FileHeader.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"


"$(INTDIR)\FileHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /D "_AFXDLL" /Fp"$(INTDIR)\Zip.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Zip.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /Gm /GX /ZI /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Zip.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Zip.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /ZI /D "NDEBUG" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ZLIB_DLL" /Fp"$(INTDIR)\Zip.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Zip.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ZipArchive.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"


"$(INTDIR)\ZipArchive.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"


"$(INTDIR)\ZipArchive.obj"	"$(INTDIR)\ZipArchive.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"


"$(INTDIR)\ZipArchive.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ENDIF 

SOURCE=.\ZipException.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"


"$(INTDIR)\ZipException.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"


"$(INTDIR)\ZipException.obj"	"$(INTDIR)\ZipException.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"


"$(INTDIR)\ZipException.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ENDIF 

SOURCE=.\ZipInternalInfo.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"


"$(INTDIR)\ZipInternalInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"


"$(INTDIR)\ZipInternalInfo.obj"	"$(INTDIR)\ZipInternalInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"


"$(INTDIR)\ZipInternalInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ENDIF 

SOURCE=.\ZipStorage.cpp

!IF  "$(CFG)" == "Zip - Win32 Release"


"$(INTDIR)\ZipStorage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Debug"


"$(INTDIR)\ZipStorage.obj"	"$(INTDIR)\ZipStorage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ELSEIF  "$(CFG)" == "Zip - Win32 Static Release"


"$(INTDIR)\ZipStorage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Zip.pch"


!ENDIF 


!ENDIF 

