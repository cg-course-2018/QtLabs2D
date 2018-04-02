#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

struct VertexP2C4
{
	glm::vec2 xy;
	glm::vec4 rgba;
};

// Генерирует список вершин треугольников по методу веера треугольников, образованного центром и вершинами фигуры.
//  @param center - геометрический центр многоугольника
//  @param hullPoints - вершины фигуры
//  @param fillColor - цвет полученных треугольников
std::vector<VertexP2C4> tesselateTriangleFan(const glm::vec2 &center, const std::vector<glm::vec2> &hullPoints, const glm::vec4 &fillColor);

// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами.
std::vector<VertexP2C4> tesselateConvex(const std::vector<glm::vec2> &verticies, const glm::vec4 &fillColor);

// Функция делит круг на треугольники,
//  возвращает массив с вершинами треугольников.
std::vector<VertexP2C4> tesselateCircle(float radius, const glm::vec2 &center, const glm::vec4 &fillColor);

// Генерирует список вершин треугольников для фигуры Polar Rose (Полярная Роза),
//  возвращает массив с вершинами треугольников.
//  @param outerRadius - радиус окружности, описывающей полярную розу
//  @param petelsCount - число лепестков, не менее 3
//  @param center - геометрический центр фигуры
//  @param fillColor - цвет полученных треугольников
std::vector<VertexP2C4> tesselatePolarRose(float outerRadius, unsigned petelsCount, const glm::vec2 &center, const glm::vec4 &fillColor);