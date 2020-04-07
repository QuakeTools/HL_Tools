#include <string>

#include "graphics/api/Exceptions.hpp"
#include "graphics/api/opengl/OpenGLEnumConverters.hpp"
#include "graphics/api/opengl/OpenGLShader.hpp"

using namespace graphics::api::opengl::detail;

namespace graphics::api::opengl
{
OpenGLShader::OpenGLShader(const ShaderDescription& description)
{
	_shader = glCreateShader(ShaderStagesToType(description.Stage));

	auto data = description.SourceCode.data();

	glShaderSource(_shader, 1, &data, nullptr);

	glCompileShader(_shader);

	GLint result = GL_FALSE;
	int infoLogLength;

	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		std::string shaderErrorMessage(static_cast<size_t>(infoLogLength), '\0');
		glGetShaderInfoLog(_shader, infoLogLength, nullptr, shaderErrorMessage.data());

		//Clean up resources now
		glDeleteShader(_shader);

		throw ShaderCompileErrorException(shaderErrorMessage);
	}
}

OpenGLShader::~OpenGLShader()
{
	glDeleteShader(_shader);
}
}
