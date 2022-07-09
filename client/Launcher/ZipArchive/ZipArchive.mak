# Microsoft Developer Studio Generated NMAKE File, Based on ZipArchive.dsp
!IF "$(CFG)" == ""
CFG=ZipArchive - Win32 Unicode Debug
!MESSAGE No configuration specified. Defaulting to ZipArchive - Win32 Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "ZipArchive - Win32 Release" && "$(CFG)" != "ZipArchive - Win32 Debug" && "$(CFG)" != "ZipArchive - Win32 Static Release" && "$(CFG)" != "ZipArchive - Win32 Unicode Debug" && "$(CFG)" != "ZipArchive - Win32 Unicode Release" && "$(CFG)" != "ZipArchive - Win32 Unicode Static Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZipArchive.mak" CFG="ZipArchive - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZipArchive - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ZipArchive - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ZipArchive - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ZipArchive - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ZipArchive - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ZipArchive - Win32 Unicode Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ZipArchive - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\ZipArchive.lib"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipArchive.pch"
	-@erase "$(INTDIR)\ZipAutoBuffer.obj"
	-@erase "$(INTDIR)\ZipBigFile.obj"
	-@erase "$(INTDIR)\ZipCentralDir.obj"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipFileHeader.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase ".\ZipArchive.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Fp"$(INTDIR)\ZipArchive.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ZipArchive.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"ZipArchive.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipAutoBuffer.obj" \
	"$(INTDIR)\ZipBigFile.obj" \
	"$(INTDIR)\ZipCentralDir.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipFileHeader.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

".\ZipArchive.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\ZipArchive_Debug.lib" "$(OUTDIR)\ZipArchive.bsc"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipArchive.pch"
	-@erase "$(INTDIR)\ZipArchive.sbr"
	-@erase "$(INTDIR)\ZipAutoBuffer.obj"
	-@erase "$(INTDIR)\ZipAutoBuffer.sbr"
	-@erase "$(INTDIR)\ZipBigFile.obj"
	-@erase "$(INTDIR)\ZipBigFile.sbr"
	-@erase "$(INTDIR)\ZipCentralDir.obj"
	-@erase "$(INTDIR)\ZipCentralDir.sbr"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipException.sbr"
	-@erase "$(INTDIR)\ZipFileHeader.obj"
	-@erase "$(INTDIR)\ZipFileHeader.sbr"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.sbr"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase "$(INTDIR)\ZipStorage.sbr"
	-@erase "$(OUTDIR)\ZipArchive.bsc"
	-@erase ".\ZipArchive_Debug.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\ZipArchive.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ZipArchive.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\ZipArchive.sbr" \
	"$(INTDIR)\ZipAutoBuffer.sbr" \
	"$(INTDIR)\ZipBigFile.sbr" \
	"$(INTDIR)\ZipCentralDir.sbr" \
	"$(INTDIR)\ZipException.sbr" \
	"$(INTDIR)\ZipFileHeader.sbr" \
	"$(INTDIR)\ZipInternalInfo.sbr" \
	"$(INTDIR)\ZipStorage.sbr"

"$(OUTDIR)\ZipArchive.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"ZipArchive_Debug.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipAutoBuffer.obj" \
	"$(INTDIR)\ZipBigFile.obj" \
	"$(INTDIR)\ZipCentralDir.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipFileHeader.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

".\ZipArchive_Debug.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"

OUTDIR=.\Static_Release
INTDIR=.\Static_Release
# Begin Custom Macros
OutDir=.\Static_Release
# End Custom Macros

ALL : "$(OUTDIR)\ZipArchive.lib"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipArchive.pch"
	-@erase "$(INTDIR)\ZipAutoBuffer.obj"
	-@erase "$(INTDIR)\ZipBigFile.obj"
	-@erase "$(INTDIR)\ZipCentralDir.obj"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipFileHeader.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase "$(OUTDIR)\ZipArchive.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Fp"$(INTDIR)\ZipArchive.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ZipArchive.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\ZipArchive.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipAutoBuffer.obj" \
	"$(INTDIR)\ZipBigFile.obj" \
	"$(INTDIR)\ZipCentralDir.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipFileHeader.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

"$(OUTDIR)\ZipArchive.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"

OUTDIR=.\Unicode_Debug
INTDIR=.\Unicode_Debug
# Begin Custom Macros
OutDir=.\Unicode_Debug
# End Custom Macros

