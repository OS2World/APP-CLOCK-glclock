@echo off
del ogf*.exe
SET WATCOM=G:\WATCOM
SET PATH=%WATCOM%\BINNT;%WATCOM%\BINW;%PATH%
SET INCLUDE=%WATCOM%\H;%WATCOM%\H\OS2;%ZINCLUDE%
SET LIB=%WATCOM%\lib386\OS2;%WATCOM%\lib386;.;%ZLIB%
SET INCLUDE=%WATCOM%\samples\os2\os2c\openil\include;%INCLUDE%
SET EDPATH=%WATCOM%\EDDAT
cls
wcl386 -dUSE_OS2 -dNO_SLEEP -k2m -cc++ -zq -w1 -3r -bt=os2v2 -bw -ox -d0 -zp1 -bcl=os2v2_pm src\main.cpp src\glclock.cpp src\GLextensions.cpp src\GLObject.cpp src\MString.cpp src\PnmIO.cpp lib11\opengl.lib lib11\glut.lib
ren main.exe glclock_os2_gl11.exe
del *.obj
del *.err

wcl386 -dUSE_GL10 -dUSE_OS2 -dNO_FONT -dNO_SLEEP -k2m -cc++ -zq -w1 -3r -bt=os2v2 -bw -ox -d0 -zp1 -bcl=os2v2_pm src\main.cpp src\glclock.cpp src\GLextensions.cpp src\GLObject.cpp src\MString.cpp src\PnmIO.cpp lib10\opengl.lib lib10\glut.lib
ren main.exe glclock_os2_gl10.exe
del *.obj
del *.err
