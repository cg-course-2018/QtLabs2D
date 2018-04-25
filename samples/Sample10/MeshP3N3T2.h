#pragma once

#include "IShaderProgram.h"
#include "libglcore/libglcore.h"
#include "libmath/Transform3D.h"
#include <glbinding/gl/enum.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

// —труктура, содержаща€ 6 чисел float, описывающих позицию вершины и нормаль к поверхности в данной вершине
#pragma pack(push, 1)
struct VertexP3N3T2
{
	glm::vec3 position; //< x, y, z
	glm::vec3 normal; //< x, y, z
	glm::vec2 textureUV; //< u, v
};
#pragma pack(pop)

// ѕредставл€ет компоненты цвета материала (завис€щие от компоненты освещени€).
struct Material
{
	glcore::TextureObject colorMap;
	glcore::TextureObject detailMap;
	glcore::TextureObject specularMap;
};

using MaterialPtr = std::shared_ptr<Material>;

struct MeshDataP3N3T2
{
	MaterialPtr material;
	std::vector<VertexP3N3T2> vertexes;
	std::vector<uint32_t> indicies;
	gl::GLenum primitive = gl::GL_TRIANGLES;
};

// √енерирует список вершин отдельных треугольников, формирующих поверхность сферы.
// @param latitudeDivisions - число делений по широте, не менее 4
// @param longitudeDivisions - число делений по долготе, не менее 4
MeshDataP3N3T2 tesselateSphere(const MaterialPtr &material, unsigned latitudeDivisions, unsigned longitudeDivisions);

class MeshP3N3T2
{
public:
	void init(const MeshDataP3N3T2 &data);

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
	MaterialPtr m_material;
};
