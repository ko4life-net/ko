# Knight Online

This is the official Knight Online repository containing sources for the game, server and tools.

To get started, clone this repo:
```
git clone https://github.com/ko4life-net/ko.git
```
and then double click to the `config.cmd` script in the root directory to fetch external dependencies.

The project may be built with older Visual Studio versions, however for the sake of consistency, please install Visual Studio 2022.
You will also need to install additional MSVC, MFC and ATL components if you haven't already. The way it's done in Visual Studio 2022 is by:
- Click on `Tool` -> `Get Tools and Features...`
- In the new window switch to `Individual components` tab
- Search for `mfc` and select `C++ MFC for latest v143 build tools (x86 & x64)`
- Search for `atl` and select `C++ ATL for latest v143 build tools (x86 & x64)`
- Search for `msvc` and select `MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)`
- Click on `Modify` to start the installation

Note that the entire project has been updated to DirectX-9. There are some runtime dlls that are needed in your system. That being said, if you don't have DirectX-9 already installed in your system and you getting error while trying to open either the game or tools similar to this: `d3dx9d_43.dll is missing`, then you need to install the redistributable runtime dlls. I zipped it from the SDK in case you having troubles: `DX9_Redist.zip` and you can download it from the Releases in GitHub. Unzip and simply run the `SETUP.exe`.

## Setting up the database

The database project is built via SQL scripts. The repository where the database is hosted can be found here: https://github.com/ko4life-net/ko-db

In order to setup the database, run the `import.ps1` powershell script by right clicking it and `Run with Powershell`.

Note that if you haven't installed MSSQL with default instance, you may want to adjust the variable `$server_name` in the import script as your instance server name.

The import should run and setup the login and username, as well as permissions for `db_owner`:
- DSN: `kodb`
- UID: `kodb_user`
- PWD: `kodb_user`

Lastly you'll need to setup odbcad settings so that the server can access the database with the odbcad driver.
