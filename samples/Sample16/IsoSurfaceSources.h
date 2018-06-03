#pragma once

#include <glm/vec3.hpp>
#include <memory>

class IIsoSurfaceSource
{
public:
	virtual ~IIsoSurfaceSource() = default;
	virtual float getStrengthAtPosition(const glm::vec3 &position) const = 0;
};

using IIsoSurfaceSourcePtr = std::shared_ptr<IIsoSurfaceSource>;

class IsoPointSource : public IIsoSurfaceSource
{
public:
	explicit IsoPointSource(const glm::vec3 &position, float radius);

	float getStrengthAtPosition(const glm::vec3 &position) const final;

private:
	glm::vec3 m_position{ 0 };
	float m_radius{ 0 };
};
