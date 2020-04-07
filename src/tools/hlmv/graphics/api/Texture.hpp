#pragma once

#include "BindableResource.hpp"
#include "Enums.hpp"

namespace graphics::api
{
struct TextureDescription
{
	const unsigned int Width;
	const unsigned int Height;
	const unsigned int Depth;

	const unsigned int MipLevels;

	const PixelFormat Format;
	const TextureType Type;

	static TextureDescription Texture1D(const unsigned int width, const unsigned int mipLevels, const PixelFormat format)
	{
		return TextureDescription{width, 0, 0, mipLevels, format, TextureType::Texture1D};
	}

	static TextureDescription Texture2D(const unsigned int width, const unsigned int height, const unsigned int mipLevels, const PixelFormat format)
	{
		return TextureDescription{width, height, 0, mipLevels, format, TextureType::Texture2D};
	}

	static TextureDescription Texture3D(const unsigned int width, const unsigned int height, const unsigned int depth, const unsigned int mipLevels, const PixelFormat format)
	{
		return TextureDescription{width, height, depth, mipLevels, format, TextureType::Texture3D};
	}

	static TextureDescription Cubemap(const unsigned int width, const unsigned int height, const unsigned int mipLevels, const PixelFormat format)
	{
		return TextureDescription{width, height, 0, mipLevels, format, TextureType::Cubemap};
	}
};

class Texture : public BindableResource
{
protected:
	Texture() = default;

public:
	Texture(Texture&&) = delete;
	Texture& operator=(Texture&&) = delete;

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	virtual ~Texture() = 0 {}

	virtual unsigned int GetWidth() const = 0;

	virtual unsigned int GetHeight() const = 0;

	virtual unsigned int GetDepth() const = 0;
};
}
