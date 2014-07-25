@ECHO OFF
@setlocal enableextensions
@cd /d "%~dp0"
CLS
echo Administrative permissions required. Detecting permissions...

net session >nul 2>&1
IF %errorLevel% == 0 (GOTO CONFIRMED) ELSE (GOTO ERROR)
:CONFIRMED
ECHO Administrative permissions confirmed.
ECHO Setting environment variables.
if DEFINED FIROOT ( echo Installation was already executed ) ELSE (
@setx /m FIROOT %CD%
@setx /m PATH "%PATH%;%CD%\Tools\bin"
ECHO "%PATH%"
ECHO Installation completed.
)
GOTO EXIT
:ERROR
ECHO Not an administrator. 
ECHO Please run this script as administrator.
:EXIT
pause>null
del /q /f null
