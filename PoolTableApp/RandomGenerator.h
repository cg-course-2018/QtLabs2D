#pragma once

#include <random>
#include <libballs2d\Ball.h>
#include <libmath\Color4f.h>

class RandomGenerator
{
public:
	RandomGenerator();
	~RandomGenerator();

	int getRandomValue(int min, int max);
	Color4f getRandomColor();
	Ball getRandomBall();
private:
	std::mt19937 m_randomEngine;
};

