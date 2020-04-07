#pragma once

#include <stdexcept>

namespace graphics::api
{
class GraphicsAPIException : std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

class GlewInitException final : GraphicsAPIException
{
public:
	using GraphicsAPIException::GraphicsAPIException;
};

class OpenGLErrorException final : GraphicsAPIException
{
public:
	using GraphicsAPIException::GraphicsAPIException;
};

class ShaderCompileErrorException : public GraphicsAPIException
{
public:
	using GraphicsAPIException::GraphicsAPIException;
};

class ProgramLinkErrorException : public GraphicsAPIException
{
public:
	using GraphicsAPIException::GraphicsAPIException;
};

class UniformBufferLayoutMissingException : public GraphicsAPIException
{
public:
	using GraphicsAPIException::GraphicsAPIException;
};

class UniformLocationNotFoundException : public GraphicsAPIException
{
public:
	using GraphicsAPIException::GraphicsAPIException;
};
}
