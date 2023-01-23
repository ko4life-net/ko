$server = "localhost"
$database = "kodb"

$drivers = Get-OdbcDriver | Where-Object { $_.Name -like "*SQL Server*" -and $_.Platform -eq "32-bit" }
if (!$drivers) {
  Write-Output "Are you sure SQL Server is installed? I couldn't find any drivers."
  exit(1)
}

$selected_driver = $null
if ($drivers.Count -eq 1) {
  $selected_driver = $drivers[0]
} else {
  while (!$selected_driver) {
    Write-Output "Select the desired SQL Driver:"
    for ($i = 0; $i -lt $drivers.Count; $i++) {
      Write-Output "$($i+1). $($drivers[$i].Name)"
    }

    $user_input = -1
    $input_valid = [int]::TryParse((Read-Host "Enter the number of the driver you want to select"), [ref]$user_input)
    if (-not $input_valid -or $user_input -lt 1 -or $user_input -gt $drivers.Count) {
      Write-Output "Invalid selection.`n"
    } else {
      $selected_driver = $drivers[$user_input - 1]
      Write-Output "Selected SQL Driver: $($selected_driver.Name)`n"
      break
    }
  }
}

Remove-OdbcDsn -Name $database -DsnType "User" -ErrorAction Ignore
Add-OdbcDsn -Name $database -DriverName $selected_driver.Name -DsnType "User" -SetPropertyValue @("Server=$server", "Database=$database", "AutoTranslate=No", "Trusted_Connection=Yes")

# Test connection
$con = New-Object System.Data.Odbc.OdbcConnection
$con.ConnectionString = "DSN=$database"
$con.Open()
$is_successful = $con.State -eq "Open"
$con.Close()
if ($is_successful) {
  Write-Output "Successfully created odbcad connection driver and tested connection!"
} else {
  Write-Output "Failed to test connection. Check that you first imported the database."
  Write-Output "If that didn't work, depending on how you installed MSSQL (Default or Named Instance), you may need to change the server above from localhost to yours."
  exit(1)
}
