#include <stdexcept>

#include <GL/glew.h>

#include "graphics/api/opengl/OpenGLDeviceBuffer.hpp"
#include "graphics/api/opengl/OpenGLEnumConverters.hpp"
#include "graphics/api/opengl/OpenGLUtils.hpp"

using namespace graphics::api::opengl::detail;

namespace graphics::api::opengl
{
OpenGLDeviceBuffer::OpenGLDeviceBuffer(const BufferDescription& description)
	: DeviceBuffer()
	, _sizeInBytes(description.SizeInBytes)
	, _bufferUsage(description.Usage)
{
	glGenBuffers(1, &_buffer);

	const auto target = BufferUsageToTarget(GetUsage());

	glBindBuffer(target, GetId());
	CheckGLError();

	const auto usage = BufferUsageToUsage(GetUsage());

	glBufferData(target, _sizeInBytes, nullptr, usage);
	CheckGLError();
}

OpenGLDeviceBuffer::~OpenGLDeviceBuffer()
{
	glDeleteBuffers(1, &_buffer);
}
}
