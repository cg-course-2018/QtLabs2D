#pragma once

// —труктура, моделирующа€ двумерный вектор
struct Vector2f
{
	float x = 0;
	float y = 0;

	Vector2f() = default;
	Vector2f(float x, float y);

	float length() const;
	void normalize();
	float projectOn(const Vector2f& other) const;

	Vector2f operator+(const Vector2f& other) const;
	Vector2f& operator+=(const Vector2f& other);
	Vector2f operator-(const Vector2f& other) const;
	Vector2f& operator-=(const Vector2f& other);
	Vector2f operator-() const;
	bool operator==(const Vector2f &a) const;
	bool operator!=(const Vector2f &a) const;
	Vector2f& operator*=(float scale);
	Vector2f& operator/=(float scale);
};

Vector2f operator*(const Vector2f& v, float scale);
Vector2f operator*(float scale, const Vector2f& v);
Vector2f operator/(const Vector2f& v, float scale);

float dot(const Vector2f& a, const Vector2f& b);
Vector2f norm(const Vector2f& a);
Vector2f perp(const Vector2f& a);
float dist(const Vector2f& a, const Vector2f& b);