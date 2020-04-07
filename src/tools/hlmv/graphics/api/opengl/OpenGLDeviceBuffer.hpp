#pragma once

#include <GL/glew.h>

#include "graphics/api/DeviceBuffer.hpp"

namespace graphics::api::opengl
{
class OpenGLDeviceBuffer final : public DeviceBuffer
{
public:
	OpenGLDeviceBuffer(const BufferDescription& description);

	OpenGLDeviceBuffer() = delete;

	OpenGLDeviceBuffer(OpenGLDeviceBuffer&&) = delete;
	OpenGLDeviceBuffer& operator=(OpenGLDeviceBuffer&&) = delete;

	OpenGLDeviceBuffer(const OpenGLDeviceBuffer&) = delete;
	OpenGLDeviceBuffer& operator=(const OpenGLDeviceBuffer&) = delete;

	virtual ~OpenGLDeviceBuffer();

	unsigned int GetSizeInBytes() const override { return _sizeInBytes; }

	std::uint8_t GetUsage() const override { return _bufferUsage; }

	GLuint GetId() const { return _buffer; }

private:
	unsigned int _sizeInBytes = 0;
	std::uint8_t _bufferUsage = 0;

	GLuint _buffer = 0;
};
}
