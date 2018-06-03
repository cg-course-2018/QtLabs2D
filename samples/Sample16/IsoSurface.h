#pragma once
#include "IsoSurfaceSources.h"
#include "MeshP3C3N3.h"
#include <array>
#include <unordered_map>
#include <vector>

struct IsoVertex
{
public:
	glm::vec3 position{ 0 };
	float fieldStrength{ 0 };
	unsigned id{ 0 };

	bool operator==(const IsoVertex &other) const;
};

struct IsoEdge
{
public:
	IsoVertex vertex1;
	IsoVertex vertex2;

	std::size_t hash() const;
	bool operator==(const IsoEdge &other) const;
};

namespace std
{
template<>
class hash<IsoEdge>
{
public:
	std::size_t operator()(IsoEdge const &key) const noexcept
	{
		return key.hash();
	}
};
} // namespace std

struct IsoIntersection
{
	IsoEdge edge1;
	IsoEdge edge2;
	IsoEdge edge3;
};

struct IsoPoint
{
	glm::vec3 position{ 0 };
	std::vector<glm::vec3> normals;
};

class IsoTetrahedron
{
public:
	explicit IsoTetrahedron(const IsoVertex &v1, const IsoVertex &v2, const IsoVertex &v3, const IsoVertex &v4);

	std::vector<IsoIntersection> getIntersectionsWithThreshold(float threshold) const;

private:
	std::array<IsoVertex, 4> m_vertexes;
};

class IsoCube
{
public:
	explicit IsoCube(
		const IsoVertex &v1, const IsoVertex &v2, const IsoVertex &v3, const IsoVertex &v4,
		const IsoVertex &v5, const IsoVertex &v6, const IsoVertex &v7, const IsoVertex &v8);

	// Возвращает true, если метасфера пересекает куб.
	// Это происходит, если потенциал поля больше treshold во всех вершинах куба
	//  либо меньше treshold во всех вершинах куба.
	bool isIntersected(float threshold) const;

	// Разделяет куб на тетраэдры.
	std::vector<IsoTetrahedron> subdivide();

private:
	std::array<IsoVertex, 8> m_vertexes;
};

// Класс отвечает за генерацию сетки вершин треугольников, формирующих метасферу,
//  т.е. изоповерхность поля.
class IsoSurface
{
public:
	// Задаёт размер триангулируемой области пространства в ячейках.
	void setSize(const glm::uvec3 &value);

	// Задаёт размер одной ячейки триангулируемой области пространства.
	void setCubeSize(float value);

	// Задаёт список источников силы поля, для которого вычисляется изоповерхность.
	void setSources(std::vector<IIsoSurfaceSourcePtr> sources);

	// Генерирует сетку вершин треугольников, формирующих метасферу.
	// @param material - материал для поверхности
	MeshDataP3C3N3 createGeometry(const Material &material);

private:
	std::vector<IsoCube> createCubes();
	void addPolygonForIntersection(const IsoIntersection &intersection);
	void calculateNormalsForPoints();
	float getFieldStrengthAtPosition(const glm::vec3 &position) const;

	// Получает точку пересечения ребра и изоповерхности, добавляя новую точку к массивам
	//  вершин и индексов либо переиспользуя существующую.
	unsigned calculatePointForIntersectedEdge(const IsoEdge &edge);

	// Функция вычисляет точку на ребре edge, находящуюся на изоповерхности,
	//  т.е. сила поля в вычисленной точке примерно равно threshold с точностью до accuracy.
	glm::vec3 getPointAtThresholdOnEdge(const IsoEdge &edge) const;

	// Функция рекурсивно вычисляет точку на отрезке [v1; v2], находящуюся на изоповерхности,
	//  т.е. сила поля в вычисленной точке примерно равно threshold с точностью до accuracy.
	// Параметры: lowPoint - точка с меньшей силой поля, highPoint - точка с большей силой поля.
	// Для нахождения точки используется дихотомия (бинарный поиск).
	glm::vec3 getPointAtThresholdBetween(const glm::vec3 &lowPoint, const glm::vec3 &highPoint) const;

	float m_accuracy{ 0.0001f };
	float m_threshold{ 0.5f };
	std::vector<IIsoSurfaceSourcePtr> m_sources;
	float m_cubeSize{ 0 };
	std::unordered_map<IsoEdge, int> m_pointIndexByEdge;
	std::vector<IsoPoint> m_points;
	std::vector<unsigned> m_indexes;

	// Размер визуализируемой области, в кубиках, (width, height, depth).
	glm::uvec3 m_size{ 0 };
};
