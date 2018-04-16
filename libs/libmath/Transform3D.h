#pragma once
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

// Преобразует координаты из локальных в мировые в следующем порядке:
//  — сначала вершины масштабируются
//    например, единичный цилиндр превращается в диск или в трубку
//  — затем поворачиваются
//    т.е. тела ориентируются в пространстве
//  — затем переносятся
//    т.е. задаётся положение тела
// изменив порядок, мы изменили бы значение трансформаций.
struct Transform3D
{
public:
	glm::vec3 m_size{ 1, 1, 1 };
	glm::quat m_orientation{ glm::angleAxis(0.f, glm::vec3(0, 1, 0)) };
	glm::vec3 m_position{ 0, 0, 0 };

	// Добавляет вращение к текущему ориентирующему вращению.
	// См. https://math.stackexchange.com/questions/360286
	void rotateBy(const glm::quat &rotation);

	// Добавляет масштабирование с указанными множителями масштабирования для трёх осей.
	void scaleBy(const glm::vec3 &scale);

	// Добавляет масштабирование с указанным множителем масштабирования.
	void scaleBy(const float scale);

	// Добавляет перемещение на указанное расстояние.
	void moveBy(const glm::vec3 &distance);

	// Создаёт матрицу 4x4 для выполнения аффинного преобразования, эквивалентного текущему состоянию класса.
	glm::mat4 toMat4() const;

	// Применяет преобразование к указанной точке.
	glm::vec3 transformPoint(const glm::vec3 &point) const;
};
