# Microsoft Developer Studio Generated NMAKE File, Based on N3Base.dsp
!IF "$(CFG)" == ""
CFG=N3Base - Win32 Debug
!MESSAGE No configuration specified. Defaulting to N3Base - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "N3Base - Win32 Release" && "$(CFG)" != "N3Base - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3Base.mak" CFG="N3Base - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3Base - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "N3Base - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "N3Base - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\N3Base.lib"


CLEAN :
	-@erase "$(INTDIR)\N3AnimControl.obj"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Chr.obj"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3Joint.obj"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3Moon.obj"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3River2.obj"
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\N3Base.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\N3Base.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3Base.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\N3Base.lib" 
LIB32_OBJS= \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3River2.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Pick.obj"

"$(OUTDIR)\N3Base.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\N3Base.lib" "$(OUTDIR)\N3Base.bsc"


CLEAN :
	-@erase "$(INTDIR)\N3AnimControl.obj"
	-@erase "$(INTDIR)\N3AnimControl.sbr"
	-@erase "$(INTDIR)\N3AnimKey.obj"
	-@erase "$(INTDIR)\N3AnimKey.sbr"
	-@erase "$(INTDIR)\N3Base.obj"
	-@erase "$(INTDIR)\N3Base.sbr"
	-@erase "$(INTDIR)\N3Camera.obj"
	-@erase "$(INTDIR)\N3Camera.sbr"
	-@erase "$(INTDIR)\N3Chr.obj"
	-@erase "$(INTDIR)\N3Chr.sbr"
	-@erase "$(INTDIR)\N3Eng.obj"
	-@erase "$(INTDIR)\N3Eng.sbr"
	-@erase "$(INTDIR)\N3IMesh.obj"
	-@erase "$(INTDIR)\N3IMesh.sbr"
	-@erase "$(INTDIR)\N3Joint.obj"
	-@erase "$(INTDIR)\N3Joint.sbr"
	-@erase "$(INTDIR)\N3Light.obj"
	-@erase "$(INTDIR)\N3Light.sbr"
	-@erase "$(INTDIR)\N3Mesh.obj"
	-@erase "$(INTDIR)\N3Mesh.sbr"
	-@erase "$(INTDIR)\N3Moon.obj"
	-@erase "$(INTDIR)\N3Moon.sbr"
	-@erase "$(INTDIR)\N3PMesh.obj"
	-@erase "$(INTDIR)\N3PMesh.sbr"
	-@erase "$(INTDIR)\N3PMeshCreate.obj"
	-@erase "$(INTDIR)\N3PMeshCreate.sbr"
	-@erase "$(INTDIR)\N3PMeshInstance.obj"
	-@erase "$(INTDIR)\N3PMeshInstance.sbr"
	-@erase "$(INTDIR)\N3River2.obj"
	-@erase "$(INTDIR)\N3River2.sbr"
	-@erase "$(INTDIR)\N3Scene.obj"
	-@erase "$(INTDIR)\N3Scene.sbr"
	-@erase "$(INTDIR)\N3Shape.obj"
	-@erase "$(INTDIR)\N3Shape.sbr"
	-@erase "$(INTDIR)\N3Skin.obj"
	-@erase "$(INTDIR)\N3Skin.sbr"
	-@erase "$(INTDIR)\N3Texture.obj"
	-@erase "$(INTDIR)\N3Texture.sbr"
	-@erase "$(INTDIR)\N3Transform.obj"
	-@erase "$(INTDIR)\N3Transform.sbr"
	-@erase "$(INTDIR)\N3TransformCollision.obj"
	-@erase "$(INTDIR)\N3TransformCollision.sbr"
	-@erase "$(INTDIR)\N3VMesh.obj"
	-@erase "$(INTDIR)\N3VMesh.sbr"
	-@erase "$(INTDIR)\Pick.obj"
	-@erase "$(INTDIR)\Pick.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\N3Base.bsc"
	-@erase "$(OUTDIR)\N3Base.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\N3Base.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\N3Base.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\N3AnimControl.sbr" \
	"$(INTDIR)\N3AnimKey.sbr" \
	"$(INTDIR)\N3Base.sbr" \
	"$(INTDIR)\N3Camera.sbr" \
	"$(INTDIR)\N3Chr.sbr" \
	"$(INTDIR)\N3Eng.sbr" \
	"$(INTDIR)\N3IMesh.sbr" \
	"$(INTDIR)\N3Joint.sbr" \
	"$(INTDIR)\N3Light.sbr" \
	"$(INTDIR)\N3Mesh.sbr" \
	"$(INTDIR)\N3Moon.sbr" \
	"$(INTDIR)\N3PMesh.sbr" \
	"$(INTDIR)\N3PMeshCreate.sbr" \
	"$(INTDIR)\N3PMeshInstance.sbr" \
	"$(INTDIR)\N3River2.sbr" \
	"$(INTDIR)\N3Scene.sbr" \
	"$(INTDIR)\N3Shape.sbr" \
	"$(INTDIR)\N3Skin.sbr" \
	"$(INTDIR)\N3Texture.sbr" \
	"$(INTDIR)\N3Transform.sbr" \
	"$(INTDIR)\N3TransformCollision.sbr" \
	"$(INTDIR)\N3VMesh.sbr" \
	"$(INTDIR)\Pick.sbr"

