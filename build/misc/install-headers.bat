rem ************************************************************************
rem * install-headers.bat
rem *

set msvc=%1
copy /Y ..\%msvc%\include\simage.h %COINDIR%\include\simage.h >nul:

