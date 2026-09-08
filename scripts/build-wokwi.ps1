param(
    [switch]$Clean
)

$ErrorActionPreference = 'Stop'
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $ProjectRoot 'build'

$CliCandidates = @()
$PathCommand = Get-Command arduino-cli -ErrorAction SilentlyContinue
if ($PathCommand) { $CliCandidates += $PathCommand.Path }
$CliCandidates += "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
$CliCandidates += "$env:LOCALAPPDATA\Arduino15\arduino-cli.exe"
$CliCandidates = @($CliCandidates | Where-Object { $_ -and (Test-Path $_) } | Select-Object -Unique)

if ($Clean -and (Test-Path $BuildDir)) {
    Remove-Item $BuildDir -Recurse -Force
}

if ($CliCandidates.Count -eq 0) {
    throw 'Arduino CLI was not found. Install Arduino IDE/Arduino CLI, then run this script again.'
}

$Cli = $CliCandidates | Select-Object -First 1
Write-Host "Using Arduino CLI: $Cli"

$coreInstalled = & $Cli core list | Select-String '^esp32:esp32\s'
if (-not $coreInstalled) {
    Write-Host 'Installing ESP32 board package...'
    & $Cli core update-index
    & $Cli core install esp32:esp32
}

Write-Host 'Installing/updating required DHT library...'
& $Cli lib install 'DHT sensor library for ESPx'

Write-Host 'Compiling firmware for Wokwi...'
& $Cli compile --fqbn esp32:esp32:esp32 --build-path $BuildDir $ProjectRoot

$Bin = Join-Path $BuildDir 'Embedded_RTOS_Project1.ino.bin'
$Elf = Join-Path $BuildDir 'Embedded_RTOS_Project1.ino.elf'
if (-not (Test-Path $Bin) -or -not (Test-Path $Elf)) {
    throw 'Build completed but the Wokwi firmware/ELF files were not found.'
}

Write-Host ''
Write-Host 'Build successful.' -ForegroundColor Green
Write-Host "Firmware: $Bin"
Write-Host "ELF:      $Elf"
Write-Host 'Next step: open diagram.json in VS Code and start Wokwi.'
