@echo off
@echo 提出に不要なファイルを削除します
pause

forfiles /s /m *.obj /c "cmd /c del /q @file"
forfiles /s /m *.user /c "cmd /c del /q @file"
forfiles /s /m *.lib /c "cmd /c del /q @file"
forfiles /s /m *.ilk /c "cmd /c del /q @file"
forfiles /s /m *.db /c "cmd /c del /q @file"
forfiles /s /m *.log /c "cmd /c del /q @file"
forfiles /s /m *.tlog /c "cmd /c del /q @file"
forfiles /s /m *.idb /c "cmd /c del /q @file"
forfiles /s /m *.pdb /c "cmd /c del /q @file"
forfiles /s /m *.suo /c "cmd /c del /ah /q @file"
forfiles /s /m *.pch /c "cmd /c del /q @file"
forfiles /s /m *.ipch /c "cmd /c del /q @file"
forfiles /s /m *.iobj /c "cmd /c del /q @file"
forfiles /s /m *.ipdb /c "cmd /c del /q @file"
forfiles /s /m *.cso /c "cmd /c del /q @file"
forfiles /s /m *.exe /c "cmd /c del /q @file"

pause