@echo off

set VENDOR_DIR=%cd%\src\vendor

if not exist %VENDOR_DIR% (
    curl -L -O https://github.com/stevewgr/ko-vendor/archive/refs/heads/master.zip
    tar -xvf master.zip
    del master.zip
    move ko-vendor-master %VENDOR_DIR%
)

call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat"

pushd src
  rmdir /s /q build
  msbuild All.sln -t:Clean -t:Build -p:Configuration=Release /m
popd

pause
