#pragma once

namespace platform
{
// Настраивает перенаправление ошибок, обнаруженных реализацией OpenGL, в QDebug,
//  используя расширение https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_debug_output.txt
void SetupDebugOutput();
}
