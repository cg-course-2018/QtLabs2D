#pragma once

#include "IShaderProgram.h"
#include "libglcore/libglcore.h"
#include "libmath/Transform3D.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

// —труктура, содержаща€ 6 чисел float, описывающих позицию вершины и нормаль к поверхности в данной вершине
#pragma pack(push, 1)
struct VertexP3N3
{
	glm::vec3 position; //< x, y, z
	glm::vec3 normal; //< x, y, z
};
#pragma pack(pop)

struct MeshDataP3N3
{
	std::vector<VertexP3N3> vertexes;
	std::vector<uint32_t> indicies;
};

// √енерирует список вершин отдельных треугольников, формирующих поверхность куба.
MeshDataP3N3 tesselateCube(const Transform3D &transform = Transform3D());

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
};
