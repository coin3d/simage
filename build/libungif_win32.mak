# Microsoft Developer Studio Generated NMAKE File, Based on libungif.dsp
!IF "$(CFG)" == ""
CFG=libungif - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libungif - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libungif - Win32 Release" && "$(CFG)" != "libungif - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libungif.mak" CFG="libungif - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libungif - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libungif - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libungif - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\libungif.lib"


CLEAN :
	-@erase "$(INTDIR)\dev2gif.obj"
	-@erase "$(INTDIR)\dgif_lib.obj"
	-@erase "$(INTDIR)\egif_lib.obj"
	-@erase "$(INTDIR)\getarg.obj"
	-@erase "$(INTDIR)\gif_err.obj"
	-@erase "$(INTDIR)\gif_font.obj"
	-@erase "$(INTDIR)\gifalloc.obj"
	-@erase "$(INTDIR)\qprintf.obj"
	-@erase "$(INTDIR)\quantize.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\libungif.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libungif.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libungif.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"libungif.lib" 
LIB32_OBJS= \
	"$(INTDIR)\dev2gif.obj" \
	"$(INTDIR)\dgif_lib.obj" \
	"$(INTDIR)\egif_lib.obj" \
	"$(INTDIR)\getarg.obj" \
	"$(INTDIR)\gif_err.obj" \
	"$(INTDIR)\gif_font.obj" \
	"$(INTDIR)\gifalloc.obj" \
	"$(INTDIR)\qprintf.obj" \
	"$(INTDIR)\quantize.obj"

".\libungif.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libungif - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\libungif.lib"


CLEAN :
	-@erase "$(INTDIR)\dev2gif.obj"
	-@erase "$(INTDIR)\dgif_lib.obj"
	-@erase "$(INTDIR)\egif_lib.obj"
	-@erase "$(INTDIR)\getarg.obj"
	-@erase "$(INTDIR)\gif_err.obj"
	-@erase "$(INTDIR)\gif_font.obj"
	-@erase "$(INTDIR)\gifalloc.obj"
	-@erase "$(INTDIR)\qprintf.obj"
	-@erase "$(INTDIR)\quantize.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\libungif.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libungif.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libungif.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"libungif.lib" 
LIB32_OBJS= \
	"$(INTDIR)\dev2gif.obj" \
	"$(INTDIR)\dgif_lib.obj" \
	"$(INTDIR)\egif_lib.obj" \
	"$(INTDIR)\getarg.obj" \
	"$(INTDIR)\gif_err.obj" \
	"$(INTDIR)\gif_font.obj" \
	"$(INTDIR)\gifalloc.obj" \
	"$(INTDIR)\qprintf.obj" \
	"$(INTDIR)\quantize.obj"

".\libungif.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libungif.dep")
!INCLUDE "libungif.dep"
!ELSE 
!MESSAGE Warning: cannot find "libungif.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libungif - Win32 Release" || "$(CFG)" == "libungif - Win32 Debug"
SOURCE=.\lib\dev2gif.c

"$(INTDIR)\dev2gif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\dgif_lib.c

"$(INTDIR)\dgif_lib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\egif_lib.c

"$(INTDIR)\egif_lib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\getarg.c

"$(INTDIR)\getarg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\gif_err.c

"$(INTDIR)\gif_err.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\gif_font.c

"$(INTDIR)\gif_font.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\gifalloc.c

"$(INTDIR)\gifalloc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\qprintf.c

"$(INTDIR)\qprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\quantize.c

"$(INTDIR)\quantize.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

