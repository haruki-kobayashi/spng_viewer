param([string]$OutputRoot=(Join-Path $PSScriptRoot 'out\dependencies'),[int]$Jobs=4)
$ErrorActionPreference='Stop'
if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) { throw 'Use the Visual Studio x64 Developer PowerShell.' }
$OutputRoot=[System.IO.Path]::GetFullPath($OutputRoot)
$sources=Join-Path $OutputRoot 'sources'
New-Item -ItemType Directory -Path $sources -Force | Out-Null
$vendor=Join-Path $PSScriptRoot 'vendor_sources'
& (Join-Path $PSScriptRoot 'fetch_dependencies.ps1') -Destination $vendor
$hashes=Get-Content -LiteralPath (Join-Path $vendor 'SHA256.json') -Raw | ConvertFrom-Json
foreach($item in $hashes) {
    $archive=Join-Path $vendor $item.file
    if((Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash -ne $item.sha256){throw "Source checksum mismatch: $($item.file)"}
}
foreach($item in @(@('qtbase-everywhere-src-6.11.2','qtbase-everywhere-src-6.11.2.tar.xz'),@('opencv-5.0.0','opencv-5.0.0.zip'))) {
    if(-not (Test-Path -LiteralPath (Join-Path $sources ($item[0]+'\CMakeLists.txt')))) {
        Push-Location $sources
        try { & cmake -E tar xf (Join-Path $vendor $item[1]); if($LASTEXITCODE -ne 0){throw 'Source extraction failed'} } finally { Pop-Location }
    }
}
$qtSource=Join-Path $sources 'qtbase-everywhere-src-6.11.2'
$cvSource=Join-Path $sources 'opencv-5.0.0'
$ipp=Get-ChildItem -LiteralPath $vendor -Filter '*ippicv*.zip' | Select-Object -First 1
$ippCache=Join-Path $cvSource '.cache\ippicv'
New-Item -ItemType Directory -Path $ippCache -Force | Out-Null
if(-not(Test-Path -LiteralPath (Join-Path $ippCache $ipp.Name))){Copy-Item -LiteralPath $ipp.FullName -Destination $ippCache}
function Invoke-Build([string]$Source,[string]$Build,[string[]]$Options) {
    & cmake -S $Source -B $Build -G Ninja '-DCMAKE_BUILD_TYPE=Release' @Options
    if($LASTEXITCODE -ne 0){throw 'Dependency configuration failed'}
    & cmake --build $Build --parallel $Jobs
    if($LASTEXITCODE -ne 0){throw 'Dependency build failed'}
    & cmake --install $Build
    if($LASTEXITCODE -ne 0){throw 'Dependency installation failed'}
}
Invoke-Build $qtSource (Join-Path $OutputRoot 'qt-build') @('-DBUILD_SHARED_LIBS=OFF','-DFEATURE_static_runtime=ON','-DQT_BUILD_TESTS=OFF','-DQT_BUILD_EXAMPLES=OFF','-DQT_BUILD_BENCHMARKS=OFF','-DFEATURE_sql=OFF','-DFEATURE_network=ON','-DFEATURE_dbus=OFF','-DFEATURE_printsupport=OFF','-DFEATURE_testlib=ON','-DFEATURE_concurrent=ON','-DFEATURE_icu=OFF','-DFEATURE_system_zlib=OFF','-DFEATURE_system_png=OFF','-DFEATURE_openssl=OFF','-DINPUT_opengl=desktop',('-DCMAKE_INSTALL_PREFIX='+(Join-Path $OutputRoot 'qt-static')))
Invoke-Build $cvSource (Join-Path $OutputRoot 'opencv-build') @('-DBUILD_SHARED_LIBS=OFF','-DBUILD_WITH_STATIC_CRT=ON','-DBUILD_LIST=core,imgproc','-DBUILD_TESTS=OFF','-DBUILD_PERF_TESTS=OFF','-DBUILD_EXAMPLES=OFF','-DBUILD_opencv_apps=OFF','-DWITH_OPENCL=OFF','-DWITH_IPP=ON','-DWITH_ITT=OFF','-DWITH_LAPACK=OFF','-DBUILD_JAVA=OFF','-DBUILD_opencv_python3=OFF','-DCPU_BASELINE=SSE3','-DWITH_TIFF=OFF','-DWITH_WEBP=OFF','-DWITH_PNG=OFF','-DWITH_JPEG=OFF','-DWITH_OPENJPEG=OFF','-DWITH_JASPER=OFF','-DWITH_PROTOBUF=OFF','-DWITH_OPENEXR=OFF','-DWITH_AVIF=OFF','-DWITH_FFMPEG=OFF','-DWITH_UNIFONT=OFF',('-DCMAKE_INSTALL_PREFIX='+(Join-Path $OutputRoot 'opencv-static')))
