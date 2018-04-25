#include "stdafx.h"
#include "FloatRect.h"

FloatRect::FloatRect(const glm::vec2 &topLeft, const glm::vec2 &bottomRight)
	: m_topLeft(topLeft)
	, m_bottomRight(bottomRight)
{
	assert(m_bottomRight.x >= m_topLeft.x
		&& m_bottomRight.y >= m_topLeft.y);
}

glm::vec2 FloatRect::getTopLeft() const
{
	return m_topLeft;
}

glm::vec2 FloatRect::getTopRight() const
{
	return { m_bottomRight.x, m_topLeft.y };
}

glm::vec2 FloatRect::getBottomLeft() const
{
	return { m_topLeft.x, m_bottomRight.y };
}

glm::vec2 FloatRect::getBottomRight() const
{
	return m_bottomRight;
}

glm::vec2 FloatRect::getSize() const
{
	return m_bottomRight - m_topLeft;
}

bool FloatRect::contains(const glm::vec2 &point) const
{
	return (m_topLeft.x <= point.x && point.x <= m_bottomRight.x
		&& m_topLeft.y <= point.y && point.y <= m_bottomRight.y);
}

FloatRect FloatRect::getScaled(const glm::vec2 &factors) const
{
	FloatRect copy(*this);

	// Домножаем для масштабирования.
	copy.m_topLeft *= factors;
	copy.m_bottomRight *= factors;

	return copy;
}

void FloatRect::moveTo(const glm::vec2 &topLeft)
{
	const glm::vec2 size = getSize();
	m_topLeft = topLeft;
	m_bottomRight = topLeft + size;
}
