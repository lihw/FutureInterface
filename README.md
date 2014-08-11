FutureInterface
===============

#Products
- Tech: the fundamental libraries for all engines
- Paper3D: a light-weighted rendering engine for live wallpaper and casual games
- Panel3D: a light-weighted 3D UI engine that can achive photowalls, cover-flow and other 3D layouts of panel-like objects easily.

# Installation
## Windows
Clone the repo of FutureInterface using `git clone https://github.com/lihw/FutureInterface.git <Dir>` or download the zip file at [https://github.com/lihw/FutureInterface/archive/master.zip](https://github.com/lihw/FutureInterface/archive/master.zip), and then uncompress the zip file to any dir you want.
**The path of installation dir should not include and blank spaces**

### Installation of 3rd-party packages
1. Download and install Microsoft Visual Studio 2012 Express
[http://www.microsoft.com/en-us/download/details.aspx?id=34673](http://www.microsoft.com/en-us/download/details.aspx?id=34673)
2. Install the Android SDK and Eclipse bundle 
32bit: [http://dl.google.com/android/adt/adt-bundle-windows-x86-20131030.zip](http://dl.google.com/android/adt/adt-bundle-windows-x86-20131030.zip)
64bit: TODO
3. Install ANT
[http://ant.apache.org/bindownload.cgi?Preferred=http%3A%2F%2Fmirrors.cnnic.cn%2Fapache%2F](http://ant.apache.org/bindownload.cgi?Preferred=http%3A%2F%2Fmirrors.cnnic.cn%2Fapache%2F)
4. Install JDK 1.7
[http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)
**Needs to be installed to C:\JDK**
5. Install the latest Android NDK
[https://developer.android.com/tools/sdk/ndk/index.html#Installing](https://developer.android.com/tools/sdk/ndk/index.html#Installing)
6. Setup environment variables
JAVA_HOME=C:\JDK
ANT_HOME=`<ANT installation dir>`
ANDROID_SDK=`<Android SDK path>`
Add `<ANT installation dir>\bin;<Android NDK installation dir>\;<Android SDK installation dir>\platform-tools\` to PATH
7. Run install.bat **as administrator**

### Build first example
1. Open MSVC 2012 and open Paper3D/examples/hellopaper3d/configs/msvc2012/hello3d.sln
2. Compile and run
3. One should see following rendering window

![First Paper3D application](http://imglf0.ph.126.net/ecpTDv4uPz3_1XebsU04Sg==/3002775051649722510.png)

4. Build the Android live wallpaper APK
5. Open cmd.exe (the dos window)
6. cd Paper3D/examples/hellopaper3d/application/configs/android/
7. Type build.bat debug and enter
8. If successful, the APK will be at Paper3D/examples/hellopaper3d/application/bin, named hellopaper3d-debug.apk
9. Install the apk into your phone by entering adb install -r hellpaper3d-debug.apk

Congrats! You have built your first live wallpaper. For any questions, please drop an email to lihw81[@]gmail.com or file an issue to the git issue page. Thanks a lot.

# Start a new project
## Android live wallpaper

## Android application

