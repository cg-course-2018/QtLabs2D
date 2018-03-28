#pragma once
#include <cstddef>
#include <glbinding/gl/types.h>

namespace glcore
{
using DeleteFn = void(gl::GLuint handle);

// Класс GLObject - универсальный RAII для ресурсов OpenGL.
// Ресурс остаётся в "живых", пока "жив" объект GLObject.
template<DeleteFn deleteFn>
class GLObject
{
public:
	// Копирование объекта запрещено (можно только переместить).
	GLObject(const GLObject &other) = delete;
	GLObject &operator=(const GLObject &other) = delete;

	// По умолчанию конструируется с нулевым объектом
	GLObject() = default;

	// Можно конструировать из числового идентификатора объекта OpenGL
	explicit GLObject(gl::GLuint handle)
		: m_handle(handle)
	{
	}

	// Можно переместить объект (согласно move-семантике)
	GLObject(GLObject &&other) noexcept
	{
		std::swap(m_handle, other.m_handle);
	}

	// Можно переместить объект (согласно move-семантике)
	GLObject &operator=(GLObject &&other) noexcept
	{
		reset();
		std::swap(m_handle, other.m_handle);
		return *this;
	}

	// При уничтожении хранимый объект также удаляется
	~GLObject()
	{
		reset();
	}

	// reset удаляет хранимый объект, если он есть.
	void reset() noexcept
	{
		if (m_handle != 0)
		{
			deleteFn(m_handle);
			m_handle = 0;
		}
	}

	// Оператор приведения к типу даёт доступ к числовому идентификатору объекта OpenGL
	operator gl::GLuint() const noexcept
	{
		return m_handle;
	}

	// get даёт доступ к числовому идентификатору объекта OpenGL
	gl::GLuint get() const noexcept
	{
		return m_handle;
	}

	// при приведении к типу bool происходит проверка - хранится ли объект?
	explicit operator bool() const noexcept
	{
		return (m_handle != 0);
	}

private:
	gl::GLuint m_handle = 0;
};

} // namespace glcore
