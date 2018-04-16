@echo off
echo Copying resources...

set ProjectDir=%1
set OutDir=%2

echo F | xcopy /Y "%ProjectDir%draw3d.frag" "%OutDir%draw3d.frag"
echo F | xcopy /Y "%ProjectDir%draw3d.vert" "%OutDir%draw3d.vert"