"$(OUTDIR)\N3Base.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\N3Base.lib" 
LIB32_OBJS= \
	"$(INTDIR)\N3AnimControl.obj" \
	"$(INTDIR)\N3AnimKey.obj" \
	"$(INTDIR)\N3Base.obj" \
	"$(INTDIR)\N3Camera.obj" \
	"$(INTDIR)\N3Chr.obj" \
	"$(INTDIR)\N3Eng.obj" \
	"$(INTDIR)\N3IMesh.obj" \
	"$(INTDIR)\N3Joint.obj" \
	"$(INTDIR)\N3Light.obj" \
	"$(INTDIR)\N3Mesh.obj" \
	"$(INTDIR)\N3Moon.obj" \
	"$(INTDIR)\N3PMesh.obj" \
	"$(INTDIR)\N3PMeshCreate.obj" \
	"$(INTDIR)\N3PMeshInstance.obj" \
	"$(INTDIR)\N3River2.obj" \
	"$(INTDIR)\N3Scene.obj" \
	"$(INTDIR)\N3Shape.obj" \
	"$(INTDIR)\N3Skin.obj" \
	"$(INTDIR)\N3Texture.obj" \
	"$(INTDIR)\N3Transform.obj" \
	"$(INTDIR)\N3TransformCollision.obj" \
	"$(INTDIR)\N3VMesh.obj" \
	"$(INTDIR)\Pick.obj"

"$(OUTDIR)\N3Base.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("N3Base.dep")
!INCLUDE "N3Base.dep"
!ELSE 
!MESSAGE Warning: cannot find "N3Base.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "N3Base - Win32 Release" || "$(CFG)" == "N3Base - Win32 Debug"
SOURCE=.\N3AnimControl.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3AnimControl.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3AnimControl.obj"	"$(INTDIR)\N3AnimControl.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3AnimKey.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3AnimKey.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3AnimKey.obj"	"$(INTDIR)\N3AnimKey.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Base.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Base.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Base.obj"	"$(INTDIR)\N3Base.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Camera.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Camera.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Camera.obj"	"$(INTDIR)\N3Camera.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Chr.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Chr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Chr.obj"	"$(INTDIR)\N3Chr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Eng.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Eng.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Eng.obj"	"$(INTDIR)\N3Eng.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3IMesh.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3IMesh.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3IMesh.obj"	"$(INTDIR)\N3IMesh.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Joint.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Joint.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Joint.obj"	"$(INTDIR)\N3Joint.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Light.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Light.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Light.obj"	"$(INTDIR)\N3Light.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Mesh.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Mesh.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Mesh.obj"	"$(INTDIR)\N3Mesh.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Moon.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Moon.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Moon.obj"	"$(INTDIR)\N3Moon.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3PMesh.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3PMesh.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3PMesh.obj"	"$(INTDIR)\N3PMesh.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3PMeshCreate.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3PMeshCreate.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3PMeshCreate.obj"	"$(INTDIR)\N3PMeshCreate.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3PMeshInstance.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3PMeshInstance.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3PMeshInstance.obj"	"$(INTDIR)\N3PMeshInstance.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3River2.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3River2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3River2.obj"	"$(INTDIR)\N3River2.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Scene.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Scene.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Scene.obj"	"$(INTDIR)\N3Scene.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Shape.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Shape.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Shape.obj"	"$(INTDIR)\N3Shape.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Skin.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Skin.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Skin.obj"	"$(INTDIR)\N3Skin.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Texture.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Texture.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Texture.obj"	"$(INTDIR)\N3Texture.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3Transform.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3Transform.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3Transform.obj"	"$(INTDIR)\N3Transform.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3TransformCollision.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3TransformCollision.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3TransformCollision.obj"	"$(INTDIR)\N3TransformCollision.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\N3VMesh.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\N3VMesh.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\N3VMesh.obj"	"$(INTDIR)\N3VMesh.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Pick.cpp

!IF  "$(CFG)" == "N3Base - Win32 Release"


"$(INTDIR)\Pick.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"


"$(INTDIR)\Pick.obj"	"$(INTDIR)\Pick.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

