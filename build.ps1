param(
    [Parameter(Mandatory=$true)][string]$QtPrefix,
    [Parameter(Mandatory=$true)][string]$OpenCVConfig,
    [string]$BuildDirectory = (Join-Path $PSScriptRoot 'out\app'),
    [int]$Jobs = 4
)
$ErrorActionPreference='Stop'
if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) { throw 'Use the Visual Studio x64 Developer PowerShell.' }
$qt=(Resolve-Path -LiteralPath $QtPrefix).Path
$cv=(Resolve-Path -LiteralPath $OpenCVConfig).Path
& cmake -S $PSScriptRoot -B $BuildDirectory -G Ninja '-DCMAKE_BUILD_TYPE=Release' '-DSPNG_BUILD_TESTS=OFF' '-DOpenCV_STATIC=ON' "-DCMAKE_PREFIX_PATH=$qt" "-DOpenCV_DIR=$cv"
if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed' }
& cmake --build $BuildDirectory --target SPNGViewer --parallel $Jobs
if ($LASTEXITCODE -ne 0) { throw 'Application build failed' }
