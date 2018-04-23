#include "stdafx.h"
#include "MeshP3N3.h"
#include <glbinding/gl32core/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl32core;
using namespace glm;

namespace
{
// Вершины куба служат материалом для формирования треугольников,
//  составляющих грани куба.
const glm::vec3 kCubeVerticies[] = {
	{ -1, +1, -1 },
	{ +1, +1, -1 },
	{ +1, -1, -1 },
	{ -1, -1, -1 },
	{ -1, +1, +1 },
	{ +1, +1, +1 },
	{ +1, -1, +1 },
	{ -1, -1, +1 },
};

// Индексы вершин задают косвенный способ получения вершины для
//  каждого из треугольников.
const glm::uvec3 kCubeIndexes[] = {
	{ 0, 1, 2 },
	{ 0, 2, 3 },
	{ 2, 1, 5 },
	{ 2, 5, 6 },
	{ 3, 2, 6 },
	{ 3, 6, 7 },
	{ 0, 3, 7 },
	{ 0, 7, 4 },
	{ 1, 0, 4 },
	{ 1, 4, 5 },
	{ 6, 5, 4 },
	{ 6, 4, 7 },
};

glm::mat4 getNormalMatrix(const glm::mat4 &modelView)
{
	return glm::transpose(glm::inverse(modelView));
}
} // namespace

MeshDataP3N3 tesselateCube(const Transform3D &transform)
{
	MeshDataP3N3 data;

	// Каждая вершина будет использоваться только в одном треугольнике, т.к. у всех граней разные нормали.
	// Поэтому мы просто заполняем массив индексов последовательностью 0, 1, ...
	// Фактически для куба буфер индексов нами не используется.
	data.indicies.resize(3 * std::size(kCubeIndexes));
	std::iota(data.indicies.begin(), data.indicies.end(), 0);

	// Создаём новые вершины, соединяя базовые координаты вершины куба и нормаль к грани куба.
	// Таким образом, одна вершина куба будет иметь три дубликата в вершинном буфере
	//  из-за того, что у всех граней куба разные нормали.
	data.vertexes.reserve(3 * std::size(kCubeIndexes));

	// Параметр transform содержит аффинное преобразование, которое надо применить ко всем вершинам.
	// Анонимная функция transformPoint будет применять преобразование к одной точке.
	const mat4 transformMat = transform.toMat4();
	const auto transformPoint = [&](const vec3 &point) {
		const vec4 transformed = transformMat * vec4(point, 1.f);
		return vec3{ transformed.x, transformed.y, transformed.z };
	};

	for (const auto &triangleIndexes : kCubeIndexes)
	{
		// Выбираем три точки из палитры вершин куба.
		const vec3 p1 = transformPoint(kCubeVerticies[triangleIndexes.x]);
		const vec3 p2 = transformPoint(kCubeVerticies[triangleIndexes.y]);
		const vec3 p3 = transformPoint(kCubeVerticies[triangleIndexes.z]);

		// Нормаль к треугольнику можно найти, используя векторное произведение двух сторон треугольника.
		const vec3 n = normalize(cross(p3 - p2, p1 - p2));

		// Дубликаты вершин помещаются в конец массива вершин.
		data.vertexes.push_back(VertexP3N3{ p1, n });
		data.vertexes.push_back(VertexP3N3{ p2, n });
		data.vertexes.push_back(VertexP3N3{ p3, n });
	}

	return data;
}

void MeshP3N3::init(const MeshDataP3N3 &data)
{
	m_maxIndex = data.vertexes.size();
	m_indexCount = data.indicies.size();

	if (!m_vao)
	{
		m_vao = glcore::createVAO();
	}
	glBindVertexArray(m_vao);

	m_vertexes = glcore::createStaticVBO(GL_ARRAY_BUFFER, data.vertexes);
	m_indicies = glcore::createStaticVBO(GL_ELEMENT_ARRAY_BUFFER, data.indicies);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexes);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicies);
}

void MeshP3N3::bindAttributes(const IShaderProgram &program)
{
	// OpenGL должен получить байтовые смещения полей относительно структуры VertexP2C4.
	const void *posOffset = reinterpret_cast<void *>(offsetof(VertexP3N3, position));
	const void *normalOffset = reinterpret_cast<void *>(offsetof(VertexP3N3, normal));
	const size_t stride = sizeof(VertexP3N3);

	// Выполнять привязку нужно в контексте VAO.
	glBindVertexArray(m_vao);

	// Привязываем атрибут нормали к данным в вершинном буфере.
	if (int location = program.getAttribute(AttributeNormal); location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, glm::vec3().length(), GL_FLOAT, GL_FALSE, stride, normalOffset);
	}

	// Привязываем атрибут позиции к данным в вершинном буфере.
	if (int location = program.getAttribute(AttributePosition); location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, glm::vec3().length(), GL_FLOAT, GL_FALSE, stride, posOffset);
	}
}

void MeshP3N3::updateUniforms(const IShaderProgram &program)
{
	const glm::mat4 transformMat = m_transform.toMat4();
	const glm::mat4 normalMat = getNormalMatrix(transformMat);
	if (int location = program.getUniform(UniformWorldMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transformMat));
	}
	if (int location = program.getUniform(UniformNormalWorldMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(normalMat));
	}
}

void MeshP3N3::draw()
{
	const GLuint minIndex = 0;
	const GLuint maxIndex = static_cast<GLuint>(m_maxIndex);
	const GLsizei indexCount = static_cast<GLsizei>(m_indexCount);
	constexpr size_t firstIndexOffset = 0;

	// Выполнять рисование нужно в контексте VAO.
	glBindVertexArray(m_vao);

	// Указатель используется вместо целочисленной переменной
	//  по историческим причинам: ранее семейство функций glDrawElements
	//  принимало указатель на область оперативной памяти,
	//  а в современном OpenGL принимает смещение от начала буфера в видеопамяти.
	const auto *pointer = reinterpret_cast<const void *>(firstIndexOffset);

	// glDrawRangeElements эффективнее, чем glDrawElements — ему передаются
	//  максимальный и минимальный индекс вершины, что позволяет видеодрайверу
	//  заранее определить размеры области памяти с вершинными данными.
	glDrawRangeElements(GL_TRIANGLES, minIndex, maxIndex, indexCount, GL_UNSIGNED_INT, pointer);
}

const Transform3D &MeshP3N3::getTransform() const
{
	return m_transform;
}

void MeshP3N3::setTransform(const Transform3D &transform)
{
	m_transform = transform;
}
