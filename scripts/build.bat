@echo off

set TUIM_HOME=C:\Program Files\tuim

if not exist ".build" (
   mkdir ".build"
   if %errorlevel% NEQ 0 (
      exit /b %errorlevel%
   )
)

cl.exe ^
   /c ^
   /DTUIM_HOME="%TUIM_HOME%" ^
   /Fo.build\libtuim.obj ^
   src-loader\libtuim.c
if %errorlevel% NEQ 0 (
   exit /b %errorlevel%
)

cl.exe ^
   /I src-loader\include ^
   /Fe.build\tuim.exe ^
   src-cli\tuim.c .build\libtuim.obj

exit /b %errorlevel%
