#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

struct VertexP2C4
{
	glm::vec2 xy;
	glm::vec4 rgba;
};

// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами и центром.
//  @param center - геометрический центр многоугольника
//  @param hullPoints - вершины многоугольника
//  @param colorGen - генератор цвета полученных треугольников
std::vector<VertexP2C4> tesselateConvexByCenter(const glm::vec2 &center, const std::vector<glm::vec2> &hullPoints, const glm::vec4 &fillColor);

// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами.
std::vector<VertexP2C4> tesselateConvex(const std::vector<glm::vec2> &verticies, const glm::vec4 &fillColor);

// Функция делит круг на треугольники,
//  возвращает массив с вершинами треугольников.
std::vector<VertexP2C4> tesselateCircle(float radius, const glm::vec2 &center, const glm::vec4 &fillColor);
