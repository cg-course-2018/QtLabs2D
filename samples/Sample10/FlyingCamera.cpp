#include "stdafx.h"
#include "FlyingCamera.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace glm;

namespace
{
// В OpenGL используется левосторонняя система координат,
//  следовательно, вектор `right` должен быть таким, чтобы с его конца
//  кратчайший поворот от вектора `m_up` к вектору `m_forward` был виден
//  по часовой стрелке, то есть совпадает с векторным произведением.
// См. http://www.gamedev.ru/code/forum/?id=43526
vec3 getRightDirection(const vec3 &up, const vec3 &forward)
{
	return glm::normalize(glm::cross(up, forward));
}

// По причинам, описанным выше, добавляем знак минус к векторному произведению.
vec3 getForwardDirection(const vec3 &up, const vec3 &right)
{
	return -glm::normalize(glm::cross(up, right));
}

// По причинам, описанным выше, используем векторное произведение без знака минус.
vec3 getUpDirection(const vec3 &forward, const vec3 &right)
{
	return glm::normalize(glm::cross(forward, right));
}
} // namespace

FlyingCamera::FlyingCamera(const glm::vec3 &eye, const glm::vec3 &at, const glm::vec3 &up)
{
	m_position = eye;

	// Вектор направления "вперёд" подвергнем нормализации, чтобы "забыть" о расстоянии до точки, куда направлен взгляд.
	m_forward = normalize(at - eye);

	// Угол между `forward` и `up` может быть не равен 90 градусам, поэтому мы дважды вычисляем векторное произведение
	//  для получения истинного направления `up`.
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

	// Поворот up вокруг right изменяет forward, и его надо пересчитать.
	m_forward = getForwardDirection(m_up, right);
}

glm::vec3 FlyingCamera::getViewPosition() const
{
	return m_position;
}

glm::mat4 FlyingCamera::getViewTransform() const
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}
