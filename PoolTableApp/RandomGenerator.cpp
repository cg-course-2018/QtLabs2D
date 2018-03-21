#include "stdafx.h"
#include "RandomGenerator.h"

#include <ctime>

namespace
{
	constexpr int BALL_MIN_RADIUS = 10;
	constexpr int BALL_MAX_RADIUS = 50;
	constexpr int BALL_MIN_SPEED = 0;
	constexpr int BALL_MAX_SPEED = 500;
}

RandomGenerator::RandomGenerator()
{
	const unsigned seed = unsigned(std::time(nullptr));
	m_randomEngine.seed(seed);
}

RandomGenerator::~RandomGenerator()
{
}

int RandomGenerator::getRandomValue(int min, int max)
{
	std::uniform_int_distribution<size_t> distribution(min, max);
	return (int)distribution(m_randomEngine);
}

Color4f RandomGenerator::getRandomColor()
{
	int r = getRandomValue(0, 255);
	int g = getRandomValue(0, 255);
	int b = getRandomValue(0, 255);
	return Color4f(r / 255.f, g / 255.f, b / 255.f);
}

Ball RandomGenerator::getRandomBall()
{
	float r = getRandomValue(BALL_MIN_RADIUS, BALL_MAX_RADIUS);
	float speed_x = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
	float speed_y = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
	float pos_x = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
	float pos_y = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
	return Ball(r, Vector2f(pos_x, pos_y), Vector2f(speed_x, speed_y), getRandomColor());
}