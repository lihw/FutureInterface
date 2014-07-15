@ECHO OFF
CLS
IF "%1" == "" GOTO USAGE
IF "%1" == "debug" GOTO DEBUG
IF "%1" == "release" GOTO RELEASE
:DEBUG
ECHO ********************************************
ECHO Start building .so in debug version
ECHO ********************************************
ECHO ndk-build USE_STL=0 ENABLE_LOGGING=1 OPENGL=ES2 NDK_DEBUG=1 > temp.bat
CALL temp.bat
ECHO ********************************************
ECHO Start building Android APK in debug version
ECHO ********************************************
ant debug
:RELEASE
ECHO ********************************************
ECHO Start building .so in release version
ECHO ********************************************
ECHO ndk-build USE_STL=0 ENABLE_LOGGING=1 OPENGL=ES2 NDK_DEBUG=0 -j4 > temp.bat
CALL temp.bat
ECHO ********************************************
ECHO Start building Android APK in release version
ECHO ********************************************
ant release
:USAGE
ECHO Usage: build.bat {debug ^| release}
:EXIT

