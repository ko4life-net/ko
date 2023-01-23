Write-Output "Configuring started..."
$root_dir = Split-Path $MyInvocation.MyCommand.Path

# This makes M$ Invoke-WebRequest and Expand-Archive work 4 times faster o.O
$global:ProgressPreference = "SilentlyContinue"

$packages_dir = "$env:APPDATA\ko4life"
if (-not (Get-Command git -ErrorAction Ignore)) {
  if (-not (Test-Path "$packages_dir\git")) {
    Write-Output "INFO: Git not found. Installing portable version at: $packages_dir"
    $tmp_dir = (New-TemporaryFile | ForEach-Object{ Remove-Item $_; mkdir $_ }).FullName
    Invoke-WebRequest `
      "https://github.com/git-for-windows/git/releases/download/v2.39.1.windows.1/MinGit-2.39.1-busybox-32-bit.zip" `
      -OutFile "$tmp_dir\PortableGit.zip"
    Remove-Item "$packages_dir\git" -Recurse -Force -ErrorAction Ignore
    Expand-Archive "$tmp_dir\PortableGit.zip" -DestinationPath "$packages_dir\git" -Force
    $env:Path = "$packages_dir\git\cmd;$env:Path"
  } else {
    $env:Path = "$packages_dir\git\cmd;$env:Path"
  }
  if (-not (Get-Command git -ErrorAction Ignore)) {
    Write-Output "ERROR: Failed to install portable git..."
    pause
    exit(1)
  }
}

if (-not (Get-Command python, pip, virtualenv -ErrorAction Ignore)) {
  if (-not (Test-Path "$packages_dir\python")) {
    Write-Output "INFO: Python not found. Installng portable version at: $packages_dir"
    $tmp_dir = (New-TemporaryFile | ForEach-Object{ Remove-Item $_; mkdir $_ }).FullName
    Invoke-WebRequest `
      "https://www.python.org/ftp/python/3.11.1/python-3.11.1-embed-win32.zip" `
      -OutFile "$tmp_dir\PortablePython.zip"
    Invoke-WebRequest `
      "https://bootstrap.pypa.io/get-pip.py" `
      -OutFile "$tmp_dir\get-pip.py"

    Remove-Item "$packages_dir\python" -Recurse -Force -ErrorAction Ignore
    Expand-Archive "$tmp_dir\PortablePython.zip" -DestinationPath "$packages_dir\python" -Force
    Remove-Item "$packages_dir\python\python3*._pth" -Force -ErrorAction Ignore
    $env:Path = "$packages_dir\python;$packages_dir\python\Scripts;$env:Path"
    python "$tmp_dir\get-pip.py"
    pip install virtualenv
  } else {
    $env:Path = "$packages_dir\python;$packages_dir\python\Scripts;$env:Path"
  }
  if (-not (Get-Command python, pip, virtualenv -ErrorAction Ignore)) {
    Write-Output "ERROR: Failed to install portable python..."
    pause
    exit(1)
  }
}

Remove-Item "$root_dir\script\venv" -Recurse -Force -ErrorAction Ignore
virtualenv "$root_dir\script\venv"
& "$root_dir\script\venv\Scripts\activate.ps1"
pip install -r "$root_dir\script\requirements.txt"
pip install "$root_dir\script"

# TODO: Modular this in python code
# As well as the export.ps1 and import.ps1 scripts for db, so that it can be managed in one place
Remove-Item "$root_dir\src\assets" -Recurse -Force -ErrorAction Ignore
Remove-Item "$root_dir\src\vendor" -Recurse -Force -ErrorAction Ignore
Remove-Item "$root_dir\src\db" -Recurse -Force -ErrorAction Ignore

git clone --depth=1 https://github.com/ko4life-net/ko-assets "$root_dir\src\assets"
git clone --depth=1 https://github.com/ko4life-net/ko-vendor "$root_dir\src\vendor"
git clone https://github.com/ko4life-net/ko-db "$root_dir\src\db"

Write-Output "`n`nConfiguring successfully done.`n"
Write-Output "You may want to to start the src\All.sln Visual Studio solution to start developing."
Write-Output "`n`nThank you and happy coding!`n"

pause
