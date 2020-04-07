#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "graphics/GraphicsUtilities.hpp"

#include "graphics/api/Enums.hpp"

namespace graphics
{
namespace api
{
class ResourceFactory;
class Shader;
}

std::shared_ptr<graphics::api::Shader> LoadShaderSource(graphics::api::ResourceFactory& factory,
	std::string&& shaderSource,
	const graphics::api::ShaderStages::ShaderStages stage);

std::shared_ptr<graphics::api::Shader> LoadShader(
	graphics::api::ResourceFactory& factory,
	const std::filesystem::path& shaderName,
	const graphics::api::ShaderStages::ShaderStages stage);

std::tuple<std::shared_ptr<graphics::api::Shader>, std::shared_ptr<graphics::api::Shader>> LoadShaderSources(
	graphics::api::ResourceFactory& factory,
	std::string&& vertexShaderSource,
	std::string&& fragmentShaderSource);

std::tuple<std::shared_ptr<graphics::api::Shader>, std::shared_ptr<graphics::api::Shader>> LoadShaders(
	graphics::api::ResourceFactory& factory,
	const std::filesystem::path& shaderName);
}
