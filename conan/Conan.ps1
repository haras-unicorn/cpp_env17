$script = $MyInvocation.MyCommand.Path
$loc = "$script/.."

Write-Output "`n`n *** Installing... *** `n`n"
. "$loc/ConanInstall.ps1"

Write-Output "`n`n *** Building... *** `n`n"
. "$loc/ConanBuild.ps1"
