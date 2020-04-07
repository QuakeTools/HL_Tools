#pragma once

#include <stdexcept>

#include "graphics/api/Enums.hpp"

namespace graphics::api
{
inline std::size_t VertexElementFormatToSizeInBytes(const VertexElementFormat format)
{
	switch (format)
	{
	case VertexElementFormat::Float1:
	case VertexElementFormat::UInt1:
	case VertexElementFormat::Int1:
		return 4U;

	case VertexElementFormat::Float2:
	case VertexElementFormat::UInt2:
	case VertexElementFormat::Int2:
		return 8U;

	case VertexElementFormat::Float3:
	case VertexElementFormat::UInt3:
	case VertexElementFormat::Int3:
		return 12U;

	case VertexElementFormat::Float4:
	case VertexElementFormat::UInt4:
	case VertexElementFormat::Int4:
		return 16U;
	}

	throw std::invalid_argument("Unsupported vertex element format");
}

inline std::size_t VertexElementFormatToSize(const VertexElementFormat format)
{
	switch (format)
	{
	case VertexElementFormat::Float1:
	case VertexElementFormat::UInt1:
	case VertexElementFormat::Int1:
		return 1U;

	case VertexElementFormat::Float2:
	case VertexElementFormat::UInt2:
	case VertexElementFormat::Int2:
		return 2U;

	case VertexElementFormat::Float3:
	case VertexElementFormat::UInt3:
	case VertexElementFormat::Int3:
		return 3U;

	case VertexElementFormat::Float4:
	case VertexElementFormat::UInt4:
	case VertexElementFormat::Int4:
		return 4U;
	}

	throw std::invalid_argument("Unsupported vertex element format");
}
}
