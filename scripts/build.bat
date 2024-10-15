@echo off

set CC=cl.exe
set TUIM_HOME=C:\Program Files

if not exist ".build" mkdir ".build"

%CC% ^
   /c ^
   /DTUIM_HOME="%TUIM_HOME%" ^
   /Fo.build/libtuim.obj ^
   src-loader\libtuim.c
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

%CC% ^
   /I src-loader\include ^
   /Fe.build/tuim.exe ^
   src-cli\tuim.c .build\libtuim.obj

exit /b %ERRORLEVEL%
