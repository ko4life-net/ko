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

# Code formatting / Linting

This project is using clang-format from LLVM project to ensure consistency in the entire codebase and to make it easier for everyone to contribute.

When a pull-request is created, automated checks will test your changes to check for possible linting issues.
Your pull-request will not be reviewed or considered to be merged if the linter checks are failing.

Therefore and to avoid further confusion, please run the `format.ps1` script before submitting your code.

Visual Studio has baked-in support for clang-format and will automatically detect the `.clang-format` file.
However it uses an older version of clang-format, which is not supporting all of the configurations we use.
You may want to configure your Visual Studio to the clang-format we have in ko-vendor. Please follow the following steps to do so:
- Go to `Tools` -> `Options` -> Search clang-format
- Tick: `Use custom clang-format.exe` and browse the path to `src/vendor/opt/bin/clang-format.exe`

Now whenever you hit CTRL + K / D keyboard shortcut, clang-format will automatically format your code as configured in this project.
