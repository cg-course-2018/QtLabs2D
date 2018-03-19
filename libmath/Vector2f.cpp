#include "stdafx.h"
#include "Vector2f.h"
#include <cmath>

Vector2f::Vector2f(float x, float y)
	: x(x), y(y)
{
}

float Vector2f::length() const
{
	return std::hypot(x, y);
}

void Vector2f::normalize()
{
	float len = length();
	if (len != 0)
	{
		x /= len;
		y /= len;
	}
}

float Vector2f::projectOn(const Vector2f & other) const
{
	return dot(*this, other) / other.length();
}

Vector2f Vector2f::operator+(const Vector2f& other) const
{
	return { x + other.x, y + other.y };
}

Vector2f& Vector2f::operator+=(const Vector2f& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector2f Vector2f::operator-(const Vector2f& other) const
{
	return { x - other.x, y - other.y };
}

Vector2f& Vector2f::operator-=(const Vector2f& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2f Vector2f::operator-() const
{
	return { -x, -y };
}

bool Vector2f::operator==(const Vector2f &a) const
{
	return ((a.x == this->x) && (a.y == this->y));
}

bool Vector2f::operator!=(const Vector2f &a) const
{
	return !(a == *this);
}

Vector2f& Vector2f::operator*=(float scale)
{
	this->x *= scale;
	this->y *= scale;
	return *this;
}

Vector2f& Vector2f::operator/=(float scale)
{
	this->x /= scale;
	this->y /= scale;
	return *this;
}

Vector2f operator*(const Vector2f& v, float scale)
{
	return { scale * v.x, scale * v.y };
}

Vector2f operator*(float scale, const Vector2f& v)
{
	return v * scale;
}

Vector2f operator/(const Vector2f& v, float scale)
{
	return { v.x / scale, v.y / scale };
}

float dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2f norm(const Vector2f& a)
{
	float len = a.length();
	if (len == 0)
		return Vector2f{};
	return a / len;
}

Vector2f perp(const Vector2f& a)
{
	return Vector2f{ -a.y, a.x };
}

float dist(const Vector2f& a, const Vector2f& b)
{
	const Vector2f c = b - a;
	return c.length();
}