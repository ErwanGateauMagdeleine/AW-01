[Setup]
AppName=AW-01
AppVersion=0.0.1
DefaultDirName={commoncf}\VST3
DefaultGroupName=AW-01
OutputBaseFilename=AW-01
Uninstallable=no
DisableDirPage=no
ArchitecturesInstallIn64BitMode=x64

[messages]
SelectDirLabel3=Select your VST3 plugin folder

[Files]
Source: "../build/OUT/AW-01.vst3/Contents/x86_64-win/AW-01.vst3"; DestDir: "{app}"; Flags: ignoreversion
