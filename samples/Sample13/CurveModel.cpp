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

	switch (m_curveMode)
	{
	case CurveMode::Lines:
		return tesselateWithLines();
	case CurveMode::HermiteSplines:
		return tesselateWithHermiteSplines();
	case CurveMode::CatmullRomSplines:
		return tesselateWithCatmulRomSplines();
	}

	throw std::logic_error("unknown curve mode");
}

std::vector<glm::vec2> CurveModel::tesselateWithLines() const
{
	return m_controlPoints;
}

std::vector<glm::vec2> CurveModel::tesselateWithCatmulRomSplines() const
{
	// Перебираем четвёрки контрольных точек, для крайних фрагментов используем
	//  крайнюю точку дважды.
	std::vector<glm::vec2> points;
	for (size_t ci = 0; ci + 2 <= m_controlPoints.size(); ++ci)
	{
		const glm::vec2 v1 = (ci == 0) ? m_controlPoints[0] : m_controlPoints[ci - 1];
		const glm::vec2 v2 = m_controlPoints[ci];
		const glm::vec2 v3 = m_controlPoints[ci + 1];
		const glm::vec2 v4 = (ci + 2 == m_controlPoints.size()) ? m_controlPoints[ci + 1] : m_controlPoints[ci + 2];

		// Для каждой четвёрки строим серию сегментов линии в зависимости от заданного числа делений.
		for (unsigned si = 0; si <= m_precision; ++si)
		{
			const float mixFactor = float(si) / float(m_precision);
			const glm::vec2 point = glm::catmullRom(v1, v2, v3, v4, mixFactor);
			points.push_back(point);
		}
	}
	return points;
}

std::vector<glm::vec2> CurveModel::tesselateWithHermiteSplines() const
{

	// Перебираем четвёрки контрольных точек, для крайних фрагментов используем
	//  крайнюю точку дважды.
	std::vector<glm::vec2> points;
	for (size_t ci = 0; ci + 2 <= m_controlPoints.size(); ++ci)
	{
		// Ниже t1, t2 - тангенсоиды, т.е. касательные лучи к кривой в точках v1, v2.
		// Тангенсоиду можно вычислить как разность двух контрольных точек.
		const glm::vec2 v1 = m_controlPoints[ci];
		const glm::vec2 v2 = m_controlPoints[ci + 1];
		const glm::vec2 t1 = (ci == 0) ? (v2 - v1) : (v1 - m_controlPoints[ci - 1]);
		const glm::vec2 t2 = (ci + 2 == m_controlPoints.size()) ? (v2 - v1) : (v2 - m_controlPoints[ci + 1]);

		// Для каждой четвёрки строим серию сегментов линии в зависимости от заданного числа делений.
		for (unsigned si = 0; si <= m_precision; ++si)
		{
			const float mixFactor = float(si) / float(m_precision);
			const glm::vec2 point = glm::hermite(v1, t1, v2, t2, mixFactor);
			points.push_back(point);
		}
	}
	return points;
}
