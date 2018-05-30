#include "stdafx.h"
#include "ShaderProgram.h"
#include <glbinding/gl33core/gl.h>

using namespace gl33core;

void ShaderProgram::init(glcore::ProgramObject program, std::vector<UniformInfo> &uniformNames, std::vector<AttributeInfo> attributeNames)
{
	m_program = std::move(program);
	initUniforms(uniformNames);
	initAttributes(attributeNames);
}

void ShaderProgram::bind() const
{
	glUseProgram(m_program.get());
}

int ShaderProgram::getUniform(UniformId id) const
{
	// Выполняем поиск слота переменной по ID.
	const auto it = m_uniforms.find(id);
	if (it != m_uniforms.end())
	{
		return it->second;
	}

	// Шейдер не предоставляет такой переменной.
	return -1;
}

int ShaderProgram::getAttribute(AttributeId id) const
{
	// Выполняем поиск слота атрибута по ID.
	const auto it = m_attributes.find(id);
	if (it != m_attributes.end())
	{
		return it->second;
	}

	// Шейдер не запрашивает такого атрибута.
	return -1;
}

void ShaderProgram::initUniforms(const std::vector<UniformInfo> &uniformNames)
{
	m_uniforms.clear();
	for (const auto &info : uniformNames)
	{
		// Запрашиваем у драйвера номер слота uniform-переменной.
		const std::string &name = info.second;
		const int location = glGetUniformLocation(m_program.get(), name.data());
		if (location == -1)
		{
			throw std::runtime_error("wrong uniform variable name: " + name);
		}

		// Сохраняем имя и номер слота переменной.
		m_uniforms.emplace(info.first, location);
	}
}

void ShaderProgram::initAttributes(const std::vector<AttributeInfo> &attributeNames)
{
	m_attributes.clear();
	for (const auto &info : attributeNames)
	{
		// Запрашиваем у драйвера номер слота атрибута.
		const std::string &name = info.second;
		const int location = glGetAttribLocation(m_program.get(), name.data());
		if (location == -1)
		{
			throw std::runtime_error("wrong vertex attribute name: " + name);
		}

		// Сохраняем имя и номер слота атрибута.
		m_attributes.emplace(info.first, location);
	}
}
