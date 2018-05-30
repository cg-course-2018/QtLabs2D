#pragma once
#include "MeshP3N3.h"
#include <array>
#include <glm/vec3.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

struct IsoVertex
{
public:
    glm::vec3 position{ 0 };
    float fieldStrength{ 0 };
    unsigned id{ 0 };

    bool operator==(const IsoVertex &other);
};

struct IsoEdge
{
public:
    IsoVertex vertex1;
    IsoVertex vertex2;

    std::size_t hash() const;
    bool operator==(const IsoEdge &other);
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

class IIsoSource
{
public:
    virtual ~IIsoSource() = default;
    virtual float getStrengthAtPosition(const glm::vec3 &position) const = 0;
};

using IIsoSourcePtr = std::unique_ptr<IIsoSource>;

class IsoPointSource : public IIsoSource
{
public:
    float getStrengthAtPosition(const glm::vec3 &position) const final;

private:
    glm::vec3 m_position{ 0 };
    float m_radius{ 0 };
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

    bool isIntersected(float threshold) const;
    std::vector<IsoTetrahedron> subdivide();

private:
    std::array<IsoVertex, 8> m_vertexes;
};

class IsoSurface
{
public:
    void setSize(const glm::uvec3 &value);
    void setCubeSize(float value);
    void setSources(std::vector<IIsoSourcePtr> &&sources);

    MeshDataP3N3 createGeometry(const Material& material);

private:
    std::vector<IsoCube> createCubes();
    void addPolygonForIntersection(const IsoIntersection& intersection);
    void calculateNormalsForPoints();
    float getFieldStrengthAtPosition(const glm::vec3& position) const;

    // Получает точку пересечения ребра и изоповерхности, добавляя новую точку к массивам
    //  вершин и индексов либо переиспользуя существующую.
    unsigned calculatePointForIntersectedEdge(const IsoEdge& edge);

    // Функция вычисляет точку на ребре edge, находящуюся на изоповерхности,
    //  т.е. сила поля в вычисленной точке примерно равно threshold с точностью до accuracy.
    glm::vec3 getPointAtThresholdOnEdge(const IsoEdge& edge) const;

    // Функция рекурсивно вычисляет точку на отрезке [v1; v2], находящуюся на изоповерхности,
    //  т.е. сила поля в вычисленной точке примерно равно threshold с точностью до accuracy.
    // Параметры: lowPoint - точка с меньшей силой поля, highPoint - точка с большей силой поля.
    // Для нахождения точки используется дихотомия (бинарный поиск).
    glm::vec3 getPointAtThresholdBetween(const glm::vec3& lowPoint, const glm::vec3& highPoint) const;

    float m_accuracy{ 0.00001f };
    float m_threshold{ 0.5f };
    std::vector<IIsoSourcePtr> m_sources;
    float m_cubeSize{ 0 };
    std::unordered_map<IsoEdge, int> m_pointIndexByEdge;
    std::vector<IsoPoint> m_points;
    std::vector<unsigned> m_indexes;

    // Размер визуализируемой области, в кубиках, (width, height, depth).
    glm::uvec3 m_size{ 0 };
};
