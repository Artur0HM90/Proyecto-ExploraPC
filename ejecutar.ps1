param([ValidateSet('Release','Debug')][string]$Configuracion='Release')
$ErrorActionPreference='Stop'
Push-Location $PSScriptRoot
try {
    $ejecutable=Join-Path $PSScriptRoot "build/vs2026/$Configuracion/ExploraPC.exe"
    if (-not (Test-Path -LiteralPath $ejecutable)) { $ejecutable=Join-Path $PSScriptRoot "build/vs2022/$Configuracion/ExploraPC.exe" }
    if (-not (Test-Path -LiteralPath $ejecutable)) { throw 'Primero ejecuta compilar.ps1 o los comandos del README.' }
    & $ejecutable
    if ($LASTEXITCODE -ne 0) { throw 'La aplicacion informo un error. Revisa la consola.' }
} finally { Pop-Location }
