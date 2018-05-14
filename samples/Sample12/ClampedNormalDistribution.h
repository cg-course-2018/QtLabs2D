#pragma once

#include <random>

// Урезанное по минимуму и максимуму нормальное распределение,
//  наибольшую вероятность выпадения имеет значение (min + max) / 2
// Создано на основе http://stackoverflow.com/questions/28618900
class ClampedNormalDistribution
{
	std::normal_distribution<float> m_distribution;
	float m_min = 0.f;
	float m_max = 0.f;

public:
	// m_distribution требует два параметра:
	// `mean`, т.е. медианное значение и одновременно мат. ожидание
	// `stddev`, т.е. стандартное отклонение (дисперсию)
	// мы выводим эти параметры из min/max.
	void param(float min, float max)
	{
		using param_type = std::normal_distribution<float>::param_type;
		const float mean = (min + max) / 2.f;
		const float stddev = (max - min) / 6.f;
		m_distribution.param(param_type(mean, stddev));
		m_min = min;
		m_max = max;
	}

	// Нормальное распределение выдаёт значения на всём диапазоне float
	// Но мы режем значения, выпадающие из диапазона [min, max]
	// Статистически, будет выброшено около 0.3% значений.
	float operator()(std::mt19937 &random)
	{
		while (true)
		{
			float number = m_distribution(random);
			if (number >= m_min && number <= m_max)
				return number;
		}
	}
};
