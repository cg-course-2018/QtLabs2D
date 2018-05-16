#include "stdafx.h"
#include "LightSource.h"
#include <glbinding/gl32core/gl.h>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace gl32core;

LightSource utils::makeDirectedLightSource(const glm::vec3 &direction, const glm::vec4 &diffuse, const glm::vec4 &specular)
{
	const glm::vec3 normalized = glm::normalize(direction);
	LightSource source;
	source.position = glm::vec4(normalized.x, normalized.y, normalized.z, 0);
	source.diffuse = diffuse;
	source.specular = specular;

	return source;
}

LightSource utils::makePositionedLightSource(const glm::vec3 &position, const glm::vec4 &diffuse, const glm::vec4 &specular)
{
	LightSource source;
	source.position = glm::vec4(position.x, position.y, position.z, 1);
	source.diffuse = diffuse;
	source.specular = specular;

	return source;
}

void utils::setLightSource0(const IShaderProgram &program, const LightSource &light)
{
	if (int loc = program.getUniform(UniformLight0Position); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.position));
	}
	if (int loc = program.getUniform(UniformLight0Diffuse); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.diffuse));
	}
	if (int loc = program.getUniform(UniformLight0Specular); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.specular));
	}
}

void utils::setLightSource1(const IShaderProgram &program, const LightSource &light)
{
	if (int loc = program.getUniform(UniformLight1Position); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.position));
	}
	if (int loc = program.getUniform(UniformLight1Diffuse); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.diffuse));
	}
	if (int loc = program.getUniform(UniformLight1Specular); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.specular));
	}
}

void utils::setLightSource2(const IShaderProgram &program, const LightSource &light)
{
	if (int loc = program.getUniform(UniformLight2Position); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.position));
	}
	if (int loc = program.getUniform(UniformLight2Diffuse); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.diffuse));
	}
	if (int loc = program.getUniform(UniformLight2Specular); loc != -1)
	{
		glUniform4fv(loc, 1, glm::value_ptr(light.specular));
	}
}
