#include "stdafx.h"
#include "MeshP3N3T2.h"
#include <cassert>
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

glm::vec3 getPositionOnSphere(float u, float v)
{
	const float radius = 1.f;
	const float latitude = float(M_PI) * (1.f - v); // 𝝅∙(𝟎.𝟓-𝒗)
	const float longitude = float(2.0 * M_PI) * u; // 𝟐𝝅∙𝒖
	const float latitudeRadius = radius * sinf(latitude);

	return {
		cosf(longitude) * latitudeRadius,
		cosf(latitude) * radius,
		sinf(longitude) * latitudeRadius
	};
}

void calculateTriangleStripIndicies(MeshDataP3N3T2 &data, unsigned columnCount, unsigned rowCount)
{
	data.indicies.clear();
	data.indicies.reserve((columnCount - 1) * rowCount * 2);
	// вычисляем индексы вершин.
	for (unsigned ci = 0; ci < columnCount - 1; ++ci)
	{
		if (ci % 2 == 0)
		{
			for (unsigned ri = 0; ri < rowCount; ++ri)
			{
				unsigned index = ci * rowCount + ri;
				data.indicies.push_back(index + rowCount);
				data.indicies.push_back(index);
			}
		}
		else
		{
			for (unsigned ri = rowCount - 1; ri < rowCount; --ri)
			{
				unsigned index = ci * rowCount + ri;
				data.indicies.push_back(index);
				data.indicies.push_back(index + rowCount);
			}
		}
	}
}
} // namespace

MeshDataP3N3T2 tesselateSphere(const MaterialPtr &material, unsigned latitudeDivisions, unsigned longitudeDivisions)
{
	constexpr unsigned kMinPrecision = 4;
	assert((latitudeDivisions >= kMinPrecision) && (longitudeDivisions >= kMinPrecision));
	assert(material != nullptr);

	MeshDataP3N3T2 data;

	// Для сферы мы используем примитив "полоса треугольников".
	data.primitive = gl::GL_TRIANGLE_STRIP;
	data.material = material;

	data.vertexes.reserve(latitudeDivisions * longitudeDivisions);
	for (unsigned longI = 0; longI < longitudeDivisions; ++longI)
	{
		const float u = float(longI) / float(longitudeDivisions - 1);
		for (unsigned latI = 0; latI < latitudeDivisions; ++latI)
		{
			const float v = float(latI) / float(latitudeDivisions - 1);

			VertexP3N3T2 vertex;
			vertex.position = getPositionOnSphere(u, v);

			// Нормаль к сфере - это нормализованный вектор радиуса к данной точке
			// Поскольку координаты центра равны 0, координаты вектора радиуса
			// будут равны координатам вершины.
			// Благодаря радиусу, равному 1, нормализация не требуется.
			vertex.normal = vertex.position;

			// Исходные параметры UV являются текстурными координатами.
			vertex.textureUV = glm::vec2(u, v);

			data.vertexes.push_back(vertex);
		}
	}

	calculateTriangleStripIndicies(data, longitudeDivisions, latitudeDivisions);

	return data;
}

void MeshP3N3T2::init(const MeshDataP3N3T2 &data)
{
	assert(data.material != nullptr);

	m_primitive = data.primitive;
	m_material = data.material;
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

void MeshP3N3T2::bindAttributes(const IShaderProgram &program)
{
	// OpenGL должен получить байтовые смещения полей относительно структуры VertexP2C4.
	const void *posOffset = reinterpret_cast<void *>(offsetof(VertexP3N3T2, position));
	const void *normalOffset = reinterpret_cast<void *>(offsetof(VertexP3N3T2, normal));
	const void *textureUVOffset = reinterpret_cast<void *>(offsetof(VertexP3N3T2, textureUV));
	const size_t stride = sizeof(VertexP3N3T2);

	// Выполнять привязку нужно в контексте VAO.
	glBindVertexArray(m_vao);

	// Привязываем атрибут позиции к данным в вершинном буфере.
	if (int location = program.getAttribute(AttributePosition); location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, glm::vec3().length(), GL_FLOAT, GL_FALSE, stride, posOffset);
	}

	// Привязываем атрибут нормали к данным в вершинном буфере.
	if (int location = program.getAttribute(AttributeNormal); location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, glm::vec3().length(), GL_FLOAT, GL_FALSE, stride, normalOffset);
	}

	// Привязываем атрибут текстурных координат UV  к данным в вершинном буфере.
	if (int location = program.getAttribute(AttributeTexCoord); location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, glm::vec2().length(), GL_FLOAT, GL_FALSE, stride, textureUVOffset);
	}
}

void MeshP3N3T2::updateUniforms(const IShaderProgram &program)
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

	// переключаемся на текстурный слот #2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_material->specularMap);
	// переключаемся на текстурный слот #1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_material->detailMap);
	// переключаемся обратно на текстурный слот #0
	// перед началом рендеринга активным будет именно этот слот.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_material->colorMap);

	if (int location = program.getUniform(UniformColorMap); location != -1)
	{
		// ColorMap привязана к текстурному слоту #0
		glUniform1i(location, 0);
	}
	if (int location = program.getUniform(UniformDetailsMap); location != -1)
	{
		// DetailMap привязана к текстурному слоту #1
		glUniform1i(location, 1);
	}
	if (int location = program.getUniform(UniformSpecularMap); location != -1)
	{
		// SpecularMap привязана к текстурному слоту #2
		glUniform1i(location, 2);
	}
}

void MeshP3N3T2::draw()
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
	glDrawRangeElements(m_primitive, minIndex, maxIndex, indexCount, GL_UNSIGNED_INT, pointer);
}

const Transform3D &MeshP3N3T2::getTransform() const
{
	return m_transform;
}

void MeshP3N3T2::setTransform(const Transform3D &transform)
{
	m_transform = transform;
}
