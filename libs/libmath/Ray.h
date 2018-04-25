#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

/*
Класс "Луч", характеризующийся точкой испускания и направлением.
  В момент времени t=0 луч находится в точке start.
  За промежуток времени t=1 луч проходит расстояние direction.
*/
class Ray
{
public:
	Ray() = default;
	explicit Ray(const glm::vec3 &start, const glm::vec3 &direction);

	glm::vec3 getPointAtTime(float time) const;

	const glm::vec3 &getStart() const;
	const glm::vec3 &getDirection() const;

	Ray getTransformedCopy(const glm::mat4 &transform) const;

private:
	glm::vec3 m_start;
	glm::vec3 m_direction;
};

struct SRayIntersection
{
	// Время пересечения с лучём (по временной шкале луча).
	float m_time;
	// Точка пересечения
	glm::vec3 m_point;
};

/*
Геометрический объект "бесконечная плоскость",
  который задаётся уравнением плоскости из 4-х коэффициентов.
*/
class Plane
{
public:
	// Три точки определяют плоскость, из них могут быть восстановлены
	//  коэффициенты уравнения плоскости.
	explicit Plane(const glm::vec3 &point0,
		const glm::vec3 &point1,
		const glm::vec3 &point2);

	explicit Plane(const glm::vec4 &planeEquation);

	bool hit(Ray const &ray, SRayIntersection &intersection) const;

private:
	// Четырехмерный вектор, хранящий коэффициенты уравнения плоскости
	glm::vec4 m_planeEquation;
};
