#pragma once

#include "IShaderProgram.h"
#include "libglcore/libglcore.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class CurveView
{
public:
	// Устанавливает контрольные точки (в малом количестве).
	void setControlPoints(const std::vector<glm::vec2> &points);

	// Устанавливает координаты вершин кривой (в большом количестве).
	void setCurvePoints(const std::vector<glm::vec2> &points);

	// Устанавливает цвет, используемый при рисовании контрольных точек.
	glm::vec4 getPointsColor() const;
	void setPointsColor(const glm::vec4& rgba);

	// Устанавливает цвет, используемый при рисовании линии кривой.
	glm::vec4 getLinesColor() const;
	void setLinesColor(const glm::vec4& rgba);

	// PointRadius - радиус, используемый при рисовании точки.
	float getPointRadius() const;
	void setPointRadius(float radius);

	// Рисует кривую с текущими параметрами.
	void draw(IShaderProgram& program);

private:
	glcore::VBO m_controlPoints;
	gl::GLsizei m_controlPointsCount = 0;

	glcore::VBO m_curvePoints;
	gl::GLsizei m_curvePointsCount = 0;

	glm::vec4 m_pointsColor;
	glm::vec4 m_linesColor;
	float m_pointRadius = 1;
};
