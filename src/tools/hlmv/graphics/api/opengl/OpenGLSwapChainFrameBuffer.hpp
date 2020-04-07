#pragma once

#include "graphics/api/FrameBuffer.hpp"

namespace graphics::api::opengl
{
class OpenGLSwapChainFrameBuffer : public FrameBuffer
{
public:
	OpenGLSwapChainFrameBuffer(const unsigned int width, const unsigned int height)
		: FrameBuffer(width, height)
	{
	}

	void Resize(const unsigned int width, const unsigned int height)
	{
		_width = width;
		_height = height;
	}
};
}
