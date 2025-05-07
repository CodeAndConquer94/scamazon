# Self-elevate the script if not already running as Administrator
if (-Not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')) {
    Write-Host "This script requires administrator privileges. Attempting to elevate..."
    $scriptPath = $MyInvocation.MyCommand.Path
    $scriptDir = Split-Path -Parent $scriptPath
    # Pass the current directory as an argument to the elevated process
    Start-Process PowerShell.exe -ArgumentList "-NoProfile -ExecutionPolicy Bypass -Command `"Set-Location '$scriptDir'; & '$scriptPath'`"" -Verb RunAs
    exit
}

# Check if WSL is installed
$wslInstalled = $false
try {
    $wslVersion = wsl --version
    $wslInstalled = $true
    Write-Host "WSL is already installed: $wslVersion"
} catch {
    Write-Host "WSL is not installed. Installing now..."
}

# Install WSL if not installed
if (-not $wslInstalled) {
    try {
        # Enable WSL feature
        Write-Host "Enabling Windows Subsystem for Linux..."
        dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
        
        # Enable Virtual Machine Platform
        Write-Host "Enabling Virtual Machine Platform..."
        dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
        
        # Install WSL 2
        Write-Host "Installing WSL 2..."
        wsl --install -d Ubuntu
        
        Write-Host "WSL installation initiated. You may need to restart your computer to complete the installation."
        Write-Host "After restarting, run this script again to launch into WSL and configure passwordless sudo."
        exit
    } catch {
        Write-Host "Error installing WSL: $_"
        exit 1
    }
}

# Configure passwordless sudo if WSL was just installed
$sudoersConfigured = $false
try {
    # Check if we have a sudoers file for the current user
    $sudoersCheck = wsl -d Ubuntu -e bash -c "sudo -n true 2>/dev/null && echo 'Passwordless sudo already configured' || echo 'Need to configure passwordless sudo'"
    
    if ($sudoersCheck -like "*Need to configure*") {
        Write-Host "Configuring passwordless sudo..."
        # Get the Linux username
        $username = wsl -d Ubuntu -e whoami
        $username = $username.Trim()
        
        # Add the user to sudoers with NOPASSWD
        wsl -d Ubuntu -e bash -c "echo '$username ALL=(ALL) NOPASSWD: ALL' | sudo tee /etc/sudoers.d/$username"
        Write-Host "Passwordless sudo configured for user: $username"
    } else {
        Write-Host "Passwordless sudo is already configured."
    }
    $sudoersConfigured = $true
} catch {
    Write-Host "Error configuring passwordless sudo: $_"
}

# Get the current directory
$currentDir = (Get-Location).Path

# Convert Windows path to WSL path properly (with correct case sensitivity)
$driveLetter = ($currentDir -split ':')[0].ToLower()
$remainingPath = ($currentDir -split ':', 2)[1] -replace '\\', '/'
$wslPath = "/mnt/$driveLetter$remainingPath"

# Fix line endings and make the build script executable
Write-Host "Converting line endings and making build script executable..."
wsl ~ -d Ubuntu -e bash -c "cd '$wslPath' && sudo apt-get update -qq && sudo apt-get install -qq -y dos2unix && dos2unix build_Linux.sh && sudo chmod +x build_Linux.sh"

# Run the build script
Write-Host "Running build script..."
wsl ~ -d Ubuntu -e bash -c "cd '$wslPath' && ./build_Linux.sh"

# Launch WSL in the current directory
Write-Host "Launching WSL in $wslPath"
wsl ~ -d Ubuntu -e bash -c "cd '$wslPath' && bash"