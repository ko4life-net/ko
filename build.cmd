@echo off
@REM Note that running this script does not require any additional installations. Not even Visual Studio.

set BUILD_DIR=%cd%\build
set VENDOR_DIR=%cd%\src\vendor
set ENGINE_DIR=%cd%\src\engine

set INCLUDE_DIRS="%ENGINE_DIR%;%VENDOR_DIR%\DXSDK8\Include;%VENDOR_DIR%\VC6\VC98\INCLUDE;%VENDOR_DIR%\VC6\VC98\MFC\INCLUDE;%VENDOR_DIR%\VC6\VC98\ATL\INCLUDE"
set LIB_DIRS="%VENDOR_DIR%\DXSDK8\Lib\x86;%VENDOR_DIR%\VC6\VC98\LIB;%VENDOR_DIR%\VC6\VC98\MFC\LIB"
set COMPILER_OPTS="/DDIRECTINPUT_VERSION=0x0800"

if not exist %VENDOR_DIR% (
    curl -L -O https://github.com/stevewgr/ko-vendor/archive/refs/heads/master.zip
    tar -xvf master.zip
    del master.zip
    move ko-vendor-master src\vendor
)

rmdir /s /q build
mkdir build

pushd src
  set PATH=%VENDOR_DIR%\VC6\Common\MSDev98\Bin;%VENDOR_DIR%\VC6\VC98\BIN;%VENDOR_DIR%\VC6\Common\TOOLS;C:\ko\VC6\Common\TOOLS\WINNT;%PATH%
  echo "Building KnightOnLine..."
  pushd game
    nmake /f WarFare.mak CFG="WarFare - Win32 Release" clean > NUL 2>&1
    nmake /f WarFare.mak CFG="WarFare - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
    copy KnightOnLine.exe %BUILD_DIR%\
  popd

  pushd server
    echo "Building AIServer..."
    pushd AIServer
      nmake /f Server.mak CFG="Server - Win32 Release" clean > NUL 2>&1
      nmake /f Server.mak CFG="Server - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Release\Server.exe %BUILD_DIR%\AIServer.exe
    popd

    echo "Building Aujard..."
    pushd Aujard
      nmake /f Aujard.mak CFG="Aujard - Win32 Release" clean > NUL 2>&1
      nmake /f Aujard.mak CFG="Aujard - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Release\Aujard.exe %BUILD_DIR%\
    popd

    echo "Building Ebenezer..."
    pushd Ebenezer
      nmake /f Ebenezer.mak CFG="Ebenezer - Win32 Release" clean > NUL 2>&1
      nmake /f Ebenezer.mak CFG="Ebenezer - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Release\Ebenezer.exe %BUILD_DIR%\
    popd

    echo "Building ItemManager..."
    pushd ItemManager
      nmake /f ItemManager.mak CFG="ItemManager - Win32 Release" clean > NUL 2>&1
      nmake /f ItemManager.mak CFG="ItemManager - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Release\ItemManager.exe %BUILD_DIR%\
    popd

    echo "Building LoginServer / VersionManager..."
    pushd LogInServer
      nmake /f Zip.mak CFG="Zip - Win32 Release" clean > NUL 2>&1
      nmake /f Zip.mak CFG="Zip - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Release\Zip.lib .
      nmake /f VersionManager.mak CFG="VersionManager - Win32 Release" clean > NUL 2>&1
      nmake /f VersionManager.mak CFG="VersionManager - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Release\VersionManager.exe %BUILD_DIR%\
    popd
  popd

  pushd tool
    echo "Building KscViewer..."
    pushd KscViewer
      nmake /f KscViewer.mak CFG="KscViewer - Win32 Release" clean > NUL 2>&1
      nmake /f KscViewer.mak CFG="KscViewer - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy KscViewer.exe %BUILD_DIR%\
    popd

    echo "Building Launcher..."
    pushd Launcher
      cd ZipArchive
      nmake /f ZipArchive.mak CFG="ZipArchive - Win32 Release" clean > NUL 2>&1
      nmake /f ZipArchive.mak CFG="ZipArchive - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy ZipArchive.lib ..\Launcher\
      cd ..\Launcher
      nmake /f Launcher.mak CFG="Launcher - Win32 Release" clean > NUL 2>&1
      nmake /f Launcher.mak CFG="Launcher - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Launcher.exe %BUILD_DIR%\
    popd

    echo "Building N3CE..."
    pushd N3CE
      nmake /f N3CE.mak CFG="N3CE - Win32 Release" clean > NUL 2>&1
      nmake /f N3CE.mak CFG="N3CE - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy N3CE.exe %BUILD_DIR%\
    popd

    echo "Building N3FXE..."
    pushd N3FXE
      nmake /f N3FXE.mak CFG="N3FXE - Win32 Release" clean > NUL 2>&1
      nmake /f N3FXE.mak CFG="N3FXE - Win32 Release" all INCLUDE="%cd%;%cd%\..\..\game;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy N3FXE.exe %BUILD_DIR%\
    popd

    echo "Building N3Indoor..."
    pushd N3Indoor
      nmake /f N3Indoor.mak CFG="N3Indoor - Win32 Release" clean > NUL 2>&1
      nmake /f N3Indoor.mak CFG="N3Indoor - Win32 Release" all INCLUDE="%cd%;%cd%\..\..\game;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Release\N3Indoor.exe %BUILD_DIR%\
    popd

    echo "Building N3ME..."
    pushd N3ME
      nmake /f N3ME.mak CFG="N3ME - Win32 Release" clean > NUL 2>&1
      nmake /f N3ME.mak CFG="N3ME - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy N3ME.exe %BUILD_DIR%\
    popd

    echo "Building N3TexViewer..."
    pushd N3TexViewer
      nmake /f N3TexViewer.mak CFG="N3TexViewer - Win32 Release" clean > NUL 2>&1
      nmake /f N3TexViewer.mak CFG="N3TexViewer - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy N3TexViewer.exe %BUILD_DIR%\
    popd

    echo "Building N3Viewer..."
    pushd N3Viewer
      nmake /f N3Viewer.mak CFG="N3Viewer - Win32 Release" clean > NUL 2>&1
      nmake /f N3Viewer.mak CFG="N3Viewer - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy N3Viewer.exe %BUILD_DIR%\
    popd

    echo "Building Option..."
    pushd Option
      nmake /f Option.mak CFG="Option - Win32 Release" clean > NUL 2>&1
      nmake /f Option.mak CFG="Option - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Option.exe %BUILD_DIR%\
    popd

    echo "Building RscTables..."
    pushd RscTables
      nmake /f RscTables.mak CFG="RscTables - Win32 Release" clean > NUL 2>&1
      nmake /f RscTables.mak CFG="RscTables - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy RscTables.exe %BUILD_DIR%\
    popd

    echo "Building ServerInfoViewer..."
    pushd ServerInfoViewer
      nmake /f ServerInfoViewer.mak CFG="ServerInfoViewer - Win32 Release" clean > NUL 2>&1
      nmake /f ServerInfoViewer.mak CFG="ServerInfoViewer - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy Release\ServerInfoViewer.exe %BUILD_DIR%\
    popd

    echo "Building SkyViewer..."
    pushd SkyViewer
      nmake /f SkyViewer.mak CFG="SkyViewer - Win32 Release" clean > NUL 2>&1
      nmake /f SkyViewer.mak CFG="SkyViewer - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy SkyViewer.exe %BUILD_DIR%\
    popd

    echo "Building UIE..."
    pushd UIE
      nmake /f UIE.mak CFG="UIE - Win32 Release" clean > NUL 2>&1
      nmake /f UIE.mak CFG="UIE - Win32 Release" all INCLUDE="%cd%;%INCLUDE_DIRS%" LIB="%LIB_DIRS%" CL="%COMPILER_OPTS%"
      copy UIE.exe %BUILD_DIR%\
    popd
  popd
popd

pause
