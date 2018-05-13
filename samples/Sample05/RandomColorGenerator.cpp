#include "RandomColorGenerator.h"

namespace
{
std::vector<glm::vec4> MakePalette()
{
	// Превращает rgb(255, 0, 128) в vec4{ 1, 0, 0.5, 1 }
	auto rgb = [](unsigned red, unsigned green, unsigned blue) {
		return glm::vec4(float(red) / 255.f, float(green) / 255.f, float(blue) / 255.f, 1);
	};

	// Цвета подобраны на сайте https://websafecolors.info/color-chart
	return {
		rgb(0, 204, 102),
		rgb(102, 102, 102),
		rgb(102, 153, 204),
		rgb(153, 255, 153),
		rgb(204, 153, 51),
		rgb(0, 255, 102),
		rgb(204, 0, 102),
		rgb(204, 102, 255),
		rgb(102, 255, 255),
		rgb(153, 255, 102),
	};
}
}

RandomColorGenerator::RandomColorGenerator()
	: m_palette(MakePalette())
	, m_generator(m_rd())
	, m_indexDist(0, m_palette.size() - 1u)
{
}

glm::vec4 RandomColorGenerator::GenerateColor()
{
	const size_t index = m_indexDist(m_generator);
	return m_palette.at(index);
}
