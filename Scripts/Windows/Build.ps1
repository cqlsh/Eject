param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Release'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$repositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path $vswhere))
{
    throw 'Visual Studio or the Visual Studio Build Tools are required to build Eject.'
}

$vswhereArguments = @(
    '-latest'
    '-products', '*'
    '-requires', 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64'
    '-property', 'installationPath'
)
$installationPath = & $vswhere @vswhereArguments
if (-not $installationPath)
{
    throw 'No Visual Studio installation with the MSVC x64 build tools was found.'
}

$installerPath = Split-Path $vswhere
$developerPrompt = Join-Path $installationPath 'Common7\Tools\VsDevCmd.bat'
$steps = @(
    "set `"PATH=$installerPath;%PATH%`""
    "`"$developerPrompt`" -arch=x64 -host_arch=x64 -no_logo"
    "cmake --workflow --preset Windows-$Configuration"
)
$command = $steps -join ' && '

Push-Location $repositoryRoot
try
{
    & $env:ComSpec /d /c $command
    $exitCode = $LASTEXITCODE
}
finally
{
    Pop-Location
}

exit $exitCode