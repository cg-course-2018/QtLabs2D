#pragma once

#include "IShaderProgram.h"
#include "libglcore/libglcore.h"
#include "libmath/Transform3D.h"
#include <glbinding/gl/enum.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

// ���������, ���������� 6 ����� float, ����������� ������� ������� � ������� � ����������� � ������ �������
#pragma pack(push, 1)
struct VertexP3N3
{
	glm::vec3 position; //< x, y, z
	glm::vec3 normal; //< x, y, z
};
#pragma pack(pop)

// ������������ ���������� ����� ��������� (��������� �� ���������� ���������).
struct Material
{
	// ����������� �������� ���������.
	glm::vec4 emission;
	// ���� ������������� ���������.
	glm::vec4 diffuse;
	// ���� ���������� ������.
	glm::vec4 specular;
};

struct MeshDataP3N3
{
	Material material;
	std::vector<VertexP3N3> vertexes;
	std::vector<uint32_t> indicies;
	gl::GLenum primitive = gl::GL_TRIANGLES;
};

// ���������� ������ ������ ��������� �������������, ����������� ����������� ����.
MeshDataP3N3 tesselateCube(const Material &material);

// ���������� ������ ������ ��������� �������������, ����������� ����������� �����.
// @param latitudePrecision - ����� ������� �� ������, �� ����� 4
// @param longitudePrecision - ����� ������� �� �������, �� ����� 4
MeshDataP3N3 tesselateSphere(const Material &material, unsigned latitudePrecision, unsigned longitudePrecision);

class MeshP3N3
{
public:
	void init(const MeshDataP3N3 &data);

	void bindAttributes(const IShaderProgram &program);
	void updateUniforms(const IShaderProgram &program);

	void draw();

	const Transform3D &getTransform() const;
	void setTransform(const Transform3D &transform);

private:
	Transform3D m_transform;
	glcore::VAO m_vao;
	glcore::VBO m_vertexes;
	glcore::VBO m_indicies;
	size_t m_maxIndex = 0;
	size_t m_indexCount = 0;
	gl::GLenum m_primitive = gl::GL_TRIANGLES;
	Material m_material;
};
