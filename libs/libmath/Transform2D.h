#pragma once
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace math
{

// ����������� ���������� �� ��������� � ������� � ��������� �������:
//  - ������� ������� ��������������
//    ��������, ��������� ���� ����� ������������ � ������
//  - ����� ��������������
//    �.�. ������ ������������� �� ���������
//  - ����� �����������
//    �.�. ������� ��������� ����
// ������� �������, �� �������� �� �������� �������������.
struct Transform2D
{
public: // ������� ������ ������������ ������ ����.
	glm::vec2 position{ 0, 0 };

	// ���� ������������ �������� ������ � ��������.
	float orientation{ 0 };

	// ��������� ������� ������ �� ���� ����.
	glm::vec2 size{ 1, 1 };

	// ��������� �������� �� �������� ����� ������.
	void rotateBy(float radians);

	// ��������� ��������������� � ���������� ����������� ��������������� ��� ���� ����.
	void scaleBy(const glm::vec2 &scale);

	// ��������� ��������������� � ��������� ���������� ���������������.
	void scaleBy(const float scale);

	// ��������� ����������� �� ��������� ����������.
	void moveBy(const glm::vec2 &distance);

	// ������ ������� 3x3 ��� ���������� ��������� ��������������, �������������� �������� ��������� ������.
	glm::mat3 toMat3() const;

	// ������ ������� 4x4 ��� ���������� ��������� ��������������, �������������� �������� ��������� ������.
	glm::mat4 toMat4() const;
};

} // namespace math