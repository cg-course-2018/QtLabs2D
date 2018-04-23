@echo off
echo Copying resources...

set ProjectDir=%1
set OutDir=%2

if not exist "%OutDir%res09" mkdir "%OutDir%res09"
echo D | xcopy /Y "%ProjectDir%*.frag" "%OutDir%res09"
echo D | xcopy /Y "%ProjectDir%*.vert" "%OutDir%res09"
