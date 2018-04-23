#include "stdafx.h"
#include "FlyingCamera.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace glm;

namespace
{
// � OpenGL ������������ ������������� ������� ���������,
//  �������������, ������ `right` ������ ���� �����, ����� � ��� �����
//  ���������� ������� �� ������� `m_up` � ������� `m_forward` ��� �����
//  �� ������� �������, �� ���� ��������� � ��������� �������������.
// ��. http://www.gamedev.ru/code/forum/?id=43526
vec3 getRightDirection(const vec3 &up, const vec3 &forward)
{
	return glm::normalize(glm::cross(up, forward));
}

// �� ��������, ��������� ����, ��������� ���� ����� � ���������� ������������.
vec3 getForwardDirection(const vec3 &up, const vec3 &right)
{
	return -glm::normalize(glm::cross(up, right));
}

// �� ��������, ��������� ����, ���������� ��������� ������������ ��� ����� �����.
vec3 getUpDirection(const vec3 &forward, const vec3 &right)
{
	return glm::normalize(glm::cross(forward, right));
}
} // namespace

FlyingCamera::FlyingCamera(const glm::vec3 &eye, const glm::vec3 &at, const glm::vec3 &up)
{
	m_position = eye;

	// ������ ����������� "�����" ���������� ������������, ����� "������" � ���������� �� �����, ���� ��������� ������.
	m_forward = normalize(at - eye);

	// ���� ����� `forward` � `up` ����� ���� �� ����� 90 ��������, ������� �� ������ ��������� ��������� ������������
	//  ��� ��������� ��������� ����������� `up`.
	const vec3 right = getRightDirection(up, m_forward);
	m_up = getUpDirection(m_forward, right);
}

void FlyingCamera::move(const glm::vec3 &delta)
{
	const vec3 right = getRightDirection(m_up, m_forward);
	const vec3 orientedDelta = right * delta.x + m_up * delta.y + m_forward * delta.z;
	m_position += orientedDelta;
}

void FlyingCamera::rotate(float yaw, float pitch)
{
	m_forward = glm::normalize(glm::rotate(m_forward, yaw, m_up));

	const vec3 right = getRightDirection(m_up, m_forward);
	m_up = glm::normalize(glm::rotate(m_up, pitch, right));

	// ������� up ������ right �������� forward, � ��� ���� �����������.
	m_forward = getForwardDirection(m_up, right);
}

glm::mat4 FlyingCamera::getViewTransform() const
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}
