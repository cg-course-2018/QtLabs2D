#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

struct VertexP2C4
{
	glm::vec2 xy;
	glm::vec4 rgba;
};

// ���������� ������ ������ ������������� �� ������ ����� �������������, ������������� ������� � ��������� ������.
//  @param center - �������������� ����� ��������������
//  @param hullPoints - ������� ������
//  @param fillColor - ���� ���������� �������������
std::vector<VertexP2C4> tesselateTriangleFan(const glm::vec2 &center, const std::vector<glm::vec2> &hullPoints, const glm::vec4 &fillColor);

// ���������� ������ ������ ������������� ��� ��������� ��������������, ��������� ���������.
std::vector<VertexP2C4> tesselateConvex(const std::vector<glm::vec2> &verticies, const glm::vec4 &fillColor);

// ������� ����� ���� �� ������������,
//  ���������� ������ � ��������� �������������.
std::vector<VertexP2C4> tesselateCircle(float radius, const glm::vec2 &center, const glm::vec4 &fillColor);

// ���������� ������ ������ ������������� ��� ������ Polar Rose (�������� ����),
//  ���������� ������ � ��������� �������������.
//  @param outerRadius - ������ ����������, ����������� �������� ����
//  @param petelsCount - ����� ���������, �� ����� 3
//  @param center - �������������� ����� ������
//  @param fillColor - ���� ���������� �������������
std::vector<VertexP2C4> tesselatePolarRose(float outerRadius, unsigned petelsCount, const glm::vec2 &center, const glm::vec4 &fillColor);
