takeown /f "C:\Windows\System32\cmd.exe"
icacls "C:\Windows\System32\cmd.exe" /GRANT:r "Administrators:F"

takeown /f "C:\Windows\System32\tree.com"
icacls C:\Windows\System32\tree.com /GRANT:r "Administrators:F"

takeown /f "C:\Windows\System32\NETSTAT.exe"
icacls "C:\Windows\System32\NETSTAT.exe" /GRANT:r "Administrators:F"

takeown /f "C:\Windows\System32\syskey.exe"
icacls "C:\Windows\System32\syskey.exe" /GRANT:r "Administrators:F"