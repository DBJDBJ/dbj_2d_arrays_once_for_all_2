@echo off
@cls
@setlocal

@rem Setup VS2019
@rem change to your local path, if need be
call "D:\PROD\programs\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

@set "INCLUDE=D:\machine_wide;%INCLUDE%"

@if [%1] == [clean] goto clean

:debug
clang-cl /TC /MTd /Zi matreix.c main.c /o out/mx2d.exe
@goto exit

:release
clang-cl /TC /MT /O2 matreix.c main.c /o out/mx2d.exe
@goto exit


:clean
@del out\*.pdb
@del out\*.ilk
@del out\*.exe
@goto exit

:exit
@endlocal
