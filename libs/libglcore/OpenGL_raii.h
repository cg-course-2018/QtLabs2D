#pragma once

/**
 * Файл содержит RAII для управления временем жизни различных ресурсов OpenGL
 */

#include "GLObject.h"

namespace glcore
{

void deleteShader(gl::GLuint handle) noexcept;
void deleteProgram(gl::GLuint handle) noexcept;
void deleteVBO(gl::GLuint handle) noexcept;
void deleteVAO(gl::GLuint handle) noexcept;
void deleteTexture(gl::GLuint handle) noexcept;

using ShaderObject = GLObject<deleteShader>;
using ProgramObject = GLObject<deleteProgram>;
using VBO = GLObject<deleteVBO>;
using VAO = GLObject<deleteVAO>;
using TextureObject = GLObject<deleteTexture>;

} // namespace glcore
