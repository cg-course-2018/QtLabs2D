#include "IsoSurface.h"
#include <algorithm>
#include <glm/geometric.hpp>
#include <numeric>

/*
 * Реализация алгоритма Marching Tetrahedra.
 * Основано на https://github.com/wilkinsona/marching-tetrahedra
 * Использовано на правах лицензии MIT, портировано на C++
 */

using namespace glm;

bool IsoVertex::operator==(const IsoVertex &other) const
{
	return id == other.id;
}

std::size_t IsoEdge::hash() const
{
	std::hash<int> getHash;
	return getHash(vertex1.id) ^ getHash(vertex2.id);
}

bool IsoEdge::operator==(const IsoEdge &other) const
{
	return std::tie(vertex1, vertex2) == std::tie(other.vertex1, other.vertex2);
}

IsoCube::IsoCube(
	const IsoVertex &v1, const IsoVertex &v2, const IsoVertex &v3, const IsoVertex &v4, const IsoVertex &v5,
	const IsoVertex &v6, const IsoVertex &v7, const IsoVertex &v8)
	: m_vertexes{ { v1, v2, v3, v4, v5, v6, v7, v8 } }
{
}

bool IsoCube::isIntersected(float threshold) const
{
	unsigned overThreshold = 0;
	for (unsigned i = 0; i < m_vertexes.size(); ++i)
	{
		const IsoVertex &v = m_vertexes[i];
		if (v.fieldStrength > threshold)
		{
			++overThreshold;
		}
		if (overThreshold > 0 && overThreshold <= i)
		{
			return true;
		}
	}
	return false;
}

std::vector<IsoTetrahedron> IsoCube::subdivide()
{
	return {
		IsoTetrahedron{ m_vertexes[7], m_vertexes[3], m_vertexes[2], m_vertexes[0] },
		IsoTetrahedron{ m_vertexes[0], m_vertexes[7], m_vertexes[6], m_vertexes[2] },
		IsoTetrahedron{ m_vertexes[7], m_vertexes[0], m_vertexes[6], m_vertexes[4] },
		IsoTetrahedron{ m_vertexes[0], m_vertexes[1], m_vertexes[2], m_vertexes[6] },
		IsoTetrahedron{ m_vertexes[4], m_vertexes[1], m_vertexes[0], m_vertexes[6] },
		IsoTetrahedron{ m_vertexes[5], m_vertexes[1], m_vertexes[4], m_vertexes[6] }
	};
}

IsoTetrahedron::IsoTetrahedron(const IsoVertex &v1, const IsoVertex &v2, const IsoVertex &v3, const IsoVertex &v4)
	: m_vertexes{ { v1, v2, v3, v4 } }
{
}

