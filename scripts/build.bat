@echo off

set CC=cl.exe
set TUIM_HOME=C:\Program Files

if not exist ".build" mkdir ".build"

%CC% /c /TP /EHsc /nologo /W4 /WX /O2 /GL /Zc:wchar_t /Zc:forScope /Zc:inline /Zi /Gm- /Od /Ob2 /Oi /Ot /Oy- /GT /GL- /Zl /Zp8 /Gy /Zc:__cplusplus /Zc:preprocessor /Fo.build\libtuim.obj src-loader\libtuim.c

%CC% /TP /EHsc /nologo /W4 /WX /O2 /GL /Zc:wchar_t /Zc:forScope /Zc:inline /Zi /Gm- /Od /Ob2 /Oi /Ot /Oy- /GT /GL- /Zl /Zp8 /Gy /Zc:__cplusplus /Zc:preprocessor /I src-loader\include /Fe.build\tuim.exe .build\libtuim.obj src-cli\tuim.c

exit /b 0
