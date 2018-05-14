#pragma once

#include "IFlyingCamera.h"

class FlyingCamera : public IFlyingCamera
{
public:
	explicit FlyingCamera(const glm::vec3 &eye, const glm::vec3 &at, const glm::vec3 &up);

	// Перемещает камеру в пространстве
	// @param delta - вектор перемещения, заданный относительно наблюдателя.
	void move(const glm::vec3 &delta) final;

	// Поворачивает камеру, используя рысканье и тангаж камеры
	// @param yaw - угол рысканья, вызывает поворот вектора forward вокруг оси, заданной вектором up.
	// @param pitch - угол тангажа, вызывает поворот вектора up вокруг оси, заданной вектором right
	void rotate(float yaw, float pitch) final;

	// Возвращает позицию камеры.
	glm::vec3 getViewPosition() const final;

	// Возвращает матрицу моделирования-вида для текущего положения и ориентации камеры.
	glm::mat4 getViewTransform() const final;

private:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
};
