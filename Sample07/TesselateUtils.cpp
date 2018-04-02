#include "stdafx.h"
#include "TesselateUtils.h"

namespace
{

	constexpr float PI = 3.1415926f;

	glm::vec2 euclidean(float radius, float angleRadians)
	{
		return {
			radius * sin(angleRadians),
			radius * cos(angleRadians)
		};
	}

} // namespace

std::vector<VertexP2C4> tesselateTriangleFan(const glm::vec2 &center, const std::vector<glm::vec2> &hullPoints, const glm::vec4 &fillColor)
{
	const size_t size = hullPoints.size();
	std::vector<VertexP2C4> verticies;
	verticies.reserve(3u * size);
	for (size_t pointIndex = 0; pointIndex < size; ++pointIndex)
	{
		// Добавляем три вершины треугольника в список.
		const size_t nextPointIndex = (pointIndex + 1) % size;
		verticies.push_back({ hullPoints.at(pointIndex), fillColor });
		verticies.push_back({ hullPoints.at(nextPointIndex), fillColor });
		verticies.push_back({ center, fillColor });
	}

	return verticies;
}

std::vector<VertexP2C4> tesselateConvex(const std::vector<glm::vec2> &verticies, const glm::vec4 &fillColor)
{
	// Центр выпуклого многоугольника - это среднее арифметическое его вершин
	const glm::vec2 center = std::accumulate(verticies.begin(), verticies.end(), glm::vec2()) / float(verticies.size());
	return tesselateTriangleFan(center, verticies, fillColor);
}

std::vector<VertexP2C4> tesselateCircle(float radius, const glm::vec2 &center, const glm::vec4 &fillColor)
{
	assert(radius > 0);

	// Круг аппроксимируется с помощью треугольников.
	// Внешняя сторона каждого треугольника имеет длину 2.
	constexpr float step = 2;
	// Число треугольников равно длине окружности, делённой на шаг по окружности.
	const auto pointCount = static_cast<unsigned>(radius * 2 * PI / step);

	// Вычисляем точки-разделители на окружности.
	std::vector<glm::vec2> points(pointCount);
	for (unsigned pi = 0; pi < pointCount; ++pi)
	{
		const auto angleRadians = static_cast<float>(2.f * PI * pi / pointCount);
		points[pi] = center + euclidean(radius, angleRadians);
	}

	return tesselateTriangleFan(center, points, fillColor);
}

std::vector<VertexP2C4> tesselatePolarRose(float outerRadius, unsigned petelsCount, const glm::vec2 &center, const glm::vec4 &fillColor)
{
	assert(outerRadius > 0);
	assert(petelsCount >= 3);

	// Фигуры аппроксимируется с помощью треугольников.
	// Внешняя сторона каждого треугольника имеет длину 2.
	constexpr float step = 2;
	// Число треугольников равно длине описанной окружности, делённой на шаг по окружности.
	const auto pointCount = static_cast<unsigned>(outerRadius * 2 * PI / step);

	// Вычисляем точки-разделители на границе фигуры.
	std::vector<glm::vec2> points(pointCount);
	for (unsigned pi = 0; pi < pointCount; ++pi)
	{
		const auto factor = static_cast<float>(petelsCount) / 2.f;
		const auto angleRadians = static_cast<float>(2.f * PI * pi / pointCount);
		const auto radius = static_cast<float>(outerRadius * std::abs(std::cos(factor * angleRadians)));

		points[pi] = center + euclidean(radius, angleRadians);
	}

	return tesselateTriangleFan(center, points, fillColor);
}