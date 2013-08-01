@echo off
del glc*.exe
cls
cl /DNO_SLEEP /nologo /Ox /MD /Zp1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /W3 src\main.cpp src\glclock.cpp src\GLextensions.cpp src\GLObject.cpp src\MString.cpp src\PnmIO.cpp user32.lib gdi32.lib /link /out:glclock_win.exe
cl /DNO_SLEEP /nologo /Ox /MD /Zp1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /W3 src\glclockmark.cpp src\GLextensions.cpp src\GLObject.cpp src\MString.cpp src\PnmIO.cpp user32.lib gdi32.lib shell32.lib /link /out:glclockm_win.exe
del *.obj
del *.err