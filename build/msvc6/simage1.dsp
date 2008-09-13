# Microsoft Developer Studio Project File - Name="simage1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=simage1 - Win32 DLL (Release)
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "simage1.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "simage1.mak" CFG="simage1 - Win32 DLL (Debug)"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "simage1 - Win32 LIB (Release)" (based on "Win32 (x86) Static Library")
!MESSAGE "simage1 - Win32 LIB (Debug)" (based on "Win32 (x86) Static Library")
!MESSAGE "simage1 - Win32 DLL (Release)" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "simage1 - Win32 DLL (Debug)" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "simage1 - Win32 LIB (Release)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "StaticRelease"
# PROP BASE Intermediate_Dir "StaticRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "StaticRelease"
# PROP Intermediate_Dir "StaticRelease"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Ox /Gy /Zi /I "." /I "include" /I "..\..\include" /I "src" /I "..\..\src" /I "..\misc\GDIPlus\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_LIB" /D SIMAGE_DEBUG=0  /D "HAVE_CONFIG_H" /D "YY_NO_UNISTD_H" /D "SIMAGE_INTERNAL" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ox /Gy /Zi /I "." /I "include" /I "..\..\include" /I "src" /I "..\..\src" /I "..\misc\GDIPlus\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_LIB" /D SIMAGE_DEBUG=0  /D "HAVE_CONFIG_H" /D "YY_NO_UNISTD_H" /D "SIMAGE_INTERNAL" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x414 /d "NDEBUG"
# ADD RSC /l 0x414 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /machine:I386 /out:"simage1s.lib"
# ADD LIB32 /nologo /machine:I386 /out:"simage1s.lib"

!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "StaticDebug"
# PROP BASE Intermediate_Dir "StaticDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "StaticDebug"
# PROP Intermediate_Dir "StaticDebug"
# PROP Target_Dir ""
MTL=midl.exe
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /GZ /Od /Zi /I "." /I "include" /I "..\..\include" /I "src" /I "..\..\src" /I "..\misc\GDIPlus\Includes" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LIB" /D SIMAGE_DEBUG=1  /D "HAVE_CONFIG_H" /D "YY_NO_UNISTD_H" /D "SIMAGE_INTERNAL" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /GZ /Od /Zi /I "." /I "include" /I "..\..\include" /I "src" /I "..\..\src" /I "..\misc\GDIPlus\Includes" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LIB" /D SIMAGE_DEBUG=1  /D "HAVE_CONFIG_H" /D "YY_NO_UNISTD_H" /D "SIMAGE_INTERNAL" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x414 /d "_DEBUG"
# ADD RSC /l 0x414 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /machine:I386 /out:"simage1sd.lib"
# ADD LIB32 /nologo /machine:I386 /out:"simage1sd.lib"

!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Release)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Ox /Gy /Zi /I "." /I "include" /I "..\..\include" /I "src" /I "..\..\src" /I "..\misc\GDIPlus\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D SIMAGE_DEBUG=0 /D "HAVE_CONFIG_H" /D "SIMAGE_MAKE_DLL" /D "YY_NO_UNISTD_H" /D "SIMAGE_INTERNAL" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ox /Gy /Zi /I "." /I "include" /I "..\..\include" /I "src" /I "..\..\src" /I "..\misc\GDIPlus\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D SIMAGE_DEBUG=0 /D "HAVE_CONFIG_H" /D "SIMAGE_MAKE_DLL" /D "YY_NO_UNISTD_H" /D "SIMAGE_INTERNAL" /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RCS=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\misc\GDIPlus\Lib\GdiPlus.lib gdi32.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\misc\GDIPlus\Lib\GdiPlus.lib gdi32.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /out:"simage1.dll" /opt:nowin98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /GZ /Zi /Od /I "." /I "include" /I "..\..\include" /I "src" /I "..\..\src" /I "..\misc\GDIPlus\Includes" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D SIMAGE_DEBUG=1 /D "HAVE_CONFIG_H" /D "SIMAGE_MAKE_DLL" /D "YY_NO_UNISTD_H" /D "SIMAGE_INTERNAL" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /GZ /Zi /Od /I "." /I "include" /I "..\..\include" /I "src" /I "..\..\src" /I "..\misc\GDIPlus\Includes" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D SIMAGE_DEBUG=1 /D "HAVE_CONFIG_H" /D "SIMAGE_MAKE_DLL" /D "YY_NO_UNISTD_H" /D "SIMAGE_INTERNAL" /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RCS=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\misc\GDIPlus\Lib\GdiPlus.lib gdi32.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\misc\GDIPlus\Lib\GdiPlus.lib gdi32.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /out:"simage1d.dll" /opt:nowin98

!ENDIF

# Begin Target

# Name "simage1 - Win32 DLL (Release)"
# Name "simage1 - Win32 DLL (Debug)"
# Name "simage1 - Win32 LIB (Release)"
# Name "simage1 - Win32 LIB (Debug)"
# Begin Group "Documents"
# PROP Default_Filter ";txt"
# Begin Source File

SOURCE=..\..\README
# End Source File
# Begin Source File

SOURCE=..\..\README.WIN32
# End Source File
# Begin Source File

SOURCE=..\..\NEWS
# End Source File
# Begin Source File

SOURCE=..\..\COPYING
# End Source File
# Begin Source File

SOURCE=..\..\ChangeLog
# End Source File
# End Group
# Begin Group "Source Files"
# PROP Default_Filter "c;cpp;ic;icc;h"
# Begin Source File

SOURCE=..\..\src\avi_encode.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\movie.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\params.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\resize.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage12.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage13.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_avi.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_eps.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_gdiplus.cpp
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_gif.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_jasper.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_jpeg.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_jpeg_writer.icc

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_jpeg_reader.icc

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_libsndfile.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_oggvorbis_reader.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_pic.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_png.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_qimage.cpp
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_quicktime.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_rgb.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_tga.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_tiff.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_write.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\simage_xwd.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# Begin Source File

SOURCE=..\..\src\stream.c
!IF  "$(CFG)" == "simage1 - Win32 DLL (Release)"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 DLL (Debug)"
# PROP Intermediate_Dir "Debug\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Release)"
# PROP Intermediate_Dir "StaticRelease\src"
!ELSEIF  "$(CFG)" == "simage1 - Win32 LIB (Debug)"
# PROP Intermediate_Dir "StaticDebug\src"
!ENDIF
# End Source File
# End Group
# Begin Group "Public Headers"
# PROP Default_Filter "h;ic;icc"
# Set Default_Filter "h"
# Begin Source File

SOURCE=.\include\simage.h

# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Private Headers"

# PROP Default_Filter "h;ic;icc"
# Begin Source File

SOURCE=.\config.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\config-debug.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\config-release.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\avi_encode.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_avi.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_eps.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_gdiplus.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_gif.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_jasper.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_jpeg.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_libsndfile.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_oggvorbis.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_pic.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_png.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_private.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_qimage.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_quicktime.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_rgb.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_tga.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_tiff.h

# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\include\simage_xwd.h

# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
