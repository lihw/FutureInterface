@ECHO OFF
CLS
IF "%1" == "" GOTO USAGE
rmdir /s /q %1
IF "%2" == "" GOTO NOFBX
:FBX
..\..\Tools\bin\msvc2012\makeproject.exe 
..\..\Tools\bin\msvc2012\fbxtool.exe -meshattrib pu %2
xcopy /E /Q res %1\application\res
rmdir /s /q res
GOTO EXIT
:NOFBX
..\..\Tools\bin\msvc2012\makeproject.exe 
GOTO EXIT
:USAGE
ECHO makeproject.bat ^<Project name^> [FBX file]
:EXIT

