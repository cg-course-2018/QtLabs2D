#include "stdafx.h"
#include "Ball.h"

Ball::Ball(float r, Vector2f center, Vector2f speed, Color4f color)
	: m_radius{ r }, m_center{ center }, m_speed{ speed }, m_color{ color }, m_remainingTime{ 10.f }
{
}

Ball::~Ball()
{
}


RectF Ball::bbox()
{
	return RectF(m_center.x - m_radius, m_center.y - m_radius, m_radius * 2, m_radius * 2);
}


Color4f Ball::color()
{
	return m_color;
}


float Ball::mass() const
{
	return radius() * radius();
}

float Ball::energy() const
{
	float speed_abs = speed().length();
	return mass() * speed_abs * speed_abs / 2;
}

Vector2f Ball::impulse() const
{
	return speed() * mass();
}

void Ball::move(float dt)
{
	m_center.x += m_speed.x * dt;
	m_center.y += m_speed.y * dt;
}

