#pragma once

#include <glm/vec2.hpp>

namespace math
{
class FloatRect
{
public:
	FloatRect() = default;
	FloatRect(const glm::vec2 &topLeft, const glm::vec2 &bottomRight);

	glm::vec2 getTopLeft() const;
	glm::vec2 getTopRight() const;
	glm::vec2 getBottomLeft() const;
	glm::vec2 getBottomRight() const;
	glm::vec2 getSize() const;

	bool contains(const glm::vec2 &point) const;
	FloatRect getScaled(const glm::vec2 &factors) const;

	void moveTo(const glm::vec2 &topLeft);

private:
	glm::vec2 m_topLeft;
	glm::vec2 m_bottomRight;
};
} // namespace math
