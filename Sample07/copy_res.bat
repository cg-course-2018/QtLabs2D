@echo off
echo Copying resources...

set ProjectDir=%1
set OutDir=%2

echo F | xcopy /Y "%ProjectDir%draw2d.frag" "%OutDir%draw2d.frag"
echo F | xcopy /Y "%ProjectDir%draw2d.vert" "%OutDir%draw2d.vert"