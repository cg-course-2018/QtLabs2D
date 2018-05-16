#include "stdafx.h"
#include "CurveModel.h"
#include <algorithm>
#include <functional>
#include <glm/gtx/spline.hpp>
#include <iterator>

size_t CurveModel::getControlPointCount() const
{
	return m_controlPoints.size();
}

glm::vec2 CurveModel::getControlPoint(size_t i) const
{
	return m_controlPoints.at(i);
}

void CurveModel::setControlPoint(size_t i, const glm::vec2 &value)
{
	m_controlPoints.at(i) = value;
}

void CurveModel::addControlPoint(const glm::vec2 &point)
{
	m_controlPoints.push_back(point);
}

std::vector<glm::vec2> CurveModel::getControlPoints() const
{
	return m_controlPoints;
}

void CurveModel::truncate(size_t size)
{
	if (m_controlPoints.size() > size)
	{
		m_controlPoints.resize(size);
	}
}

CurveMode CurveModel::getCurveMode() const
{
	return m_curveMode;
}

void CurveModel::setCurveMode(CurveMode mode)
{
	m_curveMode = mode;
}

unsigned CurveModel::getPrecision() const
{
	return m_precision;
}

void CurveModel::setPrecision(unsigned precision)
{
	if (m_precision == 0)
	{
		throw std::logic_error("precision cannot be 0");
	}
	m_precision = precision;
}

std::vector<glm::vec2> CurveModel::tesselate() const
{
	// TODO: удалите у класса свойство precision, вместо него добавьте расчёт числа необходимых
	//  сегментов на основе длины фрагмента кривой
	//   - длина фрагмента кривой больше или равна distance(v2, v3)
	//   - число сегментов можно получить, округлив длину до целого
	//  Сделать это нужно для двух методов
	//   - tesselateWithCatmulRomSplines
	//   - tesselateWithHermiteSplines

	if (m_controlPoints.size() < kMinControlPointCount)
	{
		throw std::logic_error("cannot tesselate: number of control points less than 4");
	}

	//Замыкаем
	std::vector<glm::vec2> vertexes = m_controlPoints; // = utils::tesselateCircle(1, &points);
	//

	switch (m_curveMode)
	{
	case CurveMode::Lines:
		vertexes.push_back(m_controlPoints[0]);
		return tesselateWithLines(vertexes);
	case CurveMode::HermiteSplines:
		return tesselateWithHermiteSplines(vertexes);
	case CurveMode::CatmullRomSplines:
		return tesselateWithCatmulRomSplines(vertexes);
	}

	throw std::logic_error("unknown curve mode");
}

std::vector<glm::vec2> CurveModel::tesselateWithLines(const std::vector<glm::vec2> &points) const
{

	return points;
}

std::vector<glm::vec2> CurveModel::tesselateWithCatmulRomSplines(const std::vector<glm::vec2> &points) const
{
	auto wrap = [&](int i, int d, int size) { return (size + i + d) % size; };

	// Перебираем четвёрки контрольных точек, для крайних фрагментов используем
	//  крайнюю точку дважды.
	std::vector<glm::vec2> ppoints;
	for (size_t ci = 0; ci + 1 <= points.size(); ++ci)
	{

		//const glm::vec2 v1 = (ci == 0) ? points[points.size() - 1] : points[ci - 1];
		const glm::vec2 v1 = points[wrap(ci, -1, points.size())];
		const glm::vec2 v2 = points[wrap(ci, 0, points.size())];
		const glm::vec2 v3 = points[wrap(ci, 1, points.size())];
		const glm::vec2 v4 = points[wrap(ci, 2, points.size())];

		int dist = (int)glm::round(glm::distance(v2, v3));

		// Для каждой четвёрки строим серию сегментов линии в зависимости от заданного числа делений.
		for (unsigned si = 0; si <= dist; ++si)
		{
			const float mixFactor = float(si) / float(dist);
			const glm::vec2 point = glm::catmullRom(v1, v2, v3, v4, mixFactor);
			ppoints.push_back(point);
		}
	}
	return ppoints;
}

std::vector<glm::vec2> CurveModel::tesselateWithHermiteSplines(const std::vector<glm::vec2> &points) const
{
	auto wrap = [&](int i, int d, int size) { return (size + i + d) % size; };
	// Перебираем четвёрки контрольных точек, для крайних фрагментов используем
	//  крайнюю точку дважды.
	std::vector<glm::vec2> ppoints;
	for (size_t ci = 0; ci + 1 <= points.size(); ++ci)
	{
		// Ниже t1, t2 - тангенсоиды, т.е. касательные лучи к кривой в точках v1, v2.
		// Тангенсоиду можно вычислить как разность двух контрольных точек.

		const glm::vec2 v1 = points[wrap(ci, 0, points.size())];
		const glm::vec2 v2 = points[wrap(ci, 1, points.size())];
		const glm::vec2 t1 = (v1 - points[wrap(ci, -1, points.size())]);
		const glm::vec2 t2 = (v2 - points[wrap(ci, 0, points.size())]);

		/*const glm::vec2 v1 = points[ci];
		const glm::vec2 v2 = points[ci + 1];
		const glm::vec2 t1 = (ci == 0) ? (v2 - v1) : (v1 - points[ci - 1]);
		const glm::vec2 t2 = (ci + 2 == points.size()) ? (v2 - v1) : (v2 - points[ci + 1]);*/

		int dist = (int)glm::round(glm::distance(v1, v2));

		// Для каждой четвёрки строим серию сегментов линии в зависимости от заданного числа делений.
		for (unsigned si = 0; si <= dist; ++si)
		{
			const float mixFactor = float(si) / float(dist);
			const glm::vec2 point = glm::hermite(v1, t1, v2, t2, mixFactor);
			ppoints.push_back(point);
		}
	}
	return ppoints;
}
