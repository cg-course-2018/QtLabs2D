#include "stdafx.h"
#include "SetupDebugOutput.h"
#include <QtCore/QDebug>
#include <glbinding/Meta.h>
#include <glbinding/gl33ext/gl.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl33ext;

namespace
{
// Определяем callback для передачи видеодрайверу.
// См. https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDebugMessageCallback.xhtml
void debugOutputCallback(GLenum /*source*/,
	GLenum type,
	GLuint id,
	GLenum /*severity*/,
	GLsizei /*length*/,
	const GLchar *message,
	const void * /*userParam*/)
{
	// Отсекаем все сообщения, кроме ошибок
	if (type != GL_DEBUG_TYPE_ERROR_ARB)
	{
		return;
	}
	std::string formatted = "OpenGL error #" + std::to_string(id) + ": " + message;
	qDebug() << QString::fromUtf8(formatted.data());

#if defined(_MSC_VER)
	if (::IsDebuggerPresent())
	{
		__debugbreak();
	}
#endif
}
} // namespace

void platform::SetupDebugOutput()
{
	const std::set<GLextension> extensions = glbinding::Meta::extensions();

	if (extensions.find(GLextension::GL_ARB_debug_output) != extensions.end())
	{
		glEnable(GL_DEBUG_OUTPUT);

		// Синхронный режим позволяет узнать в отладчике контекст,
		//  в котором произошла ошибка.
		// Режим может понизить производительность, но на фоне
		//  других потерь Debug-сборки это несущественно.
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

		glDebugMessageCallbackARB(debugOutputCallback, nullptr);
		// Указываем видеодрайверу выдать только один тип сообщений,
		//  GL_DEBUG_TYPE_ERROR.
		glDebugMessageControlARB(gl::GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, gl::GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}
