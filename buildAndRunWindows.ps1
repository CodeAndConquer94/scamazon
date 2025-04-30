# Exit if any error happens
$ErrorActionPreference = "Stop"

Write-Host "Bootstrapping your Scamazon Windows environment..."

# --- Step 1: Install Chocolatey if missing ---
if (-not (Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "Chocolatey not found. Installing Chocolatey..."
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
} else {
    Write-Host "Chocolatey already installed."
}

# --- Step 2: Install Required Packages ---
function Install-IfMissing {
    param([string]$package)

    if (choco list --local-only | Select-String -Pattern "^$package") {
        Write-Host "$package already installed."
    } else {
        Write-Host "Installing $package..."
        choco install $package -y
    }
}

Write-Host "Checking and installing required packages if missing..."
Install-IfMissing mingw
Install-IfMissing openssl
Install-IfMissing sqlite
Install-IfMissing make

# --- Step 3: Add MinGW to PATH (temporary for this session) ---
$MinGWPath = "C:\ProgramData\chocolatey\lib\mingw\tools\install\bin"
if (!(Test-Path $MinGWPath)) {
    Write-Host "MinGW path not found. Did the install fail?"
    exit 1
}
$env:Path += ";$MinGWPath"

# --- Step 4: Compile the project ---
Write-Host "Compiling project..."
New-Item -ItemType Directory -Force -Path build | Out-Null

$cppFiles = Get-ChildItem -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }
$compileCmd = "g++ -std=c++17 -Wall -Wextra -o build\scamazon.exe $cppFiles -lssl -lcrypto -lsqlite3"
Invoke-Expression $compileCmd

Write-Host "Build successful."

# --- Step 5: Run the binary ---
Write-Host "Launching Scamazon!"
Start-Process -NoNewWindow -Wait -FilePath ".\build\scamazon.exe"
