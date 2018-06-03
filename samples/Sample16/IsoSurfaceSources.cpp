#include "stdafx.h"
#include "IsoSurfaceSources.h"
#include <glm/geometric.hpp>

using namespace glm;

IsoPointSource::IsoPointSource(const vec3 &position, float radius)
	: m_position(position)
	, m_radius(radius)
{
}

float IsoPointSource::getStrengthAtPosition(const vec3 &position) const
{
	const float distance = glm::distance(m_position, position);
	if (distance < std::abs(m_radius))
	{
		float component1 = -0.444444 * (powf(distance, 6) / powf(m_radius, 6));
		float component2 = 1.888889 * (powf(distance, 4) / powf(m_radius, 4));
		float component3 = -2.444445 * (powf(distance, 2) / powf(m_radius, 2));
		float strength = component1 + component2 + component3 + 1.0;
		return (m_radius < 0) ? -strength : strength;
	}
	return 0;
}
