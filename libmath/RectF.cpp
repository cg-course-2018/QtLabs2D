#include "stdafx.h"
#include "RectF.h"

RectF::RectF(float x, float y, float width, float height)
	: origin(x, y), size(width, height)
{
}

RectF::RectF(Vector2f topLeft, Vector2f sz)
	: origin(topLeft), size(sz)
{
}

float RectF::leftEdge() const
{
	return origin.x;
}

float RectF::rightEdge() const
{
	return origin.x + size.x;
}

float RectF::topEdge() const
{
	return origin.y;
}

float RectF::bottomEdge() const
{
	return origin.y + size.y;
}
