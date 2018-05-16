#pragma once
#include <glm/vec2.hpp>
#include <optional>
#include <vector>

enum class CurveMode
{
	Lines,
	HermiteSplines,
	CatmullRomSplines,
};

struct CurveModel
{
public:
	static constexpr size_t kMinControlPointCount = 4;

	// Манипуляции с контрольными точками.
	size_t getControlPointCount() const;
	glm::vec2 getControlPoint(size_t i) const;
	void setControlPoint(size_t i, const glm::vec2 &value);
	void addControlPoint(const glm::vec2 &point);
	std::vector<glm::vec2> getControlPoints() const;

	// Уменьшает число контрольных точек кривой до size, а если число
	//  контрольных точек меньше size, то ничего не делает.
	void truncate(size_t size);

	// Свойство CurveMode - режим тесселяции кривой.
	CurveMode getCurveMode() const;
	void setCurveMode(CurveMode mode);

	// Свойство Precision - число сегменов в одном фрагменте, т.е. между
	//  двумя контрольными точками, не может быть равным нулю.
	unsigned getPrecision() const;
	void setPrecision(unsigned precision);

	std::vector<glm::vec2> tesselate() const;

private:
	std::vector<glm::vec2> tesselateWithLines() const;
	std::vector<glm::vec2> tesselateWithCatmulRomSplines() const;
	std::vector<glm::vec2> tesselateWithHermiteSplines() const;

	CurveMode m_curveMode = CurveMode::Lines;
	unsigned m_precision = 1;
	std::vector<glm::vec2> m_controlPoints;
};
