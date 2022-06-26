Set-Location -Path "build"
$version = (Get-Content version -Raw)
$config = @{
  Path = "sll.exe", "sllw.exe", "sll-$version.dll", "sll-$version.lib", "lib\", "lib_debug\", "sys_lib\"
  CompressionLevel = "Optimal"
  DestinationPath = "..\windows.zip"
}
Compress-Archive @config
Set-Location -Path ".."
