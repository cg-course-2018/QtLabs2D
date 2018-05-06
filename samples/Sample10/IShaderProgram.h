#pragma once
#include "Enumerations.h"

class IShaderProgram
{
public:
	virtual ~IShaderProgram() = default;

	// Привязывает программу к текущему контексту.
	virtual void bind() const = 0;

	// Возвращает целочисленный номер слота, через который можно
	//  установить значение uniform-переменной.
	// Возвращает -1, если программа не поддерживает такую переменную.
	virtual int getUniform(UniformId id) const = 0;

	// Возвращает целочисленный номер слота, через который можно
	//  привязать атрибут шейдера к данным VBO.
	// Возвращает -1, если программа не поддерживает такой атрибут.
	virtual int getAttribute(AttributeId id) const = 0;
};
