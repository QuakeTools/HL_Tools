#pragma once

#include <memory>

namespace graphics::api
{
struct BufferDescription;
class DeviceBuffer;
class Pipeline;
struct PipelineDescription;
class ResourceLayout;
struct ResourceLayoutDescription;
class ResourceSet;
struct ResourceSetDescription;
class Shader;
struct ShaderDescription;
class Texture;
struct TextureDescription;

class ResourceFactory
{
protected:
	ResourceFactory() = default;

public:
	ResourceFactory(ResourceFactory&&) = delete;
	ResourceFactory& operator=(ResourceFactory&&) = delete;

	ResourceFactory(const ResourceFactory&) = delete;
	ResourceFactory& operator=(const ResourceFactory&) = delete;

	virtual ~ResourceFactory() = 0 {}

	virtual std::shared_ptr<DeviceBuffer> CreateBuffer(const BufferDescription& description) = 0;

	virtual std::shared_ptr<Texture> CreateTexture(const TextureDescription& description) = 0;

	virtual std::shared_ptr<ResourceLayout> CreateResourceLayout(ResourceLayoutDescription&& description) = 0;

	virtual std::shared_ptr<ResourceSet> CreateResourceSet(ResourceSetDescription&& description) = 0;

	virtual std::shared_ptr<Shader> CreateShader(const ShaderDescription& description) = 0;

	virtual std::shared_ptr<Pipeline> CreatePipeline(PipelineDescription&& description) = 0;
};
}
