#pragma once

#include "IShaderProgram.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// Представляет источник света, отправляемый шейдеру.
struct LightSource
{
	// (x, y, z, 1) означает точечный источник света (positioned light).
	// (x, y, z, 0) означает направленный источник света (directed light).
	glm::vec4 position{ 0 };
	glm::vec4 diffuse{ 0 };
	glm::vec4 specular{ 0 };
};

namespace utils
{
LightSource makeDirectedLightSource(const glm::vec3 &direction, const glm::vec4 &diffuse, const glm::vec4 &specular);
LightSource makePositionedLightSource(const glm::vec3 &position, const glm::vec4 &diffuse, const glm::vec4 &specular);

void setLightSource0(const IShaderProgram &program, const LightSource &light);
void setLightSource1(const IShaderProgram &program, const LightSource &light);
void setLightSource2(const IShaderProgram &program, const LightSource &light);
} // namespace utils
