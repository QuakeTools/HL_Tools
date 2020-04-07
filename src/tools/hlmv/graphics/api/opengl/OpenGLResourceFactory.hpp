#pragma once

#include <memory>

#include "graphics/api/ResourceFactory.hpp"

namespace graphics::api::opengl
{
class OpenGLResourceFactory final : public ResourceFactory
{
public:
	OpenGLResourceFactory() = default;

	OpenGLResourceFactory(OpenGLResourceFactory&&) = delete;
	OpenGLResourceFactory& operator=(OpenGLResourceFactory&&) = delete;

	OpenGLResourceFactory(const OpenGLResourceFactory&) = delete;
	OpenGLResourceFactory& operator=(const OpenGLResourceFactory&) = delete;

	~OpenGLResourceFactory() = default;

	std::shared_ptr<DeviceBuffer> CreateBuffer(const BufferDescription& description);

	std::shared_ptr<Texture> CreateTexture(const TextureDescription& description);

	std::shared_ptr<ResourceLayout> CreateResourceLayout(ResourceLayoutDescription&& description);

	std::shared_ptr<ResourceSet> CreateResourceSet(ResourceSetDescription&& description);

	std::shared_ptr<Shader> CreateShader(const ShaderDescription& description);

	std::shared_ptr<Pipeline> CreatePipeline(PipelineDescription&& description);
};
}
