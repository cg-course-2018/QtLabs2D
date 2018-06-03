@echo off
echo Copying resources...

set ProjectDir=%1
set BuildDir=%2
set OutDir=%BuildDir%res16

if not exist "%OutDir%" mkdir "%OutDir%"
echo D | xcopy /Y "%ProjectDir%res\*.frag" "%OutDir%"
echo D | xcopy /Y "%ProjectDir%res\*.vert" "%OutDir%"
echo D | xcopy /Y "%ProjectDir%res\*.png" "%OutDir%"
echo D | xcopy /Y "%ProjectDir%res\*.json" "%OutDir%"
