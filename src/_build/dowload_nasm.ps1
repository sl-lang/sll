curl -L -o "$ENV:tmp/nasm.zip" "https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip"
Expand-Archive -Force -LiteralPath "$ENV:tmp/nasm.zip" -DestinationPath "$ENV:tmp/nasm"
$ENV:path="$ENV:tmp/nasm/nasm-2.15.05/;$ENV:path"
