#include <cassert>
#include <memory>
#include <stdexcept>
#include <tuple>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/api/FrameBuffer.hpp"
#include "graphics/api/GraphicsAPIHelpers.hpp"
#include "graphics/api/ResourceLayout.hpp"
#include "graphics/api/ResourceSet.hpp"

#include "graphics/api/opengl/OpenGLDeviceBuffer.hpp"
#include "graphics/api/opengl/OpenGLEnumConverters.hpp"
#include "graphics/api/opengl/OpenGLGraphicsDevice.hpp"
#include "graphics/api/opengl/OpenGLPipeline.hpp"
#include "graphics/api/opengl/OpenGLUtils.hpp"
#include "graphics/api/opengl/OpenGLShader.hpp"
#include "graphics/api/opengl/OpenGLSwapChainFrameBuffer.hpp"
#include "graphics/api/opengl/OpenGLTexture.hpp"

using namespace graphics::api::opengl::detail;

namespace graphics::api::opengl
{
static GLenum GetCubeTarget(unsigned int arrayLayer)
{
	switch (arrayLayer)
	{
	case 0:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	case 1:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	case 2:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	case 3:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	case 4:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	case 5:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	default:
		throw GraphicsAPIException("Unexpected array layer in UpdateTexture called on a cubemap texture.");
	}
}

OpenGLGraphicsDevice::OpenGLGraphicsDevice(const unsigned int width, const unsigned int height, const GraphicsDeviceOptions& options)
	: _options(options)
{
	const auto result = glewInit();

	if (result != GLEW_OK)
	{
		const auto error = glewGetErrorString(result);

		throw GlewInitException(reinterpret_cast<const char*>(error));
	}

	glGenVertexArrays(1, &_vao);
	CheckGLError();

	glBindVertexArray(_vao);
	CheckGLError();

	_swapChainFrameBuffer = std::make_unique<OpenGLSwapChainFrameBuffer>(width, height);
}

OpenGLGraphicsDevice::~OpenGLGraphicsDevice() = default;

const FrameBuffer& OpenGLGraphicsDevice::GetSwapChainFrameBuffer() const
{
	return *_swapChainFrameBuffer;
}

void OpenGLGraphicsDevice::ResizeMainWindow(const unsigned int width, const unsigned int height)
{
	_swapChainFrameBuffer->Resize(width, height);
}

void OpenGLGraphicsDevice::SetViewport(const Viewport& viewport)
{
	_viewport = viewport;

	glViewport((int) viewport.X, (int) viewport.Y, (unsigned int) viewport.Width, (unsigned int) viewport.Height);
	CheckGLError();

	glDepthRange(viewport.MinDepth, viewport.MaxDepth);
	CheckGLError();
}

void OpenGLGraphicsDevice::SetFullViewport()
{
	SetViewport({0.f, 0.f, static_cast<float>(_swapChainFrameBuffer->GetWidth()), static_cast<float>(_swapChainFrameBuffer->GetHeight()), 0.f, 1.f});
}

void OpenGLGraphicsDevice::SetScissorRect(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
{
	glScissor(static_cast<int>(x), static_cast<int>(_viewport.Height - static_cast<int>(height) - y), width, height);
}

void OpenGLGraphicsDevice::SetFullScissorRect()
{
	SetScissorRect(0, 0, _swapChainFrameBuffer->GetWidth(), _swapChainFrameBuffer->GetHeight());
}

void OpenGLGraphicsDevice::ClearColorTarget(const glm::vec4& clearColor)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	CheckGLError();

	glClear(GL_COLOR_BUFFER_BIT);
	CheckGLError();
}

void OpenGLGraphicsDevice::ClearDepthStencil(const float depth, const std::uint8_t stencil)
{
	glClearDepth(depth);
	CheckGLError();

	glClearStencil(stencil);
	CheckGLError();

	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	CheckGLError();
}

void OpenGLGraphicsDevice::UpdateBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int bufferOffsetInBytes, const void* pData, const unsigned int sizeInBytes)
{
	auto glBuffer = AssertSubtype<OpenGLDeviceBuffer>(buffer);

	if (glBuffer->GetId() == 0)
	{
		throw std::invalid_argument("Null buffer in UpdateBuffer");
	}

	//Allow null pointers if size is 0 (std::vector won't allocate unless it has data)
	if (sizeInBytes == 0)
	{
		return;
	}

	if (!pData)
	{
		throw std::invalid_argument("Null data pointer in UpdateBuffer");
	}

	const auto target = BufferUsageToTarget(glBuffer->GetUsage());

	glBindBuffer(target, glBuffer->GetId());
	CheckGLError();

	glBufferSubData(target, bufferOffsetInBytes, sizeInBytes, pData);
	CheckGLError();
}

