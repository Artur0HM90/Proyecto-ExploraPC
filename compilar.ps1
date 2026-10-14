param([ValidateSet('Release','Debug')][string]$Configuracion='Release')
$ErrorActionPreference='Stop'
Push-Location $PSScriptRoot
try {
    $vswhereRuta='C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhereRuta)) { throw 'Instala Visual Studio con Desarrollo para el escritorio con C++.' }
    $vsRuta=& $vswhereRuta -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $vsRuta) { throw 'Falta MSVC. Agrega Desarrollo para el escritorio con C++ en Visual Studio Installer.' }
    $cmakeRuta=Join-Path $vsRuta 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
    if (-not (Test-Path -LiteralPath $cmakeRuta)) { throw 'Faltan las herramientas CMake de C++ para Windows en Visual Studio Installer.' }
    $vsVersion=& $vswhereRuta -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationVersion
    $preset=if ($vsVersion.StartsWith('18.')) { 'vs2026' } elseif ($vsVersion.StartsWith('17.')) { 'vs2022' } else { throw "Visual Studio no contemplado: $vsVersion" }
    & $cmakeRuta --preset $preset
    if ($LASTEXITCODE -ne 0) { throw 'No se pudo configurar el proyecto.' }
    & $cmakeRuta --build "build/$preset" --config $Configuracion --parallel 4
    if ($LASTEXITCODE -ne 0) { throw 'La compilacion fallo. Revisa los errores anteriores.' }
    Write-Host "Listo: build/$preset/$Configuracion/ExploraPC.exe"
} finally { Pop-Location }
