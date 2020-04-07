#include <stdexcept>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "graphics/api/opengl/OpenGLEnumConverters.hpp"

namespace graphics::api::opengl::detail
{
GLenum BufferUsageToTarget(std::uint8_t usage)
{
	usage &= BufferTypes;

	switch (usage)
	{
	case BufferUsage::Vertex: return GL_ARRAY_BUFFER;
	case BufferUsage::Index: return GL_ELEMENT_ARRAY_BUFFER;
	case BufferUsage::Uniform: return GL_UNIFORM_BUFFER;
	default: throw std::invalid_argument("Invalid buffer usage");
	}
}

GLenum BufferUsageToUsage(std::uint8_t usage)
{
	return (usage & BufferUsage::Dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
}

GLenum ShaderStagesToType(ShaderStages::ShaderStages stage)
{
	switch (stage)
	{
	case ShaderStages::ShaderStages::Vertex: return GL_VERTEX_SHADER;
	case ShaderStages::ShaderStages::Geometry: return GL_GEOMETRY_SHADER;
	case ShaderStages::ShaderStages::Fragment: return GL_FRAGMENT_SHADER;
	}

	throw std::invalid_argument("Unknown shader stage");
}

GLenum TextureTypeToType(const TextureType type)
{
	switch (type)
	{
	case TextureType::Texture1D: return GL_TEXTURE_1D;
	case TextureType::Texture2D: return GL_TEXTURE_2D;
	case TextureType::Texture3D: return GL_TEXTURE_3D;
	case TextureType::Cubemap: return GL_TEXTURE_CUBE_MAP;
	}

	throw std::invalid_argument("Unsupported TextureType");
}

GLenum PixelFormatToFormat(const PixelFormat format)
{
	switch (format)
	{
	case PixelFormat::R8_G8_B8_A8_UNorm: return GL_RGBA;
	}

	throw std::invalid_argument("Unsupported PixelFormat");
}

GLenum PixelFormatToInternalFormat(const PixelFormat format)
{
	switch (format)
	{
	case PixelFormat::R8_G8_B8_A8_UNorm: return GL_RGBA8;
	}

	throw std::invalid_argument("Unsupported internal PixelFormat");
}

GLenum PixelFormatToPixelType(const PixelFormat format)
{
	switch (format)
	{
	case PixelFormat::R8_G8_B8_A8_UNorm: return GL_UNSIGNED_BYTE;
	}

	throw std::invalid_argument("Unsupported PixelType");
}

GLenum IndexFormatToType(const IndexFormat format)
{
	switch (format)
	{
	case IndexFormat::UInt16: return GL_UNSIGNED_SHORT;
	case IndexFormat::UInt32: return GL_UNSIGNED_INT;
	}

	throw std::invalid_argument("Unknown index format");
}

GLenum VertexElementFormatToAttrib(const VertexElementFormat format, bool& isInteger)
{
	switch (format)
	{
	case VertexElementFormat::Float1:
	case VertexElementFormat::Float2:
	case VertexElementFormat::Float3:
	case VertexElementFormat::Float4:
		isInteger = false;
		return GL_FLOAT;

	case VertexElementFormat::UInt1:
	case VertexElementFormat::UInt2:
	case VertexElementFormat::UInt3:
	case VertexElementFormat::UInt4:
		isInteger = true;
		return GL_UNSIGNED_INT;

	case VertexElementFormat::Int1:
	case VertexElementFormat::Int2:
	case VertexElementFormat::Int3:
	case VertexElementFormat::Int4:
		isInteger = true;
		return GL_INT;
	}

	throw std::invalid_argument("Unsupported vertex element format");
}

std::tuple<GLint, GLint> SamplerFilterToTextureMinMagFilter(const SamplerFilter filter, bool mip)
{
	GLint min, mag;

	switch (filter)
	{
	case SamplerFilter::MinPoint_MagPoint_MipPoint:
		min = mip ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
		mag = GL_NEAREST;
		break;
	case SamplerFilter::MinPoint_MagPoint_MipLinear:
		min = mip ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
		mag = GL_NEAREST;
		break;
	case SamplerFilter::MinPoint_MagLinear_MipPoint:
		min = mip ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
		mag = GL_LINEAR;
		break;
	case SamplerFilter::MinPoint_MagLinear_MipLinear:
		min = mip ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
		mag = GL_LINEAR;
		break;
	case SamplerFilter::MinLinear_MagPoint_MipPoint:
		min = mip ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
		mag = GL_NEAREST;
		break;
	case SamplerFilter::MinLinear_MagPoint_MipLinear:
		min = mip ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
		mag = GL_NEAREST;
		break;
	case SamplerFilter::MinLinear_MagLinear_MipPoint:
		min = mip ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
		mag = GL_LINEAR;
		break;
	case SamplerFilter::MinLinear_MagLinear_MipLinear:
		min = mip ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
		mag = GL_LINEAR;
		break;
	default:
		throw std::invalid_argument("Invalid SamplerFilter value");
	}

	return std::make_tuple(min, mag);
}

GLenum VdToGLBlendFactorSrc(const BlendFactor factor)
{
	switch (factor)
	{
	case BlendFactor::Zero:
		return GL_ZERO;
	case BlendFactor::One:
		return GL_ONE;
	case BlendFactor::SourceAlpha:
		return GL_SRC_ALPHA;
	case BlendFactor::InverseSourceAlpha:
		return GL_ONE_MINUS_SRC_ALPHA;
	case BlendFactor::DestinationAlpha:
		return GL_DST_ALPHA;
	case BlendFactor::InverseDestinationAlpha:
		return GL_ONE_MINUS_DST_ALPHA;
	case BlendFactor::SourceColor:
		return GL_SRC_COLOR;
	case BlendFactor::InverseSourceColor:
		return GL_ONE_MINUS_SRC_COLOR;
	case BlendFactor::DestinationColor:
		return GL_DST_COLOR;
	case BlendFactor::InverseDestinationColor:
		return GL_ONE_MINUS_DST_COLOR;
	case BlendFactor::BlendFactor:
		return GL_CONSTANT_COLOR;
	case BlendFactor::InverseBlendFactor:
		return GL_ONE_MINUS_CONSTANT_COLOR;
	default:
		throw std::invalid_argument("Invalid BlendFactor");
	}
}

GLenum VdToGLBlendFactorDest(const BlendFactor factor)
{
	switch (factor)
	{
	case BlendFactor::Zero:
		return GL_ZERO;
	case BlendFactor::One:
		return GL_ONE;
	case BlendFactor::SourceAlpha:
		return GL_SRC_ALPHA;
	case BlendFactor::InverseSourceAlpha:
		return GL_ONE_MINUS_SRC_ALPHA;
	case BlendFactor::DestinationAlpha:
		return GL_DST_ALPHA;
	case BlendFactor::InverseDestinationAlpha:
		return GL_ONE_MINUS_DST_ALPHA;
	case BlendFactor::SourceColor:
		return GL_SRC_COLOR;
	case BlendFactor::InverseSourceColor:
		return GL_ONE_MINUS_SRC_COLOR;
	case BlendFactor::DestinationColor:
		return GL_DST_COLOR;
	case BlendFactor::InverseDestinationColor:
		return GL_ONE_MINUS_DST_COLOR;
	case BlendFactor::BlendFactor:
		return GL_CONSTANT_COLOR;
	case BlendFactor::InverseBlendFactor:
		return GL_ONE_MINUS_CONSTANT_COLOR;
	default:
		throw std::invalid_argument("Invalid BlendFactor");
	}
}

GLenum VdToGLBlendEquationMode(const BlendFunction function)
{
	switch (function)
	{
	case BlendFunction::Add:
		return GL_FUNC_ADD;
	case BlendFunction::Subtract:
		return GL_FUNC_SUBTRACT;
	case BlendFunction::ReverseSubtract:
		return GL_FUNC_REVERSE_SUBTRACT;
	case BlendFunction::Minimum:
		return GL_MIN;
	case BlendFunction::Maximum:
		return GL_MAX;
	default:
		throw std::invalid_argument("Invalid BlendFunction");
	}
}

GLenum VdToGLDepthFunction(const ComparisonKind value)
{
	switch (value)
	{
	case ComparisonKind::Never:
		return GL_NEVER;
	case ComparisonKind::Less:
		return GL_LESS;
	case ComparisonKind::Equal:
		return GL_EQUAL;
	case ComparisonKind::LessEqual:
		return GL_LEQUAL;
	case ComparisonKind::Greater:
		return GL_GREATER;
	case ComparisonKind::NotEqual:
		return GL_NOTEQUAL;
	case ComparisonKind::GreaterEqual:
		return GL_GEQUAL;
	case ComparisonKind::Always:
		return GL_ALWAYS;
	default:
		throw std::invalid_argument("Invalid ComparisonKind");
	}
}

GLenum VdToGLStencilFunction(const ComparisonKind comparison)
{
	switch (comparison)
	{
	case ComparisonKind::Never:
		return GL_NEVER;
	case ComparisonKind::Less:
		return GL_LESS;
	case ComparisonKind::Equal:
		return GL_EQUAL;
	case ComparisonKind::LessEqual:
		return GL_LEQUAL;
	case ComparisonKind::Greater:
		return GL_GREATER;
	case ComparisonKind::NotEqual:
		return GL_NOTEQUAL;
	case ComparisonKind::GreaterEqual:
		return GL_GEQUAL;
	case ComparisonKind::Always:
		return GL_ALWAYS;
	default:
		throw std::invalid_argument("Invalid ComparisonKind");
	}
}

GLenum VdToGLCullFaceMode(const FaceCullMode cullMode)
{
	switch (cullMode)
	{
	case FaceCullMode::Back:
		return GL_BACK;
	case FaceCullMode::Front:
		return GL_FRONT;
	default:
		throw std::invalid_argument("Invalid FaceCullMode");
	}
}

GLenum VdToGLPolygonMode(const PolygonFillMode fillMode)
{
	switch (fillMode)
	{
	case PolygonFillMode::Solid:
		return GL_FILL;
	case PolygonFillMode::Wireframe:
		return GL_LINE;
	default:
		throw std::invalid_argument("Invalid PolygonFillMode");
	}
}

GLenum VdToGLFrontFaceDirection(const FrontFace frontFace)
{
	switch (frontFace)
	{
	case FrontFace::Clockwise:
		return GL_CW;
	case FrontFace::CounterClockwise:
		return GL_CCW;
	default:
		throw std::invalid_argument("Invalid FrontFace");
	}
}

GLenum VdToGLPrimitiveType(const PrimitiveTopology primitiveTopology)
{
	switch (primitiveTopology)
	{
	case PrimitiveTopology::TriangleList:
		return GL_TRIANGLES;
	case PrimitiveTopology::TriangleStrip:
		return GL_TRIANGLE_STRIP;
	case PrimitiveTopology::LineList:
		return GL_LINES;
	case PrimitiveTopology::LineStrip:
		return GL_LINE_STRIP;
	case PrimitiveTopology::PointList:
		return GL_POINTS;
	default:
		throw std::invalid_argument("Invalid PrimitiveTopology");
	}
}
}
