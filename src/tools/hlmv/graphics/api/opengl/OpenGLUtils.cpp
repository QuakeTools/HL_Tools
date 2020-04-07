#include <stdexcept>
#include <string>

#if WIN32
#include <Windows.h>

auto DEBUG_BREAK() { return __debugbreak(); }
#else
#include <signal.h>

auto DEBUG_BREAK() { return raise(SIGTRAP); }
#endif

#include <GL/glew.h>

#include "graphics/api/Exceptions.hpp"
#include "graphics/api/opengl/OpenGLUtils.hpp"

namespace graphics::api::opengl::detail
{
void CheckGLError()
{
#ifndef NDEBUG
	const auto error = glGetError();

	if (error != 0)
	{
		DEBUG_BREAK();

		throw OpenGLErrorException("glGetError indicated an error: " + std::to_string(error));
	}
#endif
}
}