void OpenGLGraphicsDevice::UpdateTexture(const std::shared_ptr<Texture>& texture,
	const void* pData, const unsigned int sizeInBytes,
	const unsigned int x, const unsigned int y, const unsigned int z,
	const unsigned int width, const unsigned int height, const unsigned int depth,
	const int mipLevel, const unsigned int arrayLayer)
{
	auto glTexture = AssertSubtype<OpenGLTexture>(texture);

	if (glTexture->GetId() == 0)
	{
		throw std::invalid_argument("Null texture in UpdateTexture");
	}

	if (!pData)
	{
		throw std::invalid_argument("Null data pointer in UpdateTexture");
	}

	const auto type = glTexture->GetType();

	glBindTexture(type, glTexture->GetId());

	switch (type)
	{
	case GL_TEXTURE_1D:
		glTexSubImage1D(GL_TEXTURE_1D, mipLevel, x, width, glTexture->GetFormat(), glTexture->GetPixelType(), pData);
		break;

	case GL_TEXTURE_2D:
		glTexSubImage2D(GL_TEXTURE_2D, mipLevel, x, y, width, height, glTexture->GetFormat(), glTexture->GetPixelType(), pData);
		break;

	case GL_TEXTURE_3D:
		glTexSubImage3D(GL_TEXTURE_2D, mipLevel, x, y, z, width, height, depth, glTexture->GetFormat(), glTexture->GetPixelType(), pData);
		break;

	case GL_TEXTURE_CUBE_MAP:
	{
		const auto cubeTarget = GetCubeTarget(arrayLayer);

		glTexSubImage2D(
			cubeTarget,
			mipLevel,
			(int) x,
			(int) y,
			width,
			height,
			glTexture->GetFormat(),
			glTexture->GetPixelType(),
			pData);
		break;
	}

	default: throw std::invalid_argument("Unsupported texture type in UpdateTexture");
	}

	CheckGLError();
}

void OpenGLGraphicsDevice::SetTextureFilterMode(const std::shared_ptr<Texture>& texture, const SamplerFilter filter, const bool mipmap)
{
	auto glTexture = AssertSubtype<OpenGLTexture>(texture);

	if (glTexture->GetId() == 0)
	{
		throw std::invalid_argument("Null texture in SetTextureFilterMode");
	}

	glBindTexture(glTexture->GetType(), glTexture->GetId());

	const auto [min, mag] = SamplerFilterToTextureMinMagFilter(filter, mipmap);

	glTexParameteri(glTexture->GetType(), GL_TEXTURE_MIN_FILTER, min);
	CheckGLError();
	glTexParameteri(glTexture->GetType(), GL_TEXTURE_MAG_FILTER, mag);
	CheckGLError();
}

void OpenGLGraphicsDevice::SetVertexBuffer(const unsigned int index, const std::shared_ptr<DeviceBuffer>& buffer, const unsigned int offset)
{
	if (index >= _vertexBuffers.size())
	{
		throw std::invalid_argument("Invalid index for vertex buffer");
	}

	auto glBuffer = AssertSubtype<OpenGLDeviceBuffer>(buffer);

	_vertexBuffers[index] = glBuffer;
	_vertexOffsets[index] = offset;
}

void OpenGLGraphicsDevice::SetIndexBuffer(const std::shared_ptr<DeviceBuffer>& buffer, const IndexFormat format)
{
	auto& glBuffer = AssertSubtype<OpenGLDeviceBuffer>(*buffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer.GetId());
	CheckGLError();

	_indexFormat = format;
}

