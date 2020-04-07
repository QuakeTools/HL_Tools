#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <glm/vec4.hpp>

#include "graphics/api/Enums.hpp"

namespace graphics::api
{
class DeviceBuffer;
class FrameBuffer;
class Pipeline;
class ResourceFactory;
class ResourceSet;
class Texture;

struct Viewport
{
	float X = 0;

	float Y = 0;

	float Width = 0;

	float Height = 0;

	float MinDepth = 0;

	float MaxDepth = 0;

	Viewport() = default;

	Viewport(float x, float y, float width, float height, float minDepth, float maxDepth)
		: X(x)
		, Y(y)
		, Width(width)
		, Height(height)
		, MinDepth(minDepth)
		, MaxDepth(maxDepth)
	{
	}

	Viewport(int x, int y, int width, int height, float minDepth, float maxDepth)
		: X(static_cast<float>(x))
		, Y(static_cast<float>(y))
		, Width(static_cast<float>(width))
		, Height(static_cast<float>(height))
		, MinDepth(minDepth)
		, MaxDepth(maxDepth)
	{
	}
};

struct GraphicsDeviceOptions
{
	bool missingUniformIsError = false;
};

class GraphicsDevice
{
protected:
	GraphicsDevice() = default;

public:
	GraphicsDevice(const GraphicsDevice&) = delete;
	GraphicsDevice& operator=(const GraphicsDevice&) = delete;

	virtual ~GraphicsDevice() = 0 {}

	virtual bool IsDepthRangeZeroToOne() const = 0;

	virtual bool IsClipSpaceYInverted() const = 0;

	virtual ResourceFactory& GetResourceFactory() = 0;

	virtual const FrameBuffer& GetSwapChainFrameBuffer() const = 0;

	virtual void ResizeMainWindow(const unsigned int width, const unsigned int height) = 0;

	virtual void SetViewport(const Viewport& viewport) = 0;

	virtual void SetFullViewport() = 0;

	virtual void SetScissorRect(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height) = 0;

	virtual void SetFullScissorRect() = 0;

	virtual void ClearColorTarget(const glm::vec4& clearColor) = 0;

	virtual void ClearDepthStencil(const float depth, const std::uint8_t stencil = 0) = 0;

	virtual void UpdateBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int bufferOffsetInBytes, const void* data, const unsigned int sizeInBytes) = 0;

	template<typename T, std::size_t SIZE>
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

	virtual void UpdateTexture(const std::shared_ptr<Texture>& texture,
		const void* pData, const unsigned int sizeInBytes,
		const unsigned int x, const unsigned int y, const unsigned int z,
		const unsigned int width, const unsigned int height, const unsigned int depth,
		const int mipLevel, const unsigned int arrayLayer) = 0;

	template<typename T, std::size_t SIZE>
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

	virtual void SetTextureFilterMode(const std::shared_ptr<Texture>& texture, const SamplerFilter filter, const bool mipmap) = 0;

	virtual void SetVertexBuffer(const unsigned int index, const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int offset = 0) = 0;

	virtual void SetIndexBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const IndexFormat format) = 0;

	virtual void DrawIndexed(unsigned int indexCount, unsigned int indexStart = 0) = 0;

	virtual void SetPipeline(const std::shared_ptr<Pipeline>& pipeline) = 0;

	virtual void SetResourceSet(unsigned int index, const std::shared_ptr<ResourceSet>& set) = 0;
};
}
