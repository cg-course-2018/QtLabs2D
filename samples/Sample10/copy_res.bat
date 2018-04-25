@echo off
echo Copying resources...

set ProjectDir=%1
set OutDir=%2

if not exist "%OutDir%res10" mkdir "%OutDir%res10"
echo D | xcopy /Y "%ProjectDir%res\*.frag" "%OutDir%res10"
echo D | xcopy /Y "%ProjectDir%res\*.vert" "%OutDir%res10"
echo D | xcopy /Y "%ProjectDir%res\*.png" "%OutDir%res10"
echo D | xcopy /Y "%ProjectDir%res\*.plist" "%OutDir%res10"
echo D | xcopy /Y "%ProjectDir%res\*.ttf" "%OutDir%res10"
