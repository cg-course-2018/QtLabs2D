#include "stdafx.h"
#include "TesselateUtils.h"
#include <cmath>
#include <numeric>

namespace
{

constexpr float PI = 3.1415926f;

glm::vec2 euclidean(float radius, float angleRadians)
{
	return {
		radius * std::sin(angleRadians),
		radius * std::cos(angleRadians)
	};
}

} // namespace

std::vector<glm::vec2> utils::tesselateConvexByCenter(const glm::vec2 &center, const std::vector<glm::vec2> &hullPoints)
{
	const size_t size = hullPoints.size();
	std::vector<glm::vec2> verticies;
	verticies.reserve(3u * size);
	for (size_t pointIndex = 0; pointIndex < size; ++pointIndex)
	{
		// Добавляем три вершины треугольника в список.
		const size_t nextPointIndex = (pointIndex + 1) % size;
		verticies.push_back(hullPoints.at(pointIndex));
		verticies.push_back(hullPoints.at(nextPointIndex));
		verticies.push_back(center);
	}

	return verticies;
}

std::vector<glm::vec2> utils::tesselateConvex(const std::vector<glm::vec2> &verticies)
{
	// Центр выпуклого многоугольника - это среднее арифметическое его вершин
	const glm::vec2 center = std::accumulate(verticies.begin(), verticies.end(), glm::vec2()) / float(verticies.size());
	return tesselateConvexByCenter(center, verticies);
}

std::vector<glm::vec2> utils::tesselateCircle(float radius, const glm::vec2 &center)
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

	return tesselateConvexByCenter(center, points);
}
