#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class IFlyingCamera
{
public:
	virtual ~IFlyingCamera() = default;

	// ���������� ������ � ������������
	// @param delta - ������ �����������, �������� ������������ �����������.
	virtual void move(const glm::vec3 &delta) = 0;

	// ������������ ������, ��������� �������� � ������ ������
	// @param yaw - ���� �������� � ��������, �������� ������� ������� forward ������ ���, �������� �������� up.
	// @param pitch - ���� ������� � ��������, �������� ������� ������� up ������ ���, �������� �������� right
	virtual void rotate(float yaw, float pitch) = 0;

	// ���������� ������� ������.
	virtual glm::vec3 getViewPosition() const = 0;

	// ���������� ������� �������������-���� ��� �������� ��������� � ���������� ������.
	virtual glm::mat4 getViewTransform() const = 0;
};
