#include "ObjectUtils.h"
#include <glbinding/gl32core/gl.h>

using namespace gl32core;

glcore::VBO glcore::createVBO()
{
	GLuint handle = 0;
	glGenBuffers(1, &handle);
	if (handle == 0)
	{
		throw std::runtime_error("cannot create VBO: no enough memory");
	}

	return VBO(handle);
}

glcore::VAO glcore::createVAO()
{
	GLuint handle = 0;
	glGenVertexArrays(1, &handle);
	if (handle == 0)
	{
		throw std::runtime_error("cannot create VAO: no enough memory");
	}
	return VAO(handle);
}

glcore::VBO glcore::createStaticVBO(gl::GLenum target, const void *bytes, const size_t byteCount)
{
	auto vbo = createVBO();

	glBindBuffer(target, vbo);
	glBufferData(target, byteCount, bytes, GL_STATIC_DRAW);

	return vbo;
}

void glcore::setStreamBufferData(gl::GLuint buffer, gl::GLenum target, const void *bytes, const size_t byteCount)
{
	glBindBuffer(target, buffer);
	glBufferData(target, byteCount, bytes, GL_STREAM_DRAW);
}
