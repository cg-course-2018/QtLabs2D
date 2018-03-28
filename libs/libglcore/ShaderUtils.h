#pragma once

#include "OpenGL_raii.h"
#include <string>
#include <vector>

namespace glcore
{

// Фасад функции glCompileShader
// Компилирует шейдер заданного типа из строки с исходным кодом
// @param type - это GL_VERTEX_SHADER, GL_FRAGMENT_SHADER или GL_GEOMETRY_SHADER.
ShaderObject compileShader(gl::GLenum type, const std::string &source);

// Фасад функции glLinkProgram
// Выполняет компоновку шейдеров в единую программу
ProgramObject linkProgram(const std::vector<ShaderObject> &shaders);

// Фасад функции glValidateProgram
// Валидация - необязательный этап, который может сообщить
// о проблемах производительности или предупреждениях компилятора GLSL
// Если проблемы есть, они выводятся в поток out.
void validateProgram(const ProgramObject &program, std::ostream &out);

} // namespace glcore
