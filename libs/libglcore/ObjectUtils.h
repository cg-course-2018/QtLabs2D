#pragma once

#include "OpenGL_raii.h"
#include <string>
#include <vector>

namespace glcore
{

// Фасад функции glGenVertexArrays
// Создаёт VAO, хранящий связь буферов данных и атрибутов вершин.
VAO createVAO();

// Фасад функции glGenBuffers
// @param target - это GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER или GL_UNIFORM_BUFFER
VBO createVBO();

// Фасад функции glGenTextures
// Создаёт ресурс - буфер для хранения текстуры
TextureObject createTexture();

// Фасад функций glGenBuffers и glBufferData
// Передаёт на видеокарту заданные байты
// @param target - это GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER или GL_UNIFORM_BUFFER
VBO createStaticVBO(gl::GLenum target, const void *bytes, const size_t byteCount);

// Фасад функции glBufferData
// Передаёт на видеокарту заданные байты
// @param target - это GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER или GL_UNIFORM_BUFFER
void setStreamBufferData(gl::GLuint buffer, gl::GLenum target, const void *bytes, const size_t byteCount);

// Эта версия функции принимает данные в виде массива. Допустимы следующие типы:
// - T[], массив в стиле C
// - std::vector<T>
// - std::array<T>
// - std::initializer_list<T>, т.е. списки инициализации
template<class ArrayT>
VBO createStaticVBO(gl::GLenum target, const ArrayT &verticies)
{
	// VertexT - тип элемента массива
	using VertexT = std::decay_t<decltype(verticies[0])>;

	// Превращаем указатель на начало массива в указатель на байты массива.
	const auto bytes = reinterpret_cast<const void *>(std::data(verticies));
	// Вычисляем размер массива в байтах.
	const size_t byteCount = sizeof(VertexT) * std::size(verticies);

	return createStaticVBO(target, bytes, byteCount);
}

// Эта версия функции принимает данные в виде массива. Допустимы следующие типы:
// - T[], массив в стиле C
// - std::vector<T>
// - std::array<T>
// - std::initializer_list<T>, т.е. списки инициализации
template<class ArrayT>
void setStreamBufferData(gl::GLuint buffer, gl::GLenum target, const ArrayT &verticies)
{
	// VertexT - тип элемента массива
	using VertexT = std::decay_t<decltype(verticies[0])>;

	// Превращаем указатель на начало массива в указатель на байты массива.
	const auto bytes = reinterpret_cast<const void *>(std::data(verticies));
	// Вычисляем размер массива в байтах.
	const size_t byteCount = sizeof(VertexT) * std::size(verticies);

	setStreamBufferData(buffer, target, bytes, byteCount);
}

} // namespace glcore
