#pragma once

// Список стандартных Uniform-переменных, используемых 3D рендерером.
enum UniformId : unsigned
{
	// Фаза анимации, float
	UniformAnimationPhase,
	// Матрица проецирования, mat4
	UniformProjectionMatrix,
	// Матрица преобразования из координат объекта в глобальные координаты, mat4
	UniformWorldMatrix,
	// Матрица преобразования из глобальных координат в координаты камеры, mat4
	UniformViewMatrix,
	// Матрица преобразования нормалей из локальных координат в глобальные координаты, mat4
	UniformNormalWorldMatrix,

	// Положение виртуальной камеры, vec3
	UniformViewerPosition,

	// Параметры источников света №0..№2, vec4
	UniformLight0Position,
	UniformLight0Diffuse,
	UniformLight0Specular,
	UniformLight1Position,
	UniformLight1Diffuse,
	UniformLight1Specular,
	UniformLight2Position,
	UniformLight2Diffuse,
	UniformLight2Specular,

	// Текстура (отображение текстурных координат на цвет материала)
	UniformColorMap,
	// Прямоугольник текстуры в текстурных координатах, vec4 (x, y, w, h).
	UniformColorMapRect,

	// Специальное значение, равное числу констант enum.
	UniformIdCount
};

// Список стандартных атрибутов, используемых 3D рендерером.
enum AttributeId : unsigned
{
	// Позиция вершины в локальной системе координат объекта, vec3
	AttributePosition,
	// Нормаль к касательной плоскости к поверхности в вершине, vec3
	AttributeNormal,
	// Текстурные координаты, vec2
	AttributeTexCoord,
	// RGB/RGBA цвет вершины, vec3 или vec4
	AttributeColor,
	// Позиция экземпляра объекта в локальной системе координат, vec3
	AttributeInstancePosition,

	// Специальное значение, равное числу констант enum.
	AttributeIdCount
};
