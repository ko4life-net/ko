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
git clone --depth=1 https://github.com/ko4life-net/ko-vendor %CURRENT_PATH%src\vendor
git clone https://github.com/ko4life-net/ko-db %CURRENT_PATH%src\db

@REM src\All.sln

echo:
echo:
echo Configuring done.
echo:
echo You may want to to start the src\All.sln Visual Studio solution to start developing.
echo:
echo If you haven't setup the database just yet, one last manual step is needed (assuming you have any version of MS-SQL server installed):
echo   1. Run the powershell script under src\db\import.ps1 to create and import the database into your SQL server.
echo     1.1. If you installed your SQL server as Named Instance, you may need to modify the server name in the script to yours.
echo   2. Setup odbcad as usual targeting kodb database name and your SQL server so that the server files can access the database.
echo     2.1. Note that you don't need to mess with the default configurations just to keep things simple.
echo:
echo:
echo Thank you and happy coding!
echo:
echo:

pause

@REM Here is registry an example of my current odbcad settings:
@REM [HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\ODBC\ODBC.INI\kodb]
@REM "Driver"="c:\\Windows\\SysWOW64\\sqlncli10.dll"
@REM "Server"="STEVEW\\SQLEXPRESS2008"
@REM "Database"="kodb"
@REM "Language"="us_english"
@REM "LastUser"="User"
@REM "Trusted_Connection"="Yes"
@REM "AutoTranslate"="No"
