param(
    [string]$Executable=(Join-Path $PSScriptRoot 'out\app\SPNGViewer.exe'),
    [string]$OutputDirectory=(Join-Path $PSScriptRoot 'out\release')
)
$ErrorActionPreference='Stop'
$Executable=[IO.Path]::GetFullPath($Executable)
$OutputDirectory=[IO.Path]::GetFullPath($OutputDirectory)
if(!(Test-Path -LiteralPath $Executable -PathType Leaf)){throw "Executable not found: $Executable. Build the application first."}
if(Test-Path -LiteralPath $OutputDirectory){throw "Output directory already exists: $OutputDirectory. Specify a new directory."}
# A failed package remains available for inspection; never overwrite or delete it.
New-Item -ItemType Directory -Path $OutputDirectory | Out-Null
try {
    $initialHash=(Get-FileHash -LiteralPath $Executable -Algorithm SHA256).Hash
    Copy-Item -LiteralPath $Executable -Destination (Join-Path $OutputDirectory 'SPNGViewer.exe')
    foreach($name in @('LICENSE','THIRD_PARTY_NOTICES.txt','BUILDING.md','VERIFICATION.md','docs','licenses')){
        Copy-Item -LiteralPath (Join-Path $PSScriptRoot $name) -Destination $OutputDirectory -Recurse
    }
    Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'RELEASE_README.md') -Destination (Join-Path $OutputDirectory 'README.md')
    & (Join-Path $PSScriptRoot 'package_source.ps1') -OutputFile (Join-Path $OutputDirectory 'SPNGViewer-source-and-dependencies.zip')
    $copiedHash=(Get-FileHash -LiteralPath (Join-Path $OutputDirectory 'SPNGViewer.exe') -Algorithm SHA256).Hash
    if($copiedHash -ne $initialHash -or (Get-FileHash -LiteralPath $Executable -Algorithm SHA256).Hash -ne $initialHash){throw 'Executable changed during packaging.'}
    $lines=foreach($name in @('SPNGViewer.exe','SPNGViewer-source-and-dependencies.zip')){
        $hash=(Get-FileHash -LiteralPath (Join-Path $OutputDirectory $name) -Algorithm SHA256).Hash.ToLowerInvariant()
        "$hash  $name"
    }
    $lines | Set-Content -LiteralPath (Join-Path $OutputDirectory 'SHA256SUMS.txt') -Encoding ASCII
    Write-Host "Created: $OutputDirectory"
}catch{
    Write-Warning "Packaging failed. Incomplete output retained at: $OutputDirectory"
    throw
}
