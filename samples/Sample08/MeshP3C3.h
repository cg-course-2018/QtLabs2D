#pragma once

#include "libglcore/ObjectUtils.h"
#include "libmath/Transform3D.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

// Структура, содержащая 6 чисел float, описывающих позицию и цвет вершины: X, Y, Z, R, G, B
#pragma pack(push, 1)
struct VertexP3C3
{
	glm::vec3 position; //< x, y, z
	glm::vec3 color; //< red, green, blue
};
#pragma pack(pop)

struct MeshDataP3C3
{
	std::vector<VertexP3C3> vertexes;
	std::vector<uint32_t> indicies;
};

// Генерирует список вершин отдельных треугольников, формирующих поверхность куба.
MeshDataP3C3 tesselateCube(const Transform3D &transform = Transform3D());

class MeshP3C3
{
public:
	void init(const MeshDataP3C3 &data);

	void bindAttributes(const glcore::ProgramObject &program, const std::string &positionAttrName, const std::string &colorAttrName);
	void updateUniforms(const glcore::ProgramObject &program, const std::string &worldMatrixUniformName);

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