ALL : "$(OUTDIR)\ZipArchive.lib" "$(OUTDIR)\ZipArchive.bsc"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipArchive.pch"
	-@erase "$(INTDIR)\ZipArchive.sbr"
	-@erase "$(INTDIR)\ZipAutoBuffer.obj"
	-@erase "$(INTDIR)\ZipAutoBuffer.sbr"
	-@erase "$(INTDIR)\ZipBigFile.obj"
	-@erase "$(INTDIR)\ZipBigFile.sbr"
	-@erase "$(INTDIR)\ZipCentralDir.obj"
	-@erase "$(INTDIR)\ZipCentralDir.sbr"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipException.sbr"
	-@erase "$(INTDIR)\ZipFileHeader.obj"
	-@erase "$(INTDIR)\ZipFileHeader.sbr"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.sbr"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase "$(INTDIR)\ZipStorage.sbr"
	-@erase "$(OUTDIR)\ZipArchive.bsc"
	-@erase "$(OUTDIR)\ZipArchive.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od /D "ZLIB_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\ZipArchive.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ZipArchive.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\ZipArchive.sbr" \
	"$(INTDIR)\ZipAutoBuffer.sbr" \
	"$(INTDIR)\ZipBigFile.sbr" \
	"$(INTDIR)\ZipCentralDir.sbr" \
	"$(INTDIR)\ZipException.sbr" \
	"$(INTDIR)\ZipFileHeader.sbr" \
	"$(INTDIR)\ZipInternalInfo.sbr" \
	"$(INTDIR)\ZipStorage.sbr"

"$(OUTDIR)\ZipArchive.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\ZipArchive.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipAutoBuffer.obj" \
	"$(INTDIR)\ZipBigFile.obj" \
	"$(INTDIR)\ZipCentralDir.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipFileHeader.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

"$(OUTDIR)\ZipArchive.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"

OUTDIR=.\Unicode_Release
INTDIR=.\Unicode_Release
# Begin Custom Macros
OutDir=.\Unicode_Release
# End Custom Macros

ALL : "$(OUTDIR)\ZipArchive.lib"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipArchive.pch"
	-@erase "$(INTDIR)\ZipAutoBuffer.obj"
	-@erase "$(INTDIR)\ZipBigFile.obj"
	-@erase "$(INTDIR)\ZipCentralDir.obj"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipFileHeader.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase "$(OUTDIR)\ZipArchive.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "ZLIB_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)\ZipArchive.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ZipArchive.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\ZipArchive.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipAutoBuffer.obj" \
	"$(INTDIR)\ZipBigFile.obj" \
	"$(INTDIR)\ZipCentralDir.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipFileHeader.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

"$(OUTDIR)\ZipArchive.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"

OUTDIR=.\Unicode_Static_Release
INTDIR=.\Unicode_Static_Release
# Begin Custom Macros
OutDir=.\Unicode_Static_Release
# End Custom Macros

ALL : "$(OUTDIR)\ZipArchive.lib"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ZipArchive.obj"
	-@erase "$(INTDIR)\ZipArchive.pch"
	-@erase "$(INTDIR)\ZipAutoBuffer.obj"
	-@erase "$(INTDIR)\ZipBigFile.obj"
	-@erase "$(INTDIR)\ZipCentralDir.obj"
	-@erase "$(INTDIR)\ZipException.obj"
	-@erase "$(INTDIR)\ZipFileHeader.obj"
	-@erase "$(INTDIR)\ZipInternalInfo.obj"
	-@erase "$(INTDIR)\ZipStorage.obj"
	-@erase "$(OUTDIR)\ZipArchive.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /D "_UNICODE" /Fp"$(INTDIR)\ZipArchive.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ZipArchive.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\ZipArchive.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ZipArchive.obj" \
	"$(INTDIR)\ZipAutoBuffer.obj" \
	"$(INTDIR)\ZipBigFile.obj" \
	"$(INTDIR)\ZipCentralDir.obj" \
	"$(INTDIR)\ZipException.obj" \
	"$(INTDIR)\ZipFileHeader.obj" \
	"$(INTDIR)\ZipInternalInfo.obj" \
	"$(INTDIR)\ZipStorage.obj" \
	".\zlib.lib"

