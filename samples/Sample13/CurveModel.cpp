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
    if (m_controlPoints.size() < kMinControlPointCount)
    {
		throw std::logic_error("cannot tesselate: number of control points less than 4");
    }

    // Перебираем четвёрки контрольных точек, для крайних фрагментов используем
    //  крайнюю точку дважды.
    std::vector<glm::vec2> points;
	for (size_t i = 1; i + 2 < m_controlPoints.size(); ++i)
    {
		const glm::vec2 v1 = m_controlPoints[i - 1];
        const glm::vec2 v2 = m_controlPoints[i];
        const glm::vec2 v3 = m_controlPoints[i + 1];
		const glm::vec2 v4 = m_controlPoints[i + 2];
        tesselateFragment(points, v1, v2, v3, v4);
    }
    return points;
}

void CurveModel::tesselateFragment(
    std::vector<glm::vec2> &points,
    const glm::vec2 &v1,
    const glm::vec2 &v2,
    const glm::vec2 &v3,
    const glm::vec2 &v4) const
{
	// TODO: удалите у класса свойство precision, вместо него добавьте расчёт числа необходимых
	//  сегментов на основе длины фрагмента кривой
	//   - длина фрагмента кривой - это distance(v2, v3)
	//   - число сегментов можно получить, округлив длину до целого

    // Выбираем функцию генерации вершины
    std::function<glm::vec2(float)> generatePoint;
    switch (m_curveMode)
    {
    case CurveMode::Lines:
        generatePoint = [&](float mixFactor) {
            return glm::mix(v2, v3, mixFactor);
        };
        break;
    case CurveMode::CubicSplines:
        generatePoint = [&](float mixFactor) {
            return glm::cubic(v1, v2, v3, v4, mixFactor);
        };
        break;
    case CurveMode::CatmullRomSplines:
        generatePoint = [&](float mixFactor) {
            return glm::catmullRom(v1, v2, v3, v4, mixFactor);
        };
        break;
    }

    for (unsigned i = 0; i < m_precision; ++i)
    {
		const float mixFactor = 1 * float(i) / float(m_precision);
        const glm::vec2 point = generatePoint(mixFactor);
        points.push_back(point);
    }
}
