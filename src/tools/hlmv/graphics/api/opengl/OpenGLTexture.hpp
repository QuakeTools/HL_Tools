#pragma once

#include <GL/glew.h>

#include "graphics/api/Texture.hpp"

namespace graphics::api::opengl
{
class OpenGLTexture final : public Texture
{
public:
	OpenGLTexture(const TextureDescription& description);

	OpenGLTexture() = delete;

	OpenGLTexture(Texture&&) = delete;
	OpenGLTexture& operator=(Texture&&) = delete;

	OpenGLTexture(const Texture&) = delete;
	OpenGLTexture& operator=(const Texture&) = delete;

	~OpenGLTexture();

	unsigned int GetWidth() const { return _width; }

	unsigned int GetHeight() const { return _height; }

	unsigned int GetDepth() const { return _depth; }

	GLuint GetId() const { return _texture; }

	GLenum GetFormat() const { return _format; }

	GLenum GetInternalFormat() const { return _internalFormat; }

	GLenum GetPixelType() const { return _pixelType; }

	GLenum GetType() const { return _type; }

private:
	GLuint _texture;

	const unsigned int _width;
	const unsigned int _height;
	const unsigned int _depth;
	const unsigned int _mipLevels;
	const GLenum _format;
	const GLenum _internalFormat;
	const GLenum _pixelType;
	const GLenum _type;
};
}