"$(OUTDIR)\ZipArchive.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ZipArchive.dep")
!INCLUDE "ZipArchive.dep"
!ELSE 
!MESSAGE Warning: cannot find "ZipArchive.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ZipArchive - Win32 Release" || "$(CFG)" == "ZipArchive - Win32 Debug" || "$(CFG)" == "ZipArchive - Win32 Static Release" || "$(CFG)" == "ZipArchive - Win32 Unicode Debug" || "$(CFG)" == "ZipArchive - Win32 Unicode Release" || "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"
SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Fp"$(INTDIR)\ZipArchive.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ZipArchive.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\ZipArchive.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ZipArchive.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Fp"$(INTDIR)\ZipArchive.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ZipArchive.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W4 /Gm /GX /ZI /Od /D "ZLIB_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_UNICODE" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\ZipArchive.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ZipArchive.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /D "ZLIB_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_UNICODE" /Fp"$(INTDIR)\ZipArchive.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ZipArchive.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /D "_UNICODE" /Fp"$(INTDIR)\ZipArchive.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ZipArchive.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ZipArchive.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"


"$(INTDIR)\ZipArchive.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"


"$(INTDIR)\ZipArchive.obj"	"$(INTDIR)\ZipArchive.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"


"$(INTDIR)\ZipArchive.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"


"$(INTDIR)\ZipArchive.obj"	"$(INTDIR)\ZipArchive.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"


"$(INTDIR)\ZipArchive.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"


"$(INTDIR)\ZipArchive.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ENDIF 

SOURCE=.\ZipAutoBuffer.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"


"$(INTDIR)\ZipAutoBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"


"$(INTDIR)\ZipAutoBuffer.obj"	"$(INTDIR)\ZipAutoBuffer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"


"$(INTDIR)\ZipAutoBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"


"$(INTDIR)\ZipAutoBuffer.obj"	"$(INTDIR)\ZipAutoBuffer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"


"$(INTDIR)\ZipAutoBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"


"$(INTDIR)\ZipAutoBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ENDIF 

SOURCE=.\ZipBigFile.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"


"$(INTDIR)\ZipBigFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"


"$(INTDIR)\ZipBigFile.obj"	"$(INTDIR)\ZipBigFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"


"$(INTDIR)\ZipBigFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"


"$(INTDIR)\ZipBigFile.obj"	"$(INTDIR)\ZipBigFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"


"$(INTDIR)\ZipBigFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"


"$(INTDIR)\ZipBigFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ENDIF 

SOURCE=.\ZipCentralDir.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"


"$(INTDIR)\ZipCentralDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"


"$(INTDIR)\ZipCentralDir.obj"	"$(INTDIR)\ZipCentralDir.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"


"$(INTDIR)\ZipCentralDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"


"$(INTDIR)\ZipCentralDir.obj"	"$(INTDIR)\ZipCentralDir.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"


"$(INTDIR)\ZipCentralDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"


"$(INTDIR)\ZipCentralDir.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ENDIF 

SOURCE=.\ZipException.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"


"$(INTDIR)\ZipException.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"


"$(INTDIR)\ZipException.obj"	"$(INTDIR)\ZipException.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"


"$(INTDIR)\ZipException.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"


"$(INTDIR)\ZipException.obj"	"$(INTDIR)\ZipException.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"


"$(INTDIR)\ZipException.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"


"$(INTDIR)\ZipException.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ENDIF 

SOURCE=.\ZipFileHeader.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"


"$(INTDIR)\ZipFileHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"


"$(INTDIR)\ZipFileHeader.obj"	"$(INTDIR)\ZipFileHeader.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"


"$(INTDIR)\ZipFileHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"


"$(INTDIR)\ZipFileHeader.obj"	"$(INTDIR)\ZipFileHeader.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"


"$(INTDIR)\ZipFileHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"


"$(INTDIR)\ZipFileHeader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ENDIF 

SOURCE=.\ZipInternalInfo.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"


"$(INTDIR)\ZipInternalInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"


"$(INTDIR)\ZipInternalInfo.obj"	"$(INTDIR)\ZipInternalInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"


"$(INTDIR)\ZipInternalInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"


"$(INTDIR)\ZipInternalInfo.obj"	"$(INTDIR)\ZipInternalInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"


"$(INTDIR)\ZipInternalInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"


"$(INTDIR)\ZipInternalInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ENDIF 

SOURCE=.\ZipStorage.cpp

!IF  "$(CFG)" == "ZipArchive - Win32 Release"


"$(INTDIR)\ZipStorage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Debug"


"$(INTDIR)\ZipStorage.obj"	"$(INTDIR)\ZipStorage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Static Release"


"$(INTDIR)\ZipStorage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Debug"


"$(INTDIR)\ZipStorage.obj"	"$(INTDIR)\ZipStorage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Release"


"$(INTDIR)\ZipStorage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ELSEIF  "$(CFG)" == "ZipArchive - Win32 Unicode Static Release"


"$(INTDIR)\ZipStorage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ZipArchive.pch"


!ENDIF 


!ENDIF 

