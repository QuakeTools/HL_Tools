#pragma once

#include <stdexcept>
#include <string>

#include <GL/glew.h>

#include "graphics/api/Enums.hpp"
#include "graphics/api/Shader.hpp"

namespace graphics::api::opengl
{
class OpenGLShader final : public Shader
{
public:
	OpenGLShader(const ShaderDescription& description);

	OpenGLShader() = delete;

	OpenGLShader(Shader&&) = delete;
	OpenGLShader& operator=(Shader&&) = delete;

	OpenGLShader(const Shader&) = delete;
	OpenGLShader& operator=(const Shader&) = delete;

	~OpenGLShader();

	GLuint GetId() const { return _shader; }

private:
	GLuint _shader;
};
}
