#pragma once
#include <glm/vec4.hpp>
#include <vector>
#include <random>

class RandomColorGenerator
{
public:
	RandomColorGenerator();

	glm::vec4 GenerateColor();

private:
	// Палитра цветов для выбора.
	std::vector<glm::vec4> m_palette;

	// random_device - это источник непредсказуемых псевдослучайных чисел.
	std::random_device m_rd;

	// mt19937 - это "Вихрь Мерсенна", он размножает одно псевдослучайное число,
	//  превращая в несколько по известному алгоритму.
	std::mt19937 m_generator;

	// В линейном случайном распределении вероятность появления
	//  каждого из чисел одинакова.
	std::uniform_int_distribution<size_t> m_indexDist;
};