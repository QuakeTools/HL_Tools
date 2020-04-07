#pragma once

#include <cstdint>

namespace graphics::api
{
namespace BufferUsage
{
enum BufferUsage : std::uint8_t
{
	Vertex = 1 << 0,
	Index = 1 << 1,
	Uniform = 1 << 2,
	Dynamic = 1 << 6
};
}

const std::uint8_t BufferTypes = BufferUsage::Vertex | BufferUsage::Index | BufferUsage::Uniform;

enum class SamplerFilter : std::uint8_t
{
	MinPoint_MagPoint_MipPoint,
	MinPoint_MagPoint_MipLinear,
	MinPoint_MagLinear_MipPoint,
	MinPoint_MagLinear_MipLinear,
	MinLinear_MagPoint_MipPoint,
	MinLinear_MagPoint_MipLinear,
	MinLinear_MagLinear_MipPoint,
	MinLinear_MagLinear_MipLinear,
};

enum class IndexFormat : std::uint8_t
{
	UInt16 = 0,
	UInt32,
};

enum class BlendFactor : std::uint8_t
{
	Zero,
	One,
	SourceAlpha,
	InverseSourceAlpha,
	DestinationAlpha,
	InverseDestinationAlpha,
	SourceColor,
	InverseSourceColor,
	DestinationColor,
	InverseDestinationColor,
	BlendFactor,
	InverseBlendFactor,
};

enum class BlendFunction : std::uint8_t
{
	Add,
	Subtract,
	ReverseSubtract,
	Minimum,
	Maximum,
};

enum class ComparisonKind : std::uint8_t
{
	Never,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always,
};

enum class StencilOperation : std::uint8_t
{
	Keep,
	Zero,
	Replace,
	IncrementAndClamp,
	DecrementAndClamp,
	Invert,
	IncrementAndWrap,
	DecrementAndWrap,
};

enum class FaceCullMode : std::uint8_t
{
	Back,
	Front,
	None,
};

enum class PolygonFillMode : std::uint8_t
{
	Solid,
	Wireframe,
};

enum class FrontFace : std::uint8_t
{
	Clockwise,
	CounterClockwise,
};

enum class PrimitiveTopology : std::uint8_t
{
	TriangleList,
	TriangleStrip,
	LineList,
	LineStrip,
	PointList,
};

enum class VertexElementSemantic : std::uint8_t
{
	Position = 0,
	Normal,
	TextureCoordinate,
	Color,
	BoneIndex
};

enum class VertexElementFormat : std::uint8_t
{
	Float1 = 0,
	Float2,
	Float3,
	Float4,

	UInt1,
	UInt2,
	UInt3,
	UInt4,

	Int1,
	Int2,
	Int3,
	Int4,
};

enum class ResourceKind : std::uint8_t
{
	UniformBuffer = 0,
	TextureReadOnly,
};

namespace ShaderStages
{
enum ShaderStages : std::uint8_t
{
	None = 0,
	Vertex = 1 << 0,
	Geometry = 1 << 1,
	Fragment = 1 << 2,
};
}

enum class TextureType : std::uint8_t
{
	Texture1D,
	Texture2D,
	Texture3D,
	Cubemap,
};

enum class PixelFormat : std::uint8_t
{
	R8_G8_B8_A8_UNorm = 0,
};
}
