#pragma once

// Список стандартных Uniform-переменных, используемых 3D рендерером.
enum UniformId : unsigned
{
    // Фаза анимации, float
    UniformAnimationPhase,
    // Матрица проецирования, mat4
	UniformProjectionMatrix,
	// Цвет текущего прохода рисования (цвет точки, контура или заливки), vec4
	UniformCurrentColor,
};

// Список стандартных атрибутов, используемых 3D рендерером.
enum AttributeId : unsigned
{
    // Позиция вершины в локальной системе координат объекта, vec3
	AttributePosition,
    // Текстурные координаты, vec2
    AttributeTexCoord,
    // RGB/RGBA цвет вершины, vec3 или vec4
	AttributeColor,
};
