#pragma once

#include <string>

#include "Enums.hpp"

namespace graphics::api
{
struct ShaderDescription
{
	ShaderDescription() = delete;

	ShaderDescription(ShaderDescription&&) = default;
	ShaderDescription& operator=(ShaderDescription&&) = default;

	ShaderDescription(const ShaderStages::ShaderStages stage, std::string&& sourceCode)
		: Stage(stage)
		, SourceCode(std::move(sourceCode))
	{
	}

	const ShaderStages::ShaderStages Stage;
	const std::string SourceCode;
};

class Shader
{
protected:
	Shader() = default;

public:
	Shader(Shader&&) = delete;
	Shader& operator=(Shader&&) = delete;

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	virtual ~Shader() = 0 {}
};
}
