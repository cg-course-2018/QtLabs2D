#pragma once

#include "IFlyingCamera.h"

class FlyingCamera : public IFlyingCamera
{
public:
	explicit FlyingCamera(const glm::vec3 &eye, const glm::vec3 &at, const glm::vec3 &up);

	// ���������� ������ � ������������
	// @param delta - ������ �����������, �������� ������������ �����������.
	void move(const glm::vec3 &delta) final;

	// ������������ ������, ��������� �������� � ������ ������
	// @param yaw - ���� ��������, �������� ������� ������� forward ������ ���, �������� �������� up.
	// @param pitch - ���� �������, �������� ������� ������� up ������ ���, �������� �������� right
	void rotate(float yaw, float pitch) final;

	// ���������� ������� ������.
	glm::vec3 getViewPosition() const final;

	// ���������� ������� �������������-���� ��� �������� ��������� � ���������� ������.
	glm::mat4 getViewTransform() const final;

private:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
};
