#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace utils
{

// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами и центром.
//  @param center - геометрический центр многоугольника
//  @param hullPoints - вершины многоугольника
//  @param colorGen - генератор цвета полученных треугольников
std::vector<glm::vec2> tesselateConvexByCenter(const glm::vec2 &center, const std::vector<glm::vec2> &hullPoints);

// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами.
std::vector<glm::vec2> tesselateConvex(const std::vector<glm::vec2> &verticies);

// Функция делит круг на треугольники,
//  возвращает массив с вершинами треугольников.
std::vector<glm::vec2> tesselateCircle(float radius, const glm::vec2 &center);

}
