#include "stdafx.h"
#include "Transform3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

using namespace glm;

void Transform3D::rotateBy(const glm::quat &rotation)
{
	m_orientation *= rotation;
}

void Transform3D::scaleBy(const glm::vec3 &scale)
{
	m_size *= scale;
}

void Transform3D::scaleBy(const float scale)
{
	m_size *= scale;
}

void Transform3D::moveBy(const glm::vec3 &distance)
{
	m_position += distance;
}

glm::mat4 Transform3D::toMat4() const
{
	const mat4 scaleMatrix = scale(mat4(1), m_size);
	const mat4 rotationMatrix = mat4_cast(m_orientation);
	const mat4 translateMatrix = translate(mat4(1), m_position);
	return translateMatrix * rotationMatrix * scaleMatrix;
}

glm::vec3 Transform3D::transformPoint(const glm::vec3 &point) const
{
	const glm::vec4 result = toMat4() * glm::vec4(point, 1.f);
	return { result.x, result.y, result.z };
}
