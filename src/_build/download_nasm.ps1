Invoke-WebRequest "https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip" -OutFile "$ENV:tmp/nasm.zip"
Expand-Archive -Force -LiteralPath "$ENV:tmp/nasm.zip" -DestinationPath "$ENV:tmp/nasm"
Add-Content "$ENV:GITHUB_ENV" "PATH=$ENV:tmp/nasm/nasm-2.15.05/;$ENV:PATH"
