# Anti Tech Support ScamKit
Out of nowhere, I started receiving tech support scam calls, I'm talking like 5 per day!
So I've decided to fuck with them a wee bit.

## ScamKit
ScamKit contains:  
Fake `cmd.exe` - All commands yield `'%s' is not recognized as an internal or external command, operable program or batch file.`  
Fake `NETSTAT.exe` - Prints `No active connections - System is secure`  
Fake `tree.com` - Prints `System is secure`  
Fake `syskey.exe` - Fake SYSKEY (SAM Lock Tool) GUI. Nothing is locked. Instead, the encryption password is copied to clipboard after being set.  

## PowerShell Scripts
`Perms_GRANT.ps1` - Removes `TrustedInstaller` from original executables, so they can be replaced with fake ones.
`Perms_REVOKE.ps1` - What the fuck do you think..

## `ScamKit.exe`
Automatically backs up original executables and replaces them with fake ones.
This action is reversible, just run `ScamKit.exe --restore`

### 5IQ HOW-TO
1. Run `Perms_GRANT.ps1` in Administrator PowerShell (May need to run `Set-ExecutionPolicy RemoteSigned`)
2. Run `ScamKit.exe` from Administrator CMD
3. DONE

## 3IQ HOW-TO-REVERSE
1. Run `ScamKit.exe --restore` from Administrator CMD
2. Run `Perms_REVOKE.ps1`
3. Done


I am in no-way responsible for any actions done by this software.
As this also messes up with `System32`, I am NOT responsible for any damage done to any machine, EVER.

Please just use this inside a VM.

## DEMO
[YouTube DEMO](https://www.youtube.com/watch?v=IvT0WidP7h4)

ski ski ski píčo Alberto Tomba
