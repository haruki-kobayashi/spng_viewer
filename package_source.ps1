param([string]$OutputFile=(Join-Path $PSScriptRoot 'out\packages\SPNGViewer-source-and-dependencies.zip'))
$ErrorActionPreference='Stop'
$OutputFile=[IO.Path]::GetFullPath($OutputFile)
if([IO.Path]::GetExtension($OutputFile) -ne '.zip' -or (Test-Path -LiteralPath $OutputFile)){throw 'Specify a new .zip output file.'}
& (Join-Path $PSScriptRoot 'fetch_dependencies.ps1')
$parent=[IO.Path]::GetDirectoryName($OutputFile)
New-Item -ItemType Directory -Path $parent -Force | Out-Null
$workspace=Join-Path $parent ('source-package-'+[guid]::NewGuid().ToString('N'))
$source=Join-Path $workspace 'source'
New-Item -ItemType Directory -Path $source | Out-Null
try {
    foreach($name in @('CMakeLists.txt','README.md','BUILDING.md','VERIFICATION.md','LICENSE','THIRD_PARTY_NOTICES.txt','build.ps1','build_dependencies.ps1','fetch_dependencies.ps1','package_source.ps1','package_release.ps1','RELEASE_README.md','.gitignore','.gitattributes')){
        Copy-Item -LiteralPath (Join-Path $PSScriptRoot $name) -Destination $source
    }
    foreach($name in @('src','docs','licenses','vendor_sources','tests\fixtures','tests\screen_fixture')){
        $destination=Join-Path $source $name
        New-Item -ItemType Directory -Path ([IO.Path]::GetDirectoryName($destination)) -Force | Out-Null
        if($name -eq 'vendor_sources'){
            New-Item -ItemType Directory -Path $destination | Out-Null
            Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'vendor_sources\SHA256.json') -Destination $destination
            $items=Get-Content -LiteralPath (Join-Path $PSScriptRoot 'vendor_sources\SHA256.json') -Raw | ConvertFrom-Json
            foreach($item in $items){Copy-Item -LiteralPath (Join-Path $PSScriptRoot ('vendor_sources\'+$item.file)) -Destination $destination}
        }else{Copy-Item -LiteralPath (Join-Path $PSScriptRoot $name) -Destination $destination -Recurse}
    }
    Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot 'tests') -File -Filter '*.cpp' | ForEach-Object {Copy-Item -LiteralPath $_.FullName -Destination (Join-Path $source 'tests')}
    $evidence=Join-Path $source 'evidence'
    New-Item -ItemType Directory -Path $evidence | Out-Null
    Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot 'evidence') -File -Filter 'expected_*.png' | ForEach-Object {Copy-Item -LiteralPath $_.FullName -Destination $evidence}
    Compress-Archive -LiteralPath $source -DestinationPath $OutputFile -CompressionLevel Optimal
    Write-Host "Created: $OutputFile"
}finally{
    $resolved=(Resolve-Path -LiteralPath $workspace).ProviderPath
    if([IO.Path]::GetDirectoryName($resolved) -ne $parent -or [IO.Path]::GetFileName($resolved) -notlike 'source-package-*'){throw 'Unexpected temporary source-package directory'}
    Remove-Item -LiteralPath $resolved -Recurse -Force
}