void OpenGLGraphicsDevice::DrawIndexed(unsigned int indexCount, unsigned int indexStart)
{
	FlushResourceSets();
	FlushVertexLayouts();

	const auto indexSize = _indexFormat == IndexFormat::UInt16 ? 2u : 4u;

	glDrawElements(_primitiveType, indexCount, IndexFormatToType(_indexFormat), reinterpret_cast<void*>(indexStart * indexSize));
	CheckGLError();
}

void OpenGLGraphicsDevice::SetPipeline(const std::shared_ptr<Pipeline>& pipeline)
{
	auto glPipeline = AssertSubtype<OpenGLPipeline>(pipeline);

	if (_pipeline != glPipeline)
	{
		_pipeline = glPipeline;

		ActivateGraphicsPipeline();
	}
}

void OpenGLGraphicsDevice::SetResourceSet(unsigned int index, const std::shared_ptr<ResourceSet>& set)
{
	_resourceSets[index] = set;
}

void OpenGLGraphicsDevice::ActivateGraphicsPipeline()
{
	_resourceSets.resize(_pipeline->GetResourceLayouts().size());

	// Blend State

	const auto& blendState = _pipeline->GetBlendState();
	glBlendColor(blendState.BlendFactor.r, blendState.BlendFactor.g, blendState.BlendFactor.b, blendState.BlendFactor.a);
	CheckGLError();

	const auto& attachment = blendState.AttachmentState;

	if (!attachment.BlendEnabled)
	{
		glDisable(GL_BLEND);
		CheckGLError();
	}
	else
	{
		glEnable(GL_BLEND);
		CheckGLError();

		glBlendFuncSeparate(
			VdToGLBlendFactorSrc(attachment.SourceColorFactor),
			VdToGLBlendFactorDest(attachment.DestinationColorFactor),
			VdToGLBlendFactorSrc(attachment.SourceAlphaFactor),
			VdToGLBlendFactorDest(attachment.DestinationAlphaFactor));
		CheckGLError();

		glBlendEquationSeparate(
			VdToGLBlendEquationMode(attachment.ColorFunction),
			VdToGLBlendEquationMode(attachment.AlphaFunction));
		CheckGLError();
	}

	// Depth Stencil State

	const auto& dss = _pipeline->GetDepthStencilState();

	if (!dss.DepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
		CheckGLError();
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
		CheckGLError();

		glDepthFunc(VdToGLDepthFunction(dss.DepthComparison));
		CheckGLError();
	}

	glDepthMask(dss.DepthWriteEnabled);
	CheckGLError();

	if (dss.StencilTestEnabled)
	{
		glEnable(GL_STENCIL_TEST);
		CheckGLError();

		glStencilFuncSeparate(
			GL_FRONT,
			VdToGLStencilFunction(dss.StencilFront.Comparison),
			(int) dss.StencilReference,
			dss.StencilReadMask);
		CheckGLError();

		glStencilFuncSeparate(
			GL_BACK,
			VdToGLStencilFunction(dss.StencilBack.Comparison),
			(int) dss.StencilReference,
			dss.StencilReadMask);
		CheckGLError();

		glStencilMask(dss.StencilWriteMask);
		CheckGLError();
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
		CheckGLError();
	}

	// Rasterizer State

	const auto& rs = _pipeline->GetRasteriserState();

	if (rs.CullMode == FaceCullMode::None)
	{
		glDisable(GL_CULL_FACE);
		CheckGLError();
	}
	else
	{
		glEnable(GL_CULL_FACE);
		CheckGLError();

		glCullFace(VdToGLCullFaceMode(rs.CullMode));
		CheckGLError();
	}

	//if(_backend == GraphicsBackend.OpenGL)
	{
		glPolygonMode(GL_FRONT_AND_BACK, VdToGLPolygonMode(rs.FillMode));
		CheckGLError();
	}

	if (!rs.ScissorTestEnabled)
	{
		glDisable(GL_SCISSOR_TEST);
		CheckGLError();
	}
	else
	{
		glEnable(GL_SCISSOR_TEST);
		CheckGLError();
	}

	//if(_backend == GraphicsBackend.OpenGL)
	{
		if (!rs.DepthClipEnabled)
		{
			glEnable(GL_DEPTH_CLAMP);
			CheckGLError();
		}
		else
		{
			glDisable(GL_DEPTH_CLAMP);
			CheckGLError();
		}
	}

	glFrontFace(VdToGLFrontFaceDirection(rs.FrontFace));
	CheckGLError();

	// Primitive Topology
	_primitiveType = VdToGLPrimitiveType(_pipeline->GetPrimitiveTopology());

	// Shader Set
	glUseProgram(_pipeline->GetProgram());
	CheckGLError();

	_vertexBuffers.resize(_pipeline->GetVertexLayouts().size());
	_vertexOffsets.resize(_pipeline->GetVertexLayouts().size());
}

