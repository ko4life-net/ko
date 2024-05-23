@REM Script for noobs that have hard time reading instructions.
@echo off

echo Configuring started...
set CURRENT_PATH=%~dp0

where /q git
if ERRORLEVEL 1 (
    echo ERROR: Git command line tool is not installed. Please install it: https://git-scm.com/download/win
    pause
    exit 1
)

rmdir /s /q %CURRENT_PATH%src\assets 2> NUL
rmdir /s /q %CURRENT_PATH%src\vendor 2> NUL
rmdir /s /q %CURRENT_PATH%src\db 2> NUL

git clone --depth=1 https://github.com/ko4life-net/ko-assets %CURRENT_PATH%src\assets
git clone --depth=1 --branch=1.0.0 https://github.com/ko4life-net/ko-vendor %CURRENT_PATH%src\vendor
git clone --branch=1.1.1 https://github.com/ko4life-net/ko-db %CURRENT_PATH%src\db

@REM src\All.sln

echo:
echo:
echo Configuring done.
echo:
echo You may want to to start the src\All.sln Visual Studio solution to start developing.
echo:
echo If you haven't setup the database just yet, one last manual step is needed (assuming you have any version of Microsoft SQL Server installed):
echo   - Run the powershell script under src\db\import.ps1 to create and import the database into your SQL server.
echo:
echo:
echo Thank you and happy coding!
echo:
echo:

pause
