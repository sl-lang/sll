Set-Location -Path "build"
$config = @{
  Path = "sll.exe", "sllw.exe", ("sll-"+(Get-Content version -Raw)+".dll"), ("sll-"+(Get-Content version -Raw)+".lib"), "lib\", "lib_debug\", "sys_lib\"
  CompressionLevel = "Optimal"
  DestinationPath = "..\windows.zip"
}
Compress-Archive @config
Set-Location -Path ".."
