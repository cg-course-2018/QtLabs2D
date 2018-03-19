#pragma once
#include "Vector2f.h"

// Структура RectF представляет прямоугольник, стороны которого параллельны осям координат.
struct RectF
{
	RectF() = default;
	RectF(float x, float y, float width, float height);
	RectF(Vector2f topLeft, Vector2f sz);

	// Позиция левого верхнего угла.
	Vector2f origin;
	// Размеры прямоугольника (ширина, высота).
	Vector2f size;

	float leftEdge() const;
	float rightEdge() const;
	float topEdge() const;
	float bottomEdge() const;
};
