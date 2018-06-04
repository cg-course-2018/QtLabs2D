#include "stdafx.h"
#include "MeshP3C3N3.h"
#include <glbinding/gl32core/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

// Используем функции из gl32core, экспортированные библиотекой glbinding.
using namespace gl32core;
using namespace glm;

namespace
{
glm::mat4 getNormalMatrix(const glm::mat4 &modelView)
{
	return glm::transpose(glm::inverse(modelView));
}
} // namespace

void MeshP3C3N3::init(const MeshDataP3C3N3 &data)
{
	m_primitive = data.primitive;
	m_material = data.material;
	m_maxIndex = data.vertexes.size();
	m_indexCount = data.indicies.size();

	m_vertexes = glcore::createStaticVBO(GL_ARRAY_BUFFER, data.vertexes);
	m_indicies = glcore::createStaticVBO(GL_ELEMENT_ARRAY_BUFFER, data.indicies);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexes);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicies);
}

void MeshP3C3N3::draw(const RenderContext &ctx)
{
	updateUniforms(ctx);
	bindAttributes(ctx.program.get());
	drawGeometry();

	SceneGraphNode::draw(ctx);
}

void MeshP3C3N3::bindAttributes(const IShaderProgram &program)
{
	// OpenGL должен получить байтовые смещения полей относительно структуры VertexP2C4.
	const void *posOffset = reinterpret_cast<void *>(offsetof(VertexP3C3N3, position));
	const void *colorOffset = reinterpret_cast<void *>(offsetof(VertexP3C3N3, color));
	const void *normalOffset = reinterpret_cast<void *>(offsetof(VertexP3C3N3, normal));
	const size_t stride = sizeof(VertexP3C3N3);

	// Привязываем вершинные буферы к текущему состоянию OpenGL.
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexes);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicies);

	// Привязываем атрибут нормали к данным в вершинном буфере.
	if (int location = program.getAttribute(AttributeNormal); location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, glm::vec3().length(), GL_FLOAT, GL_FALSE, stride, normalOffset);
	}

	// Привязываем атрибут цвета к данным в вершинном буфере.
	if (int location = program.getAttribute(AttributeColor); location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, glm::vec3().length(), GL_FLOAT, GL_TRUE, stride, colorOffset);
	}

	// Привязываем атрибут позиции к данным в вершинном буфере.
	if (int location = program.getAttribute(AttributePosition); location != -1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, glm::vec3().length(), GL_FLOAT, GL_FALSE, stride, posOffset);
	}
}

void MeshP3C3N3::updateUniforms(const RenderContext &ctx)
{
	const auto &program = ctx.program.get();
	const glm::mat4 transformMat = ctx.parentWorldMat4 * getLocalTransform().toMat4();
	const glm::mat4 normalMat = getNormalMatrix(transformMat);

	if (int location = program.getUniform(UniformWorldMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transformMat));
	}
	if (int location = program.getUniform(UniformNormalWorldMatrix); location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(normalMat));
	}
	if (int location = program.getUniform(UniformMaterialEmission); location != -1)
	{
		glUniform4fv(location, 1, glm::value_ptr(m_material.emission));
	}
	if (int location = program.getUniform(UniformMaterialDiffuse); location != -1)
	{
		glUniform4fv(location, 1, glm::value_ptr(m_material.diffuse));
	}
	if (int location = program.getUniform(UniformMaterialSpecular); location != -1)
	{
		glUniform4fv(location, 1, glm::value_ptr(m_material.specular));
	}
}

void MeshP3C3N3::drawGeometry()
{
	const GLuint minIndex = 0;
	const GLuint maxIndex = static_cast<GLuint>(m_maxIndex);
	const GLsizei indexCount = static_cast<GLsizei>(m_indexCount);
	constexpr size_t firstIndexOffset = 0;

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
