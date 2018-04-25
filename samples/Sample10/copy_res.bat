@echo off
echo Copying resources...

set ProjectDir=%1
set OutDir=%2

if not exist "%OutDir%res10" mkdir "%OutDir%res10"
echo D | xcopy /Y "%ProjectDir%*.frag" "%OutDir%res10"
echo D | xcopy /Y "%ProjectDir%*.vert" "%OutDir%res10"