void OpenGLGraphicsDevice::FlushResourceSets()
{
	GLuint ubBaseIndex = 0;

	for (unsigned int slot = 0; slot < _pipeline->GetBoundResourceLayouts().size(); ++slot)
	{
		const auto& layout = _pipeline->GetResourceLayouts()[slot];

		for (unsigned int element = 0; element < layout->GetElements().size(); ++element)
		{
			const auto& description = layout->GetElements()[element];

			switch (description.Kind)
			{
			case ResourceKind::UniformBuffer:
			{
				auto& uniformBuffer = AssertSubtype<OpenGLDeviceBuffer>(*_resourceSets[slot]->Resources[element]);

				const UniformBufferBinding* binding = nullptr;

				if (_pipeline->GetUniformBinding(slot, element, binding))
				{
					if (binding->Location != GL_INVALID_INDEX)
					{
						glUniformBlockBinding(_pipeline->GetProgram(), binding->Location, ubBaseIndex);
						CheckGLError();

						glBindBufferRange(
							GL_UNIFORM_BUFFER,
							ubBaseIndex,
							uniformBuffer.GetId(),
							0,
							uniformBuffer.GetSizeInBytes());
						CheckGLError();

						++ubBaseIndex;
					}
				}
				break;
			}

			case ResourceKind::TextureReadOnly:
			{
				auto& texture = AssertSubtype<OpenGLTexture>(*_resourceSets[slot]->Resources[element]);

				const TextureBinding* binding = nullptr;

				if (_pipeline->GetTextureBinding(slot, element, binding))
				{
					glActiveTexture(GL_TEXTURE0 + binding->TextureUnit);
					CheckGLError();
					glBindTexture(texture.GetType(), texture.GetId());
					CheckGLError();

					glUniform1i(binding->Location, binding->TextureUnit);
					CheckGLError();
				}
				break;
			}
			}
		}
	}
}

void OpenGLGraphicsDevice::FlushVertexLayouts()
{
	GLuint index = 0;

	for (size_t i = 0; i < _pipeline->GetVertexLayouts().size(); ++i)
	{
		const auto& layout = _pipeline->GetVertexLayouts()[i];

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffers[i]->GetId());
		CheckGLError();

		unsigned int offset = 0;

		for (const auto& element : layout.Elements)
		{
			if (element.Location != -1)
			{
				//This must always be true since we don't provide manual bindings
				//assert(element.location == index);

				//if(index >= _activeVertexAttributes)
				{
					glEnableVertexAttribArray(element.Location);
					CheckGLError();
				}

				auto actualOffset = element.Description.Offset != 0 ? element.Description.Offset : offset;
				actualOffset += _vertexOffsets[i];

				bool isInteger;
				const auto type = VertexElementFormatToAttrib(element.Description.Format, isInteger);

				if (isInteger)
				{
					glVertexAttribIPointer(
						element.Location,
						VertexElementFormatToSize(element.Description.Format),
						type,
						layout.Stride,
						reinterpret_cast<void*>(actualOffset)
					);
				}
				else
				{
					glVertexAttribPointer(
						element.Location,
						VertexElementFormatToSize(element.Description.Format),
						type,
						GL_FALSE,
						layout.Stride,
						reinterpret_cast<void*>(actualOffset)
					);
				}
				CheckGLError();
			}
			else
			{
				glDisableVertexAttribArray(index);
				CheckGLError();
			}

			offset += VertexElementFormatToSizeInBytes(element.Description.Format);

			++index;
		}
	}

	const auto currentlyActiveAttributes = index;

	if (index < _activeVertexAttributes)
	{
		for (; index < _activeVertexAttributes; ++index)
		{
			glDisableVertexAttribArray(index);
			CheckGLError();
		}
	}

	_activeVertexAttributes = currentlyActiveAttributes;
}
}
