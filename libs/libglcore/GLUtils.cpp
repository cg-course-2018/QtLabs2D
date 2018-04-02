#include "GLUtils.h"
#include <glbinding/gl32core/gl.h>

#pragma warning(disable : 4251) // {symbol} needs to have dll-interface to be used by clients of class Binding.
#include <glbinding/Binding.h>

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl32core;

void glcore::validateOpenGLError()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::string message;
		switch (error)
		{
		case GL_INVALID_ENUM:
			message = "invalid enum passed to GL function (GL_INVALID_ENUM)";
			break;
		case GL_INVALID_VALUE:
			message = "invalid parameter passed to GL function (GL_INVALID_VALUE)";
			break;
		case GL_INVALID_OPERATION:
			message = "cannot execute some of GL functions in current state (GL_INVALID_OPERATION)";
			break;
		case GL_OUT_OF_MEMORY:
			message = "no enough memory to execute GL function (GL_OUT_OF_MEMORY)";
			break;
		default:
			message = "error with one of GL extensions (framebuffers, shaders, etc)";
			break;
		}
		throw std::runtime_error("OpenGL error: " + message);
	}
}

void glcore::initGLBinding()
{
	glbinding::Binding::initialize();
}
