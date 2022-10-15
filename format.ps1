# Runs clang-format in parallel on the entire project
$vendor_dir = "$PSScriptRoot/src/vendor"
$clang_format = "$vendor_dir/opt/bin/clang-format.exe"

if (-Not (Test-Path -Path $vendor_dir,$clang_format)) {
  Write-Host "Directory [$vendor_dir] or file [$clang_format] not exists. Please run the config.cmd script."
  exit(1)
}

foreach ($fn In Get-ChildItem -Path $PSScriptRoot/src/common,$PSScriptRoot/src/engine,$PSScriptRoot/src/game,$PSScriptRoot/src/server,$PSScriptRoot/src/tool -Include *.cpp,*.c,*.hpp,*.h -Recurse) {
  Write-Host "clang-format: [$($fn.FullName)]"
  # & $clang_format -style=file -i $fn.FullName --verbose > $null
  $pi = New-Object System.Diagnostics.ProcessStartInfo
  $pi.FileName = $clang_format
  $pi.UseShellExecute = $false
  $pi.RedirectStandardError = $true
  $pi.RedirectStandardOutput = $true
  $pi.Arguments = "-style=file -i $($fn.FullName)"
  $pr = New-Object System.Diagnostics.Process
  $pr.StartInfo = $pi
  $pr.Start() > $null
}

exit(0)
