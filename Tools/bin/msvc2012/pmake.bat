@ECHO OFF
CLS
IF "%1" == "" GOTO USAGE
rmdir /s /q %1
IF "%2" == "" GOTO NOFBX
:FBX
pmake.exe 
fbxtool.exe -meshattrib pu %2
xcopy /E /Q res %1\application\res
rmdir /s /q res
GOTO EXIT
:NOFBX
pmake.exe 
GOTO EXIT
:USAGE
ECHO pmake.bat ^<Project name^> [FBX file]
:EXIT

