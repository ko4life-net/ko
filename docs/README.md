# Knight Online

This is the official Knight Online repository containing sources for the game, server and tools.

This is the main project which focuses only on source development. Its corresponding submodules are fetched automatically via the `config.cmd` script and maintained under the following projects:
- [Database development](https://github.com/ko4life-net/ko-db)
- [Client assets](https://github.com/ko4life-net/ko-assets)
- [Portable SDKs and utils](https://github.com/ko4life-net/ko-vendor)



### Prerequisite

- Visual Studio 2022: The project may be built with older Visual Studio versions, however for the sake of consistency, please install Visual Studio 2022.
You will also need to install additional MSVC, MFC and ATL components if you haven't already. The way it's done in Visual Studio 2022 is by:
  - Click on `Tool` -> `Get Tools and Features...`
  - In the new window switch to `Individual components` tab
  - Search for `mfc` and select `C++ MFC for latest v143 build tools (x86 & x64)`
  - Search for `atl` and select `C++ ATL for latest v143 build tools (x86 & x64)`
  - Search for `msvc` and select `MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)`
  - Click on `Modify` to start the installation
- Microsoft SQL Server Express or Developer (confirmed to be working with 2008 and 2022)
  - SQL Server: https://www.microsoft.com/en-us/sql-server/sql-server-downloads
  - SQL Management Studio: https://learn.microsoft.com/en-us/sql/ssms/download-sql-server-management-studio-ssms
- Git: https://git-scm.com/download/win


### Getting Started

To get started, clone this repository:
```
git clone https://github.com/ko4life-net/ko.git
```
and then double click on the `config.cmd` script in the root directory.

After configuring the project is done, you can now start the Visual Studio solution (`*.sln`) file of your preference under the src directory.


### Code formatting / Linting

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
