#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include <glm/vec4.hpp>

#include "graphics/api/Enums.hpp"
#include "graphics/api/Pipeline.hpp"

namespace graphics::api::opengl
{
struct UniformBufferBinding
{
	UniformBufferBinding(const GLuint location)
		: Location(location)
	{
	}

	const GLuint Location = 0;
};

struct TextureBinding
{
	TextureBinding(const GLenum textureUnit, const GLint location)
		: TextureUnit(textureUnit)
		, Location(location)
	{
	}

	const GLenum TextureUnit;
	const GLint Location;
};

struct BoundResourceLayout
{
protected:
	friend class OpenGLPipeline;

public:
	BoundResourceLayout() = default;
	BoundResourceLayout(BoundResourceLayout&&) = default;
	BoundResourceLayout& operator=(BoundResourceLayout&&) = default;
	~BoundResourceLayout() = default;

	BoundResourceLayout(
		std::unordered_map<unsigned int, std::unique_ptr<UniformBufferBinding>>&& uniforms,
		std::unordered_map<unsigned int, std::unique_ptr<TextureBinding>>&& textures)
		: _uniforms(std::move(uniforms))
		, _textures(std::move(textures))
	{
	}

protected:
	bool GetUniformBinding(const unsigned int element, const UniformBufferBinding*& pOutBinding)
	{
		if (auto it = _uniforms.find(element); it != _uniforms.end())
		{
			pOutBinding = it->second.get();

			return true;
		}

		return false;
	}

	bool GetTextureBinding(const unsigned int element, const TextureBinding*& pOutBinding)
	{
		if (auto it = _textures.find(element); it != _textures.end())
		{
			pOutBinding = it->second.get();

			return true;
		}

		return false;
	}

private:
	std::unordered_map<unsigned int, std::unique_ptr<UniformBufferBinding>> _uniforms;
	std::unordered_map<unsigned int, std::unique_ptr<TextureBinding>> _textures;
};

struct BoundVertexElement
{
	BoundVertexElement() = default;
	BoundVertexElement(BoundVertexElement&&) = default;
	BoundVertexElement& operator=(BoundVertexElement&&) = default;

	BoundVertexElement(VertexElementDescription&& description, const GLint location)
		: Description(std::move(description))
		, Location(location)
	{
	}

	VertexElementDescription Description;
	GLint Location = -1;
};

struct BoundVertexLayout
{
	std::vector<BoundVertexElement> Elements;
	unsigned int Stride = 0;
};

class OpenGLPipeline final : public Pipeline
{
public:
	OpenGLPipeline(PipelineDescription&& description);

	OpenGLPipeline() = delete;

	OpenGLPipeline(OpenGLPipeline&&) = delete;
	OpenGLPipeline& operator=(OpenGLPipeline&&) = delete;

	OpenGLPipeline(const OpenGLPipeline&) = delete;
	OpenGLPipeline& operator=(const OpenGLPipeline&) = delete;

	~OpenGLPipeline();

	GLuint GetProgram() const { return _program; }

	const BlendStateDescription& GetBlendState() const { return _blendState; }

	const DepthStencilStateDescription& GetDepthStencilState() const { return _depthStencilState; }

	const RasterizerStateDescription& GetRasteriserState() const { return _rasteriserState; }

	PrimitiveTopology GetPrimitiveTopology() const { return _primitiveTopology; }

	const std::vector<BoundVertexLayout>& GetVertexLayouts() const { return _vertexLayouts; }

	const std::vector<std::shared_ptr<ResourceLayout>>& GetResourceLayouts() const { return _resourceLayouts; }

	const std::vector<BoundResourceLayout>& GetBoundResourceLayouts() const { return _boundResourceLayouts; }

	bool GetUniformBinding(const unsigned int slot, const unsigned int element, const UniformBufferBinding*& pOutBinding)
	{
		return _boundResourceLayouts[slot].GetUniformBinding(element, pOutBinding);
	}

	bool GetTextureBinding(const unsigned int slot, const unsigned int element, const TextureBinding*& pOutBinding)
	{
		return _boundResourceLayouts[slot].GetTextureBinding(element, pOutBinding);
	}

private:
	GLuint _program;

	const BlendStateDescription _blendState;

	const DepthStencilStateDescription _depthStencilState;

	const RasterizerStateDescription _rasteriserState;

	const PrimitiveTopology _primitiveTopology;

	std::vector<BoundVertexLayout> _vertexLayouts;

	const std::vector<std::shared_ptr<ResourceLayout>> _resourceLayouts;

	std::vector<BoundResourceLayout> _boundResourceLayouts;
};
}
