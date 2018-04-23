#pragma once
#include "Enumerations.h"

class IShaderProgram
{
public:
	virtual ~IShaderProgram() = default;

	// ����������� ��������� � �������� ���������.
	virtual void bind() const = 0;

	// ���������� ������������� ����� �����, ����� ������� �����
	//  ���������� �������� uniform-����������.
	// ���������� -1, ���� ��������� �� ������������ ����� ����������.
	virtual int getUniform(UniformId id) const = 0;

	// ���������� ������������� ����� �����, ����� ������� �����
	//  ��������� ������� ������� � ������ VBO.
	// ���������� -1, ���� ��������� �� ������������ ����� �������.
	virtual int getAttribute(AttributeId id) const = 0;
};