std::vector<IsoIntersection> IsoTetrahedron::getIntersectionsWithThreshold(float threshold) const
{
	std::vector<IsoIntersection> intersections;
	unsigned intersectionType = 0;
	unsigned bit = 1;
	for (const IsoVertex &v : m_vertexes)
	{
		if (v.fieldStrength >= threshold)
		{
			intersectionType |= bit;
		}
		bit <<= 1;
	}
	switch (intersectionType)
	{
	case 0:
		// Тетраэдр полностью снаружи поля, нет пересечений с метасферой.
		break;
	case 16:
		// Тетраэдр полностью внутри поля, нет пересечений с метасферой.
		break;
	case 1:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[1] },
			IsoEdge{ m_vertexes[0], m_vertexes[2] },
			IsoEdge{ m_vertexes[0], m_vertexes[3] } });
		break;
	case 2:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[1], m_vertexes[0] },
			IsoEdge{ m_vertexes[1], m_vertexes[3] },
			IsoEdge{ m_vertexes[1], m_vertexes[2] } });
		break;
	case 3:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[1], m_vertexes[3] },
			IsoEdge{ m_vertexes[0], m_vertexes[2] },
			IsoEdge{ m_vertexes[0], m_vertexes[3] } });
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[1], m_vertexes[3] },
			IsoEdge{ m_vertexes[1], m_vertexes[2] },
			IsoEdge{ m_vertexes[0], m_vertexes[2] } });
		break;
	case 4:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[2], m_vertexes[0] },
			IsoEdge{ m_vertexes[2], m_vertexes[1] },
			IsoEdge{ m_vertexes[2], m_vertexes[3] } });
		break;
	case 5:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[1] },
			IsoEdge{ m_vertexes[2], m_vertexes[3] },
			IsoEdge{ m_vertexes[0], m_vertexes[3] } });
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[1] },
			IsoEdge{ m_vertexes[1], m_vertexes[2] },
			IsoEdge{ m_vertexes[2], m_vertexes[3] } });
		break;
	case 6:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[1] },
			IsoEdge{ m_vertexes[1], m_vertexes[3] },
			IsoEdge{ m_vertexes[2], m_vertexes[3] } });
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[2], m_vertexes[3] },
			IsoEdge{ m_vertexes[0], m_vertexes[2] },
			IsoEdge{ m_vertexes[0], m_vertexes[1] } });
		break;
	case 7:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[3], m_vertexes[1] },
			IsoEdge{ m_vertexes[3], m_vertexes[2] },
			IsoEdge{ m_vertexes[3], m_vertexes[0] } });
		break;
	case 8:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[3], m_vertexes[0] },
			IsoEdge{ m_vertexes[3], m_vertexes[2] },
			IsoEdge{ m_vertexes[3], m_vertexes[1] } });
		break;
	case 9:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[2], m_vertexes[3] },
			IsoEdge{ m_vertexes[1], m_vertexes[3] },
			IsoEdge{ m_vertexes[0], m_vertexes[1] } });
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[1] },
			IsoEdge{ m_vertexes[0], m_vertexes[2] },
			IsoEdge{ m_vertexes[2], m_vertexes[3] } });
		break;
	case 10:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[3] },
			IsoEdge{ m_vertexes[2], m_vertexes[3] },
			IsoEdge{ m_vertexes[0], m_vertexes[1] } });
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[2], m_vertexes[3] },
			IsoEdge{ m_vertexes[1], m_vertexes[2] },
			IsoEdge{ m_vertexes[0], m_vertexes[1] } });
		break;
	case 11:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[2], m_vertexes[3] },
			IsoEdge{ m_vertexes[2], m_vertexes[1] },
			IsoEdge{ m_vertexes[2], m_vertexes[0] } });
		break;
	case 12:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[3] },
			IsoEdge{ m_vertexes[0], m_vertexes[2] },
			IsoEdge{ m_vertexes[1], m_vertexes[3] } });
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[2] },
			IsoEdge{ m_vertexes[1], m_vertexes[2] },
			IsoEdge{ m_vertexes[1], m_vertexes[3] } });
		break;
	case 13:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[1], m_vertexes[2] },
			IsoEdge{ m_vertexes[1], m_vertexes[3] },
			IsoEdge{ m_vertexes[1], m_vertexes[0] } });
		break;
	case 14:
		intersections.push_back(IsoIntersection{
			IsoEdge{ m_vertexes[0], m_vertexes[3] },
			IsoEdge{ m_vertexes[0], m_vertexes[2] },
			IsoEdge{ m_vertexes[0], m_vertexes[1] } });
		break;
	}

	return intersections;
}

void IsoSurface::setSize(const uvec3 &value)
{
	m_size = value;
}

void IsoSurface::setCubeSize(float value)
{
	m_cubeSize = value;
}

void IsoSurface::setSources(std::vector<IIsoSurfaceSourcePtr> sources)
{
	m_sources = std::move(sources);
}

MeshDataP3N3 IsoSurface::createGeometry(const Material &material)
{
	m_points.clear();
	m_pointIndexByEdge.clear();
	m_indexes.clear();

	for (auto &&cube : createCubes())
	{
		if (cube.isIntersected(m_threshold))
		{
			for (auto &&tetrahedron : cube.subdivide())
			{
				for (auto &&intersection : tetrahedron.getIntersectionsWithThreshold(m_threshold))
				{
					addPolygonForIntersection(intersection);
				}
			}
		}
	}

	calculateNormalsForPoints();

	std::vector<VertexP3N3> vertexes(m_points.size());
	std::transform(m_points.begin(), m_points.end(), vertexes.begin(), [](const IsoPoint &p) {
		vec3 combinedNormal{ 0 };
		for (const vec3 &normal : p.normals)
		{
			combinedNormal += normal;
		}
		const vec3 averagedNormal = combinedNormal / static_cast<float>(p.normals.size());
		return VertexP3N3{ p.position, averagedNormal };
	});

	MeshDataP3N3 result;
	result.material = material;
	result.primitive = gl::GL_TRIANGLES;
	result.vertexes = vertexes;
	result.indicies = m_indexes;

	return result;
}

