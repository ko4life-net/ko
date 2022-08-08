@REM Script for noobs that have hard time reading instructions.
@echo off

echo Configuring started...

where /q git
if ERRORLEVEL 1 (
    echo ERROR: Git command line tool is not installed. Please install it: https://git-scm.com/download/win
    pause
    exit 1
)

rmdir /s /q src\assets 2> NUL
rmdir /s /q src\vendor 2> NUL

git clone --depth=1 https://github.com/stevewgr/ko-assets src\assets
git clone --depth=1 https://github.com/stevewgr/ko-vendor src\vendor

@REM src\All.sln

echo Configuring done.

echo If you already setup the database with odbcad settings and the import.ps1 script from: https://github.com/stevewgr/ko-assets
echo You may want to to start the src\All.sln to start developing.

pause
