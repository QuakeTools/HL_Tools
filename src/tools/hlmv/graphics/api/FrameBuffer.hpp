#pragma once

namespace graphics::api
{
class FrameBuffer
{
protected:
	FrameBuffer(const unsigned int width, const unsigned int height)
		: _width(width)
		, _height(height)
	{
	}

public:
	FrameBuffer(FrameBuffer&&) = delete;
	FrameBuffer& operator=(FrameBuffer&&) = delete;

	unsigned int GetWidth() const { return _width; }

	unsigned int GetHeight() const { return _height; }

protected:
	unsigned int _width;
	unsigned int _height;
};
}
