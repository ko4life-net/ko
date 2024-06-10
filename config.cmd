@echo off

echo Configuring started...
set CURRENT_PATH=%~dp0

echo Allowing current user to run powershell scripts...
powershell -command "Set-ExecutionPolicy Bypass -Scope CurrentUser"

where /q git
if ERRORLEVEL 1 (
    echo ERROR: Git command line tool is not installed. Please install it: https://git-scm.com/download/win
    pause
    exit 1
)

git submodule update --init --recursive

@REM src\All.sln

echo:
echo:
echo Configuring done.
echo:
echo You may want to to start the src\All.sln Visual Studio solution to start developing.
echo:
echo If you haven't setup the database just yet, one last manual step is needed (assuming you have any version of Microsoft SQL Server installed).
echo Run the powershell script under src\db\import.ps1 to setup the database for you.
echo:
echo:
echo Thank you and happy coding!
echo:
echo:

pause
