#include "stdafx.h"
#include "CurveView.h"
#include "TesselateUtils.h"
#include <glbinding/gl33core/gl.h>
#include <glm/gtc/type_ptr.hpp>

using namespace gl33core;

void CurveView::setControlPoints(const std::vector<glm::vec2> &points)
{
	// TODO: реализуйте рисование каждой контрольной точки отдельной фигурой - кругом.
	// Для этого:
	// 1) создайте новый vector<vec2>, его можно назвать, например, vertexes - он будет хранить
	//    все вершины всех треугольников. Инициализируйте VBO с помощью этого массива.
	// 2) Используйте функцию utils::tesselateCircle(...) для получения массива точек
	//    каждого из кругов.
	// 3) Используте метод insert(...) контейнера std::vector<> для конкатенации массивов вершин.

	m_controlPoints = glcore::createStaticVBO(GL_ARRAY_BUFFER, points);
	m_controlPointsCount = static_cast<gl::GLsizei>(points.size());
}

void CurveView::setCurvePoints(const std::vector<glm::vec2> &points)
{
	m_curvePoints = glcore::createStaticVBO(GL_ARRAY_BUFFER, points);
	m_curvePointsCount = static_cast<gl::GLsizei>(points.size());
}

glm::vec4 CurveView::getPointsColor() const
{
	return m_pointsColor;
}

void CurveView::setPointsColor(const glm::vec4 &rgba)
{
	m_pointsColor = rgba;
}

glm::vec4 CurveView::getLinesColor() const
{
	return m_linesColor;
}

void CurveView::setLinesColor(const glm::vec4 &rgba)
{
	m_linesColor = rgba;
}

float CurveView::getPointRadius() const
{
	return m_pointRadius;
}

void CurveView::setPointRadius(float radius)
{
	m_pointRadius = radius;
}

void CurveView::draw(IShaderProgram &program)
{
	// Рисуем точки.
	const int colorLocation = program.getUniform(UniformCurrentColor);
	const unsigned positionLocation = static_cast<unsigned>(program.getAttribute(AttributePosition));

	glBindBuffer(GL_ARRAY_BUFFER, m_curvePoints);
	glUniform4fv(colorLocation, 1, glm::value_ptr(m_linesColor));
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
	glDrawArrays(GL_LINE_STRIP, 0, m_curvePointsCount);

	// TODO: реализуйте рисование каждой контрольной точки отдельной фигурой - кругом.
	// Для этого:
	//  1) храните m_controlPoints вершины фигур (кругов), а не точки
	//  2) вызывайте glDrawArrays с примитивом GL_TRIANGLES
	glPointSize(2 * m_pointRadius);
	glBindBuffer(GL_ARRAY_BUFFER, m_controlPoints);
	glUniform4fv(colorLocation, 1, glm::value_ptr(m_pointsColor));
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
	glDrawArrays(GL_POINTS, 0, m_controlPointsCount);
}
