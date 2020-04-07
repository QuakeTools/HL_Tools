#include <algorithm>
#include <stdexcept>

#include "graphics/api/Texture.hpp"

#include "graphics/api/opengl/OpenGLEnumConverters.hpp"
#include "graphics/api/opengl/OpenGLTexture.hpp"
#include "graphics/api/opengl/OpenGLUtils.hpp"

using namespace graphics::api::opengl::detail;

namespace graphics::api::opengl
{
OpenGLTexture::OpenGLTexture(const TextureDescription& description)
	: _width(description.Width)
	, _height(description.Height)
	, _depth(description.Depth)
	, _mipLevels(description.MipLevels)
	, _format(PixelFormatToFormat(description.Format))
	, _internalFormat(PixelFormatToInternalFormat(description.Format))
	, _pixelType(PixelFormatToPixelType(description.Format))
	, _type(TextureTypeToType(description.Type))
{
	glGenTextures(1, &_texture);

	glBindTexture(_type, _texture);

	auto levelWidth = _width;
	auto levelHeight = _height;
	auto levelDepth = _depth;

	for (unsigned int mipLevel = 0; mipLevel < _mipLevels; ++mipLevel)
	{
		switch (_type)
		{
		case GL_TEXTURE_1D:
			glTexImage1D(GL_TEXTURE_1D, mipLevel, _internalFormat, levelWidth, 0, _format, _pixelType, nullptr);
			break;

		case GL_TEXTURE_2D:
			glTexImage2D(GL_TEXTURE_2D, mipLevel, _internalFormat, levelWidth, levelHeight, 0, _format, _pixelType, nullptr);
			break;

		case GL_TEXTURE_3D:
			glTexImage3D(GL_TEXTURE_2D, mipLevel, _internalFormat, levelWidth, levelHeight, levelDepth, 0, _format, _pixelType, nullptr);
			break;

		case GL_TEXTURE_CUBE_MAP:
		{
			for (int face = 0; face < 6; face++)
			{
				// Set size, load empty data into texture
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
					mipLevel,
					_internalFormat,
					levelWidth,
					levelHeight,
					0,
					_format,
					_pixelType,
					nullptr);
			}
			break;
		}

		default: throw std::invalid_argument("Unsupported texture type in Texture::Texture");
		}

		levelWidth = std::max(1U, levelWidth / 2);
		levelHeight = std::max(1U, levelHeight / 2);
		levelDepth = std::max(1U, levelDepth / 2);

		CheckGLError();
	}
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &_texture);
}
}
