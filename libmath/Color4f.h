#pragma once
struct Color4f
{
public:
	Color4f();
	Color4f(float r, float g, float b, float a = 1.0f);
	~Color4f() {}

	float r;
	float g;
	float b;
	float a;
};

