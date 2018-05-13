#pragma once

#include "IShaderProgram.h"
#include "libglcore/libglcore.h"
#include <string>
#include <unordered_map>
#include <vector>

using UniformInfo = std::pair<UniformId, std::string>;
using AttributeInfo = std::pair<AttributeId, std::string>;

class ShaderProgram final : public IShaderProgram
{
public:
	void init(glcore::ProgramObject program, std::vector<UniformInfo> &uniformNames, std::vector<AttributeInfo> attributeNames);

	void bind() const final;
	int getUniform(UniformId id) const final;
	int getAttribute(AttributeId id) const final;

private:
	using UniformMap = std::unordered_map<UniformId, int>;
	using AttributeMap = std::unordered_map<AttributeId, int>;

	void initUniforms(const std::vector<UniformInfo> &uniformNames);
	void initAttributes(const std::vector<AttributeInfo> &attributeNames);

	glcore::ProgramObject m_program;
	UniformMap m_uniforms;
	AttributeMap m_attributes;
};
