#include "stdafx.h"
#include "MeshP3C3.h"
#include <glbinding/gl32core/gl.h>
#include <glm/gtc/type_ptr.hpp>

// ���������� ������� �� gl32core, ���������������� ����������� glbinding.
using namespace gl32core;

namespace
{
/*const glm::vec3 kYellowColor = { 1.0f, 1.0f, 0.2f };
const glm::vec3 kOrangeColor = { 1.0f, 0.5f, 0.1f };
const glm::vec3 kBlueColor = { 0.f, 0.4f, 0.6f };
const glm::vec3 kGreenColor = { 0.1f, 1.0f, 0.1f };
const glm::vec3 kDarkBlueColor = { 0.1f, 0.1f, 0.8f };
const glm::vec3 kVioletColor = { 0.3f, 0.0f, 0.3f };
const glm::vec3 kLightGrayColor = { 0.5f, 0.5f, 0.5f };
const glm::vec3 kBrownColor = { 0.62f, 0.32f, 0.18f };*/

	const glm::vec3 kYellowColor = { 0.f, 0.f, -1.0f };
	const glm::vec3 kOrangeColor = { std::sqrt(1.5f), 0.f, 0.5f };
	const glm::vec3 kBlueColor = { -std::sqrt(1.5f), 0.f, 0.5f };
	const glm::vec3 kGreenColor = { 0.f, std::sqrt(2.f), 0.f };

// ������� ���� ������ ���������� ��� ������������ �������������,
//  ������������ ����� ����.
/*const VertexP3C3 kCubeVerticies[] = {
	{ { -1, +1, -1 }, kYellowColor },
	{ { +1, +1, -1 }, kOrangeColor },
	{ { +1, -1, -1 }, kBlueColor },
	{ { -1, -1, -1 }, kGreenColor },
	{ { -1, +1, +1 }, kLightGrayColor },
	{ { +1, +1, +1 }, kDarkBlueColor },
	{ { +1, -1, +1 }, kVioletColor },
	{ { -1, -1, +1 }, kBrownColor },
};*/

	const VertexP3C3 kTetrahedronVerticies[] = {
		{ { 0.f, 0.f, -1.0f }, kYellowColor },
	{ { std::sqrt(1.5f), 0.f, 0.5f }, kOrangeColor },
	{ { -std::sqrt(1.5f), 0.f, 0.5f }, kBlueColor },
	{ { 0.f, std::sqrt(2.f), 0.f }, kGreenColor } };

// ������� ������ ������ ��������� ������ ��������� ������� ���
//  ������� �� �������������.
/*const glm::uvec3 kCubeIndexes[] = {
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
};*/

	const glm::uvec3 kTetrahedronIndexes[] = {
		{ 0, 1, 3 },
	{ 0, 2, 3 },
	{ 0, 1, 2 },
	{ 1, 2, 3 } };

} // namespace

MeshDataP3C3 tesselateCube(const Transform3D &transform)
{
	MeshDataP3C3 data;

	// �������� ��������� ������, ����� �������������� ��.
	data.vertexes.assign(std::begin(kTetrahedronVerticies), std::end(kTetrahedronVerticies));
	const glm::mat4 transformMat = transform.toMat4();
	for (auto &v : data.vertexes)
	{
		const glm::vec4 transformed = transformMat * glm::vec4(v.position, 1.f);
		v.position = { transformed.x, transformed.y, transformed.z };
	}

	// �������� ������� ������.
	data.indicies.reserve(3 * std::size(kTetrahedronVerticies));
	for (const auto &triangleIndexes : kTetrahedronIndexes)
	{
		data.indicies.push_back(triangleIndexes.x);
		data.indicies.push_back(triangleIndexes.y);
		data.indicies.push_back(triangleIndexes.z);
	}

	return data;
}

void MeshP3C3::init(const MeshDataP3C3 &data)
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

void MeshP3C3::bindAttributes(const glcore::ProgramObject &program, const std::string &positionAttrName, const std::string &colorAttrName)
{
	// OpenGL ������ �������� �������� �������� ����� ������������ ��������� VertexP2C4.
	const void *colorOffset = reinterpret_cast<void *>(offsetof(VertexP3C3, color));
	const void *posOffset = reinterpret_cast<void *>(offsetof(VertexP3C3, position));
	const size_t stride = sizeof(VertexP3C3);

	// ��������� �������� ����� � ��������� VAO.
	glBindVertexArray(m_vao);

	// ����������� ������� ����� � ������ � ��������� ������.
	{
		const int colorLocation = glGetAttribLocation(program, colorAttrName.data());
		glEnableVertexAttribArray(colorLocation);
		glVertexAttribPointer(colorLocation, glm::vec3().length(), GL_FLOAT, GL_TRUE, stride, colorOffset);
	}

	// ����������� ������� ������� � ������ � ��������� ������.
	{
		const int posLocation = glGetAttribLocation(program, positionAttrName.data());
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, glm::vec3().length(), GL_FLOAT, GL_FALSE, stride, posOffset);
	}
}

void MeshP3C3::updateUniforms(const glcore::ProgramObject &program, const std::string &worldMatrixUniformName)
{
	const glm::mat4 transformMat = m_transform.toMat4();
	glUniformMatrix4fv(glGetUniformLocation(program, worldMatrixUniformName.data()), 1, GL_FALSE, glm::value_ptr(transformMat));
}

void MeshP3C3::draw()
{
	const GLuint minIndex = 0;
	const GLuint maxIndex = static_cast<GLuint>(m_maxIndex);
	const GLsizei indexCount = static_cast<GLsizei>(m_indexCount);
	constexpr size_t firstIndexOffset = 0;

	// ��������� ��������� ����� � ��������� VAO.
	glBindVertexArray(m_vao);

	// ��������� ������������ ������ ������������� ����������
	//  �� ������������ ��������: ����� ��������� ������� glDrawElements
	//  ��������� ��������� �� ������� ����������� ������,
	//  � � ����������� OpenGL ��������� �������� �� ������ ������ � �����������.
	const auto *pointer = reinterpret_cast<const void *>(firstIndexOffset);

	// glDrawRangeElements �����������, ��� glDrawElements � ��� ����������
	//  ������������ � ����������� ������ �������, ��� ��������� �������������
	//  ������� ���������� ������� ������� ������ � ���������� �������.
	glDrawRangeElements(GL_TRIANGLES, minIndex, maxIndex, indexCount, GL_UNSIGNED_INT, pointer);
}

const Transform3D &MeshP3C3::getTransform() const
{
	return m_transform;
}

void MeshP3C3::setTransform(const Transform3D &transform)
{
	m_transform = transform;
}
