#pragma once
#include "Enumerations.h"

class IShaderProgram
{
public:
	virtual ~IShaderProgram() = default;

	// ѕрив€зывает программу к текущему контексту.
	virtual void bind() const = 0;

	// ¬озвращает целочисленный номер слота, через который можно
	//  установить значение uniform-переменной.
	// ¬озвращает -1, если программа не поддерживает такую переменную.
	virtual int getUniform(UniformId id) const = 0;

	// ¬озвращает целочисленный номер слота, через который можно
	//  прив€зать атрибут шейдера к данным VBO.
	// ¬озвращает -1, если программа не поддерживает такой атрибут.
	virtual int getAttribute(AttributeId id) const = 0;
};
