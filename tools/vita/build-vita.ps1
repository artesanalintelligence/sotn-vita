<#
.SYNOPSIS
Builds sotn.vpk and the ux0:data/sotn folder for the PS Vita from your own
Castlevania: Symphony of the Night (USA, SLUS-00067) disc image, with Docker
Desktop.

.DESCRIPTION
Builds the tools/vita/docker image (VitaSDK and build tools) from this
repository, then runs tools/vita/build_from_rom.sh inside it. The folder of
the .cue is mounted read-only; nothing from the disc goes into the image.

.EXAMPLE
powershell -ExecutionPolicy Bypass -File tools\vita\build-vita.ps1 -Cue "D:\roms\Castlevania - Symphony of the Night (USA).cue"

.EXAMPLE
powershell -ExecutionPolicy Bypass -File tools\vita\build-vita.ps1 -Cue D:\roms\sotn.cue -Out D:\sotn-vita -Jobs 4
#>
param(
    # Cue sheet of the disc; its .bin files must be in the same folder
    [Parameter(Mandatory = $true)][string]$Cue,
    # Output folder (default: build\vita-out in the repository)
    [string]$Out = "",
    # Docker image tag
    [string]$Image = "sotn-vita-kit",
    # Reuse the existing image instead of rebuilding it
    [switch]$SkipImageBuild,
    # Parallel compile jobs inside the container (default: all CPUs)
    [int]$Jobs = 0,
    # Redo every step, ignoring the step cache
    [switch]$Clean
)
$ErrorActionPreference = "Stop"

function Fail($msg) {
    Write-Host "ERROR: $msg" -ForegroundColor Red
    exit 1
}

$repo = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path

$docker = (Get-Command docker -ErrorAction SilentlyContinue).Source
if (-not $docker) {
    $candidate = "C:\Program Files\Docker\Docker\resources\bin\docker.exe"
    if (Test-Path $candidate) { $docker = $candidate }
}
if (-not $docker) { Fail "docker not found. Install Docker Desktop: https://www.docker.com/products/docker-desktop/" }
# Windows PowerShell turns redirected native stderr into errors, keep going
$ErrorActionPreference = "Continue"
& $docker info --format "{{.ServerVersion}}" 2>$null | Out-Null
$dockerUp = ($LASTEXITCODE -eq 0)
$ErrorActionPreference = "Stop"
if (-not $dockerUp) { Fail "Docker is not running. Start Docker Desktop (Linux containers) and try again." }

if (-not (Test-Path -LiteralPath $Cue -PathType Leaf)) { Fail "cue sheet not found: $Cue" }
$cuePath = (Resolve-Path -LiteralPath $Cue).Path
$romDir = Split-Path -Parent $cuePath
$cueName = Split-Path -Leaf $cuePath
if ($romDir.StartsWith($repo + "\", [StringComparison]::OrdinalIgnoreCase) -and
    -not $romDir.StartsWith((Join-Path $repo "disks") , [StringComparison]::OrdinalIgnoreCase)) {
    Write-Host "WARNING: the disc image is inside the repository; keep it outside (or in disks\) so it is never committed." -ForegroundColor Yellow
}

if ($Out -eq "") { $Out = Join-Path $repo "build\vita-out" }
New-Item -ItemType Directory -Force -Path $Out | Out-Null
$outDir = (Resolve-Path -LiteralPath $Out).Path

$script = Join-Path $repo "tools\vita\build_from_rom.sh"
if ([IO.File]::ReadAllText($script).Contains("`r`n")) {
    Write-Host "WARNING: the checkout has CRLF line endings. The image normalizes the scripts, but if the build fails run: git rm --cached -r -q . ; git reset --hard" -ForegroundColor Yellow
}

if (-not $SkipImageBuild) {
    Write-Host "==> Building the Docker image $Image (first time: 5-10 minutes)" -ForegroundColor Cyan
    & $docker build -f (Join-Path $repo "tools\vita\docker\Dockerfile") -t $Image $repo
    if ($LASTEXITCODE -ne 0) { Fail "docker build failed (see above)" }
}

$runArgs = @("run", "--rm", "-v", "${romDir}:/rom:ro", "-v", "${outDir}:/out", $Image)
if ($Jobs -gt 0) { $runArgs += @("--jobs", "$Jobs") }
if ($Clean) { $runArgs += "--clean" }
$runArgs += @("/rom/$cueName", "/out")
Write-Host "==> Building the Vita port from $cueName" -ForegroundColor Cyan
& $docker @runArgs
if ($LASTEXITCODE -ne 0) { Fail "the build failed (see above)" }

Write-Host ""
Write-Host "Output: $outDir" -ForegroundColor Green
Write-Host "  sotn.vpk           the application"
Write-Host "  ux0\data\sotn\     game data (your disc), goes to ux0:data/sotn"
Write-Host ""
Write-Host "PS Vita (HENkaku/Enso + VitaShell):"
Write-Host "  1. Copy sotn.vpk to the Vita (USB or FTP in VitaShell) and install it."
Write-Host "  2. Copy the folder ux0\data\sotn to ux0:data/sotn on the Vita."
Write-Host "Vita3K:"
Write-Host "  1. File > Install .vpk > sotn.vpk"
Write-Host "  2. Copy ux0\data\sotn into the emulator's ux0\data\ folder"
Write-Host "     (default: %APPDATA%\Vita3K\Vita3K\ux0\data\sotn)."
