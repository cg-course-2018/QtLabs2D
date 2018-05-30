#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class IFlyingCamera
{
public:
	virtual ~IFlyingCamera() = default;

	// Перемещает камеру в пространстве
	// @param delta - вектор перемещения, заданный относительно наблюдателя.
	virtual void move(const glm::vec3 &delta) = 0;

	// Поворачивает камеру, используя рысканье и тангаж камеры
	// @param yaw - угол рысканья в радианах, вызывает поворот вектора forward вокруг оси, заданной вектором up.
	// @param pitch - угол тангажа в радианах, вызывает поворот вектора up вокруг оси, заданной вектором right
	virtual void rotate(float yaw, float pitch) = 0;

	// Возвращает позицию камеры.
	virtual glm::vec3 getViewPosition() const = 0;

	// Возвращает матрицу моделирования-вида для текущего положения и ориентации камеры.
	virtual glm::mat4 getViewTransform() const = 0;
};
