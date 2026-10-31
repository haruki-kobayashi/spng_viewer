param([string]$Destination=(Join-Path $PSScriptRoot 'vendor_sources'))
$ErrorActionPreference='Stop'
$Destination=[IO.Path]::GetFullPath($Destination)
New-Item -ItemType Directory -Path $Destination -Force | Out-Null
$items=Get-Content -LiteralPath (Join-Path $PSScriptRoot 'vendor_sources\SHA256.json') -Raw | ConvertFrom-Json
foreach($item in $items){
    if([IO.Path]::GetFileName($item.file) -ne $item.file -or ([uri]$item.url).Scheme -ne 'https'){throw 'Invalid dependency manifest'}
    $target=Join-Path $Destination $item.file
    if(Test-Path -LiteralPath $target){
        if((Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash -ne $item.sha256){throw "Checksum mismatch; existing file left untouched: $target"}
        Write-Host "Verified: $($item.file)"
        continue
    }
    $temporary=Join-Path $Destination ($item.file+'.'+[guid]::NewGuid().ToString('N')+'.partial')
    try {
        Write-Host "Downloading: $($item.file)"
        Invoke-WebRequest -Uri $item.url -OutFile $temporary -UseBasicParsing
        if((Get-Item -LiteralPath $temporary).Length -ne $item.bytes -or (Get-FileHash -LiteralPath $temporary -Algorithm SHA256).Hash -ne $item.sha256){throw "Downloaded source checksum mismatch: $($item.file)"}
        Move-Item -LiteralPath $temporary -Destination $target -ErrorAction Stop
    } finally {
        if(Test-Path -LiteralPath $temporary){Remove-Item -LiteralPath $temporary -Force}
    }
}
