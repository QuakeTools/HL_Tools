#include <fstream>
#include <stdexcept>

#include "graphics/GraphicsUtilities.hpp"

#include "graphics/api/ResourceFactory.hpp"
#include "graphics/api/Shader.hpp"

namespace graphics
{
const std::filesystem::path ShaderBasePath = std::filesystem::path{"resources"} / "shaders";
const std::string VertexShaderExtension = ".vert";
const std::string GeometryShaderExtension = ".geo";
const std::string FragmentShaderExtension = ".frag";

static const std::string& GetShaderExtension(const graphics::api::ShaderStages::ShaderStages stage)
{
	switch (stage)
	{
	case graphics::api::ShaderStages::Vertex: return VertexShaderExtension;
	case graphics::api::ShaderStages::Geometry: return GeometryShaderExtension;
	case graphics::api::ShaderStages::Fragment: return FragmentShaderExtension;

	default: throw std::invalid_argument("Unsupported shader stage");
	}
}

//TODO: move to common namespace
static std::string ReadFileContents(const std::filesystem::path& fileName)
{
	std::ifstream file{fileName};

	if (!file)
	{
		throw std::runtime_error(std::string{"Couldn't open file \""} + fileName.u8string() + "\"");
	}

	file.seekg(0, file.end);

	const auto size = static_cast<size_t>(file.tellg());

	std::string buffer(size + 1, '\0');

	file.seekg(0, file.beg);

	file.read(buffer.data(), size);

	return buffer;
}

std::shared_ptr<graphics::api::Shader> LoadShaderSource(graphics::api::ResourceFactory& factory,
	std::string&& shaderSource,
	const graphics::api::ShaderStages::ShaderStages stage)
{
	graphics::api::ShaderDescription description
	{
		stage,
		std::move(shaderSource)
	};

	return factory.CreateShader(std::move(description));
}

std::shared_ptr<graphics::api::Shader> LoadShader(
	graphics::api::ResourceFactory& factory,
	const std::filesystem::path& shaderName,
	const graphics::api::ShaderStages::ShaderStages stage)
{
	auto fileName = ShaderBasePath / shaderName;

	fileName += GetShaderExtension(stage);

	auto source = ReadFileContents(fileName);

	return LoadShaderSource(factory, std::move(source), stage);
}

std::tuple<std::shared_ptr<graphics::api::Shader>, std::shared_ptr<graphics::api::Shader>> LoadShaderSources(
	graphics::api::ResourceFactory& factory,
	std::string&& vertexShaderSource,
	std::string&& fragmentShaderSource)
{
	auto vertexShader = LoadShaderSource(factory, std::move(vertexShaderSource), graphics::api::ShaderStages::Vertex);

	auto fragmentShader = LoadShaderSource(factory, std::move(fragmentShaderSource), graphics::api::ShaderStages::Fragment);

	return {std::move(vertexShader), std::move(fragmentShader)};
}

std::tuple<std::shared_ptr<graphics::api::Shader>, std::shared_ptr<graphics::api::Shader>> LoadShaders(
	graphics::api::ResourceFactory& factory,
	const std::filesystem::path& shaderName)
{
	return std::make_tuple(
		LoadShader(factory, shaderName, graphics::api::ShaderStages::Vertex),
		LoadShader(factory, shaderName, graphics::api::ShaderStages::Fragment)
	);
}
}
