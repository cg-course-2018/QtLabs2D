@echo off
echo Copying resources...

set ProjectDir=%1
set OutDir=%2

if not exist "%OutDir%res10" mkdir "%OutDir%res10"
echo D | xcopy /Y "res10\%ProjectDir%*.frag" "%OutDir%res10"
echo D | xcopy /Y "res10\%ProjectDir%*.vert" "%OutDir%res10"
echo D | xcopy /Y "res10\%ProjectDir%*.png" "%OutDir%res10"
echo D | xcopy /Y "res10\%ProjectDir%*.xml" "%OutDir%res10"
