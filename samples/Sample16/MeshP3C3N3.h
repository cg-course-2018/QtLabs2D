#pragma once

#include "IShaderProgram.h"
#include "SceneGraph.h"
#include "libglcore/libglcore.h"
#include "libmath/Transform3D.h"
#include <glbinding/gl/enum.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// Структура, содержащая 6 чисел float, описывающих позицию вершины и нормаль к поверхности в данной вершине
#pragma pack(push, 1)
struct VertexP3C3N3
{
	glm::vec3 position{ 0 }; //< x, y, z
	glm::vec3 color{ 1, 1, 1 }; //< r, g, b
	glm::vec3 normal{ 0 }; //< x, y, z
};
#pragma pack(pop)

// Представляет компоненты цвета материала (зависящие от компоненты освещения).
struct Material
{
	// Собственное свечение материала.
	glm::vec4 emission{ 0 };
	// Цвет рассеивающего излучения.
	glm::vec4 diffuse{ 0 };
	// Цвет отражающих бликов.
	glm::vec4 specular{ 0 };
};

struct MeshDataP3C3N3
{
	Material material;
	std::vector<VertexP3C3N3> vertexes;
	std::vector<uint32_t> indicies;
	gl::GLenum primitive = gl::GL_TRIANGLES;
};

class MeshP3C3N3 : public SceneGraphNode
{
public:
    void init(const MeshDataP3C3N3 &data);

	void draw(const RenderContext& ctx) final;

private:
	void bindAttributes(const IShaderProgram &program);
	void updateUniforms(const RenderContext& ctx);
	void drawGeometry();

	glcore::VBO m_vertexes;
	glcore::VBO m_indicies;
	size_t m_maxIndex = 0;
	size_t m_indexCount = 0;
	gl::GLenum m_primitive = gl::GL_TRIANGLES;
	Material m_material;
};
