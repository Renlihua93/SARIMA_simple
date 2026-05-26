# Build sarimatest with CLion-bundled CMake/Ninja (same as ctsa-master)
$Root = $PSScriptRoot
$Cmake = "C:\software\CLion 2026.1.2\bin\cmake\win\x64\bin\cmake.exe"
$Ninja = "C:\software\CLion 2026.1.2\bin\ninja\win\x64\ninja.exe"
$Gcc = "C:/software/CLion 2026.1.2/bin/mingw/bin/gcc.exe"
$BuildDir = Join-Path $Root "cmake-build-debug"

if (-not (Test-Path $Cmake)) {
    Write-Error "CMake not found at $Cmake — open project in CLion and Build instead."
    exit 1
}

if (-not (Test-Path $BuildDir)) {
    & $Cmake -S $Root -B $BuildDir -G Ninja `
        -DCMAKE_BUILD_TYPE=Debug `
        -DCMAKE_MAKE_PROGRAM=$Ninja `
        -DCMAKE_C_COMPILER=$Gcc
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

& $Cmake --build $BuildDir --target sarimatest
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$Exe = Join-Path $BuildDir "sarimatest.exe"
if (Test-Path $Exe) {
    Write-Host "`n--- sarimatest output ---"
    & $Exe
}
