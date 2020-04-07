#pragma once

#include <cstdint>

namespace graphics
{
using RenderPasses = std::uint32_t;

namespace RenderPass
{
enum RenderPass : RenderPasses
{
	Underlay = 1 << 0,
	Standard = 1 << 1,
	AlphaBlend = 1 << 2,
	Overlay3D = 1 << 3,
	Overlay2D = 1 << 4,

	All = static_cast<RenderPasses>(~0)
};
}

struct RenderOrderKey
{
	RenderOrderKey() = default;

	RenderOrderKey(const unsigned long long value)
		: Value(value)
	{
	}

	RenderOrderKey(const RenderOrderKey&) = default;
	RenderOrderKey& operator=(const RenderOrderKey&) = default;

	unsigned long long Value = 0;
};
}