std::vector<IsoCube> IsoSurface::createCubes()
{
	const vec3 origin = -0.5f * m_cubeSize * vec3(m_size);
	std::vector<IsoVertex> vertexes;
	unsigned idCounter = 0;
	glm::vec3 position{ 0 };
	for (unsigned indexY = 0; indexY <= m_size.y; ++indexY)
	{
		position.y = origin.y + static_cast<float>(indexY) * m_cubeSize;
		for (unsigned indexZ = 0; indexZ <= m_size.z; ++indexZ)
		{
			position.z = origin.z + static_cast<float>(indexZ) * m_cubeSize;
			for (unsigned indexX = 0; indexX <= m_size.x; ++indexX)
			{
				position.x = origin.x + static_cast<float>(indexX) * m_cubeSize;
				float strength = getFieldStrengthAtPosition(position);
				vertexes.push_back(IsoVertex{ position, strength, ++idCounter });
			}
		}
	}

	std::vector<IsoCube> cubes;
	for (unsigned indexY = 0; indexY < m_size.y; ++indexY)
	{
		for (unsigned indexZ = 0; indexZ < m_size.z; ++indexZ)
		{
			for (unsigned indexX = 0; indexX < m_size.x; ++indexX)
			{
				unsigned xLow = indexX;
				unsigned xHigh = indexX + 1;
				unsigned zLow = indexZ * (m_size.x + 1);
				unsigned zHigh = (indexZ + 1) * (m_size.x + 1);
				unsigned yLow = indexY * (m_size.x + 1) * (m_size.y + 1);
				unsigned yHigh = (indexY + 1) * (m_size.x + 1) * (m_size.y + 1);
				cubes.push_back(IsoCube{
					vertexes[xLow + yLow + zLow],
					vertexes[xHigh + yLow + zLow],
					vertexes[xHigh + yLow + zHigh],
					vertexes[xLow + yLow + zHigh],
					vertexes[xLow + yHigh + zLow],
					vertexes[xHigh + yHigh + zLow],
					vertexes[xHigh + yHigh + zHigh],
					vertexes[xLow + yHigh + zHigh] });
			}
		}
	}

	return cubes;
}

void IsoSurface::addPolygonForIntersection(const IsoIntersection &intersection)
{
	unsigned index1 = calculatePointForIntersectedEdge(intersection.edge1);
	unsigned index2 = calculatePointForIntersectedEdge(intersection.edge2);
	unsigned index3 = calculatePointForIntersectedEdge(intersection.edge3);
	m_indexes.push_back(index1);
	m_indexes.push_back(index2);
	m_indexes.push_back(index3);
}

void IsoSurface::calculateNormalsForPoints()
{
	for (size_t i = 0; i + 2 < m_indexes.size(); i += 3)
	{
		IsoPoint &p1 = m_points[m_indexes[i]];
		IsoPoint &p2 = m_points[m_indexes[i + 1]];
		IsoPoint &p3 = m_points[m_indexes[i + 2]];
		const vec3 edge1 = p1.position - p2.position;
		const vec3 edge2 = p1.position - p3.position;
		const vec3 normal = -normalize(cross(edge1, edge2));
		p1.normals.push_back(normal);
		p2.normals.push_back(normal);
		p3.normals.push_back(normal);
	}
}

float IsoSurface::getFieldStrengthAtPosition(const vec3 &position) const
{
	float strength = 0;
	for (auto &&source : m_sources)
	{
		strength += source->getStrengthAtPosition(position);
	}
	return strength;
}

unsigned IsoSurface::calculatePointForIntersectedEdge(const IsoEdge &edge)
{
	auto it = m_pointIndexByEdge.find(edge);
	if (it == m_pointIndexByEdge.end())
	{
		// Если точка для этого ребра ещё не была вычислена, то вычисляем её..
		vec3 pointForEdge = getPointAtThresholdOnEdge(edge);
		m_points.push_back(IsoPoint{ pointForEdge, {} });
		unsigned index = static_cast<unsigned>(m_points.size() - 1);
		m_pointIndexByEdge[edge] = index;

		return index;
	}

	return it->second;
}

vec3 IsoSurface::getPointAtThresholdOnEdge(const IsoEdge &edge) const
{
	vec3 lowPoint{ 0 };
	vec3 highPoint{ 0 };
	if (edge.vertex1.fieldStrength < edge.vertex2.fieldStrength)
	{
		lowPoint = edge.vertex1.position;
		highPoint = edge.vertex1.position;
	}
	else
	{
		lowPoint = edge.vertex2.position;
		highPoint = edge.vertex1.position;
	}

	return getPointAtThresholdBetween(lowPoint, highPoint);
}

vec3 IsoSurface::getPointAtThresholdBetween(const vec3 &lowPoint, const vec3 &highPoint) const
{
	const vec3 midpoint = lowPoint + (highPoint - lowPoint) / 2.0f;
	const float strength = getFieldStrengthAtPosition(midpoint);
	const float delta = strength - m_threshold;

	// TODO: sergey.shambir - jumpedOver добавлено для отладки, убрать или оставить?
	const bool found = std::abs(delta) < m_accuracy;
	const bool jumpOver = glm::distance(lowPoint, highPoint) < m_accuracy * m_cubeSize;
	if (found || jumpOver)
	{
		return midpoint;
	}
	if (delta < 0)
	{
		return getPointAtThresholdBetween(midpoint, highPoint);
	}
	return getPointAtThresholdBetween(lowPoint, midpoint);
}
