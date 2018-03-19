#include "stdafx.h"
#include "RectF.h"

Rect2F::Rect2F(float x, float y, float width, float height)
	: origin(x, y), size(width, height)
{
}

Rect2F::Rect2F(Vector2f topLeft, Vector2f sz)
	: origin(topLeft), size(sz)
{
}

float Rect2F::leftEdge() const
{
	return origin.x;
}

float Rect2F::rightEdge() const
{
	return origin.x + size.x;
}

float Rect2F::topEdge() const
{
	return origin.y;
}

float Rect2F::bottomEdge() const
{
	return origin.y + size.y;
}
