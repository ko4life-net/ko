# Knight Online

This is the official Knight Online repository containing sources for the game, server and tools.

Before opening any of the projects, make sure you clone the vendor directory:
```
git clone --depth=1 https://github.com/stevewgr/ko-vendor.git src\vendor
```

It contains portable SDKs to build the project without having to install additional tools.

The only software you need to install for now is Visual Studio 2022.

Note that if you only want to build the project completely, without having to install anything (SDKs, VS, git etc.),
simply double click the `build.cmd` script in the root directory. All artifacts can be then found under the `build` directory.

Note that the entire project has been updated to DirectX-9. There are some runtime dlls that are needed in your system. That being said, if you don't have DirectX-9 already installed in your system and you getting error while trying to open either the game or tools similar to this: `d3dx9d_43.dll is missing`, then you need to install the redistributable runtime dlls. I zipped it from the SDK in case you having troubles: `DX9_Redist.zip` and you can download it from the Releases in GitHub. Unzip and simply run the `SETUP.exe`.
