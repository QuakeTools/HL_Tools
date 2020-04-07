#include <stdexcept>
#include <string>

#include "graphics/api/Exceptions.hpp"
#include "graphics/api/Pipeline.hpp"
#include "graphics/api/ResourceLayout.hpp"

#include "graphics/api/opengl/OpenGLEnumConverters.hpp"
#include "graphics/api/opengl/OpenGLPipeline.hpp"
#include "graphics/api/opengl/OpenGLShader.hpp"
#include "graphics/api/opengl/OpenGLUtils.hpp"

using namespace graphics::api::opengl::detail;

namespace graphics::api::opengl
{
static std::vector<BoundVertexLayout> CreateBoundVertexLayouts(std::vector<VertexLayoutDescription>& vertexLayouts, const GLuint program)
{
	std::vector<BoundVertexLayout> result;

	result.reserve(vertexLayouts.size());

	for (auto& layout : vertexLayouts)
	{
		BoundVertexLayout bound;

		bound.Elements.reserve(layout.Elements.size());
		bound.Stride = layout.Stride;

		for (auto& element : layout.Elements)
		{
			bound.Elements.emplace_back(std::move(element), glGetAttribLocation(program, element.Name.c_str()));
		}

		result.emplace_back(std::move(bound));
	}

	return result;
}

OpenGLPipeline::OpenGLPipeline(PipelineDescription&& description)
	: _blendState(std::move(description.BlendState))
	, _depthStencilState(std::move(description.DepthStencilState))
	, _rasteriserState(std::move(description.RasterizerState))
	, _primitiveTopology(description.PrimitiveTopology)
	, _resourceLayouts(std::move(description.ResourceLayouts))
{
	_program = glCreateProgram();

	for (const auto& shader : description.ShaderSet.Shaders)
	{
		auto glShader = AssertSubtype<OpenGLShader>(shader);

		glAttachShader(_program, glShader->GetId());
	}

	glLinkProgram(_program);

	for (const auto& shader : description.ShaderSet.Shaders)
	{
		auto glShader = AssertSubtype<OpenGLShader>(shader);

		glDetachShader(_program, glShader->GetId());
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	glGetProgramiv(_program, GL_LINK_STATUS, &result);
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		std::string programErrorMessage(static_cast<size_t>(infoLogLength), '\0');
		glGetProgramInfoLog(_program, infoLogLength, nullptr, programErrorMessage.data());

		//Clean up resources now
		glDeleteShader(_program);

		throw ProgramLinkErrorException(programErrorMessage);
	}

	_vertexLayouts = CreateBoundVertexLayouts(description.ShaderSet.VertexLayouts, _program);

	_boundResourceLayouts.reserve(_resourceLayouts.size());

	//Texture unit relative to GL_TEXTURE0
	GLenum textureUnit = 0;

	for (const auto& resourceLayout : _resourceLayouts)
	{
		std::unordered_map<unsigned int, std::unique_ptr<UniformBufferBinding>> uniforms;
		std::unordered_map<unsigned int, std::unique_ptr<TextureBinding>> textures;

		for (unsigned int i = 0; i < resourceLayout->GetElements().size(); ++i)
		{
			const auto& element = resourceLayout->GetElements()[i];

			switch (element.Kind)
			{
			case ResourceKind::UniformBuffer:
			{
				const auto location = glGetUniformBlockIndex(_program, element.Name.c_str());

				uniforms.emplace(i, std::make_unique<UniformBufferBinding>(location));
				break;
			}

			case ResourceKind::TextureReadOnly:
			{
				const auto location = glGetUniformLocation(_program, element.Name.c_str());

				CheckGLError();

				if (location != -1)
				{
					textures.emplace(i, std::make_unique<TextureBinding>(textureUnit, location));
				}

				++textureUnit;
				break;
			}

			default: throw std::invalid_argument("Unsupported ResourceKind");
			}
		}

		_boundResourceLayouts.emplace_back(std::move(uniforms), std::move(textures));
	}
}

OpenGLPipeline::~OpenGLPipeline()
{
	glDeleteProgram(_program);
}
}
