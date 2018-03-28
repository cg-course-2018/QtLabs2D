#include "OpenGL_raii.h"
#include <glbinding/gl32core/gl.h>

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl32core;

void glcore::deleteShader(gl::GLuint handle) noexcept
{
	glDeleteShader(handle);
}

void glcore::deleteProgram(gl::GLuint handle) noexcept
{
	glDeleteProgram(handle);
}

void glcore::deleteVBO(gl::GLuint handle) noexcept
{
	glDeleteBuffers(1, &handle);
}

void glcore::deleteVAO(gl::GLuint handle) noexcept
{
	glDeleteVertexArrays(1, &handle);
}

void glcore::deleteTexture(gl::GLuint handle) noexcept
{
	glDeleteTextures(1, &handle);
}
