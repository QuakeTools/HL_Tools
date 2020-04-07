#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include <GL/glew.h>

#include <glm/vec4.hpp>

#include "graphics/api/Enums.hpp"
#include "graphics/api/Exceptions.hpp"
#include "graphics/api/GraphicsDevice.hpp"
#include "graphics/api/opengl/OpenGLResourceFactory.hpp"

namespace graphics::api::opengl
{
class OpenGLDeviceBuffer;
class OpenGLPipeline;
class OpenGLSwapChainFrameBuffer;

class OpenGLGraphicsDevice final : public GraphicsDevice
{
public:
	OpenGLGraphicsDevice(const unsigned int width, const unsigned int height, const GraphicsDeviceOptions& options);

	OpenGLGraphicsDevice(const OpenGLGraphicsDevice&) = delete;
	OpenGLGraphicsDevice& operator=(const OpenGLGraphicsDevice&) = delete;

	~OpenGLGraphicsDevice();

	bool IsDepthRangeZeroToOne() const { return false; }

	bool IsClipSpaceYInverted() const { return false; }

	ResourceFactory& GetResourceFactory() { return _resourceFactory; }

	const FrameBuffer& GetSwapChainFrameBuffer() const;

	void ResizeMainWindow(const unsigned int width, const unsigned int height);

	void SetViewport(const Viewport& viewport);

	void SetFullViewport();

	void SetScissorRect(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height);

	void SetFullScissorRect();

	void ClearColorTarget(const glm::vec4& clearColor);

	void ClearDepthStencil(const float depth, const std::uint8_t stencil = 0);

	void UpdateBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int bufferOffsetInBytes, const void* pData, const unsigned int sizeInBytes);

	template<typename T, size_t SIZE>
	void UpdateBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int bufferOffsetInBytes, const std::array<T, SIZE>& data)
	{
		UpdateBuffer(buffer, bufferOffsetInBytes, data.data(), data.size() * sizeof(T));
	}

	template<typename T>
	void UpdateBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int bufferOffsetInBytes, const std::vector<T>& data)
	{
		UpdateBuffer(buffer, bufferOffsetInBytes, data.data(), data.size() * sizeof(T));
	}

	template<typename T>
	void UpdateBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int bufferOffsetInBytes, const T& data)
	{
		UpdateBuffer(buffer, bufferOffsetInBytes, &data, sizeof(T));
	}

	void UpdateTexture(const std::shared_ptr<Texture>& texture,
		const void* pData, const unsigned int sizeInBytes,
		const unsigned int x, const unsigned int y, const unsigned int z,
		const unsigned int width, const unsigned int height, const unsigned int depth,
		const int mipLevel, const unsigned int arrayLayer);

	template<typename T, size_t SIZE>
	void UpdateTexture(const std::shared_ptr<Texture>& texture,
		const std::array<T, SIZE>& data,
		const unsigned int x, const unsigned int y, const unsigned int z,
		const unsigned int width, const unsigned int height, const unsigned int depth,
		const int mipLevel, const unsigned int arrayLayer)
	{
		UpdateTexture(texture,
			data.data(), data.size() * sizeof(T),
			x, y, z,
			width, height, depth,
			mipLevel, arrayLayer);
	}

	template<typename T>
	void UpdateTexture(const std::shared_ptr<Texture>& texture,
		const std::vector<T>& data,
		const unsigned int x, const unsigned int y, const unsigned int z,
		const unsigned int width, const unsigned int height, const unsigned int depth,
		const int mipLevel, const unsigned int arrayLayer)
	{
		UpdateTexture(texture,
			data.data(), data.size() * sizeof(T),
			x, y, z,
			width, height, depth,
			mipLevel, arrayLayer);
	}

	template<typename T>
	void UpdateTexture(const std::shared_ptr<Texture>& texture,
		const T& data,
		const unsigned int x, const unsigned int y, const unsigned int z,
		const unsigned int width, const unsigned int height, const unsigned int depth,
		const int mipLevel, const unsigned int arrayLayer)
	{
		UpdateTexture(texture,
			&data, sizeof(T),
			x, y, z,
			width, height, depth,
			mipLevel, arrayLayer);
	}

	void SetTextureFilterMode(const std::shared_ptr<Texture>& texture, const SamplerFilter filter, const bool mipmap);

	void SetVertexBuffer(const unsigned int index, const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int offset = 0);

	void SetIndexBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const IndexFormat format);

	void DrawIndexed(unsigned int indexCount, unsigned int indexStart = 0);

	void SetPipeline(const std::shared_ptr<Pipeline>& pipeline);

	void SetResourceSet(unsigned int index, const std::shared_ptr<ResourceSet>& set);

private:
	void ActivateGraphicsPipeline();

	void FlushResourceSets();
	void FlushVertexLayouts();

private:
	const GraphicsDeviceOptions _options;

	OpenGLResourceFactory _resourceFactory;

	GLuint _vao{};

	std::unique_ptr<OpenGLSwapChainFrameBuffer> _swapChainFrameBuffer;

	Viewport _viewport;

	IndexFormat _indexFormat = IndexFormat::UInt16;

	std::shared_ptr<OpenGLPipeline> _pipeline;

	GLenum _primitiveType = GL_TRIANGLES;

	std::vector<std::shared_ptr<ResourceSet>> _resourceSets;

	std::vector<std::shared_ptr<OpenGLDeviceBuffer>> _vertexBuffers;
	std::vector<unsigned int> _vertexOffsets;

	GLuint _activeVertexAttributes = 0;
};
}
