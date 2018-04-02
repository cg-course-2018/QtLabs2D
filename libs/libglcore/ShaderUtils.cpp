#include "ShaderUtils.h"
#include <glbinding/gl32core/gl.h>

using namespace gl32core;

namespace glcore
{
namespace
{
std::string readShaderInfoLog(GLuint id)
{
	// При неудаче есть лог ошибок, который мы соберём
	// и в первую очередь надо узнать длину лога.
	GLint logLength = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

	// Зная длину, выделяем строку нужного размера и копируем в неё лог
	std::string log(logLength, '\0');
	if (logLength > 0)
	{
		GLsizei ignored = 0;
		glGetShaderInfoLog(id, static_cast<gl::GLsizei>(log.size()), &ignored, &log[0]);
	}

	return log;
}

std::string readProgramInfoLog(GLuint id)
{
	// При неудаче есть лог ошибок, который мы соберём
	// и в первую очередь надо узнать длину лога.
	GLint logLength = 0;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

	// Зная длину, выделяем строку нужного размера и копируем в неё лог
	std::string log(logLength, '\0');
	if (logLength > 0)
	{
		GLsizei ignored = 0;
		glGetProgramInfoLog(id, static_cast<gl::GLsizei>(log.size()), &ignored, &log[0]);
	}

	return log;
}

void checkShaderCompilationStatus(GLuint id)
{
	GLboolean ok = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
	if (ok == GL_FALSE)
	{
		// Бросаем исключение, прикрепив к нему лог
		const std::string log = readShaderInfoLog(id);
		throw std::runtime_error("shader compilation failed: " + log);
	}
}

void checkProgramLinkingStatus(GLuint id)
{
	GLboolean status = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Бросаем исключение, прикрепив к нему лог
		const std::string log = readProgramInfoLog(id);
		throw std::runtime_error("program linking failed " + log);
	}
}
} // anonymous namespace

ShaderObject compileShader(gl::GLenum type, const std::string &source)
{
	// Выделяем ресурс шейдера
	ShaderObject obj = ShaderObject(glCreateShader(type));

	// Передаём исходный код шейдера видеодрайверу
	const auto length = static_cast<int>(source.length());
	const char *sourceLine = source.data();
	glShaderSource(obj.get(), 1, (const GLchar **)&sourceLine, &length);

	// Просим видеодрайвер скомпилировать шейдер и проверяем статус
	glCompileShader(obj.get());
	checkShaderCompilationStatus(obj.get());

	return obj;
}

ProgramObject linkProgram(const std::vector<ShaderObject> &shaders)
{
	// Запрашиваем у видеодрайера новый объект.
	ProgramObject obj = ProgramObject(glCreateProgram());

	// Прикрепляем ранее скомпилированные шейдеры.
	for (const ShaderObject &shader : shaders)
	{
		glAttachShader(obj.get(), shader.get());
	}

	// Просим видеодрайвер выполнить компоновку и проверяем статус.
	glLinkProgram(obj.get());
	checkProgramLinkingStatus(obj.get());

	// Отсоединяем шейдеры, поскольку программа уже собрана.
	for (const ShaderObject &shader : shaders)
	{
		glDetachShader(obj.get(), shader.get());
	}

	return obj;
}

void validateProgram(const ProgramObject &program, std::ostream &out)
{
	// Просим видеодрайвер выполнить валидацию программы.
	// Видеодрайвер может указать на потенциальные ошибки или неоптимальный код.
	glValidateProgram(program.get());

	GLboolean status = 0;
	glGetProgramiv(program.get(), GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Записываем лог валидации в поток
		const std::string log = readProgramInfoLog(program.get());
		out << log;
	}
}

} // namespace glcore
