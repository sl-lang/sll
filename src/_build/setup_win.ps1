Invoke-WebRequest "https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip" -OutFile "$ENV:TMP/nasm.zip"
Expand-Archive -Force -LiteralPath "$ENV:TMP/nasm.zip" -DestinationPath "$ENV:TMP/nasm"
$vswhere_output = (& "${Env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -products '*' -latest -prerelease -property installationPath) + "\VC\Auxiliary\Build\vcvarsall.bat"
foreach ($elem in @(& "$vswhere_output" x64 "&&" set)) {
	$elem=$elem.Split("=")
	if ($elem[0] -eq "PATH"){
		$PATH=$elem[1]
	}
	elseif ($elem[0] -eq "INCLUDE"){
		$INCLUDE=$elem[1]
	}
	elseif ($elem[0] -eq "LIB"){
		$LIB=$elem[1]
	}
	elseif ($elem[0] -eq "LIBPATH"){
		$LIBPATH=$elem[1]
	}
}
Add-Content "$ENV:GITHUB_ENV" "`nPATH=$ENV:TMP/nasm/nasm-2.15.05/;$PATH`nINCLUDE=$INCLUDE`nLIB=$LIB`nLIBPATH=$LIBPATH`n"
