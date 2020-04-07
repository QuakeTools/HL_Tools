#include <stdexcept>
#include <string>

#include "graphics/api/Exceptions.hpp"
#include "graphics/api/GraphicsAPIHelpers.hpp"
#include "graphics/api/GraphicsDevice.hpp"
#include "graphics/api/Pipeline.hpp"
#include "graphics/api/ResourceLayout.hpp"
#include "graphics/api/Shader.hpp"

namespace graphics::api
{
constexpr BlendAttachmentDescription BlendAttachmentDescription::OverrideBlend =
{
	true,
	BlendFactor::One,
	BlendFactor::Zero,
	BlendFunction::Add,
	BlendFactor::One,
	BlendFactor::Zero,
	BlendFunction::Add
};

constexpr BlendAttachmentDescription BlendAttachmentDescription::AlphaBlend =
{
	true,
	BlendFactor::SourceAlpha,
	BlendFactor::InverseSourceAlpha,
	BlendFunction::Add,
	BlendFactor::SourceAlpha,
	BlendFactor::InverseSourceAlpha,
	BlendFunction::Add
};

constexpr BlendAttachmentDescription BlendAttachmentDescription::AdditiveBlend =
{
	true,
	BlendFactor::SourceAlpha,
	BlendFactor::One,
	BlendFunction::Add,
	BlendFactor::SourceAlpha,
	BlendFactor::One,
	BlendFunction::Add
};

constexpr BlendAttachmentDescription BlendAttachmentDescription::Disabled =
{
	false,
	BlendFactor::One,
	BlendFactor::Zero,
	BlendFunction::Add,
	BlendFactor::One,
	BlendFactor::Zero,
	BlendFunction::Add
};

constexpr BlendAttachmentDescription BlendAttachmentDescription::AdditiveOneOneBlend
{
	true,
	BlendFactor::One,
	BlendFactor::One,
	BlendFunction::Add,
	BlendFactor::One,
	BlendFactor::One,
	BlendFunction::Add,
};

constexpr BlendStateDescription BlendStateDescription::SingleOverrideBlend =
{
	BlendAttachmentDescription::OverrideBlend
};

constexpr BlendStateDescription BlendStateDescription::SingleAlphaBlend =
{
	BlendAttachmentDescription::AlphaBlend
};

constexpr BlendStateDescription BlendStateDescription::SingleAdditiveBlend =
{
	BlendAttachmentDescription::AdditiveBlend
};

constexpr BlendStateDescription BlendStateDescription::SingleDisabled =
{
	BlendAttachmentDescription::Disabled
};

const BlendStateDescription BlendStateDescription::SingleAdditiveOneOneBlend
{
	BlendAttachmentDescription::AdditiveOneOneBlend
};

constexpr DepthStencilStateDescription DepthStencilStateDescription::DepthOnlyLessEqual =
{
	true,
	true,
	ComparisonKind::LessEqual
};

constexpr DepthStencilStateDescription DepthStencilStateDescription::DepthOnlyLessEqualRead =
{
	true,
	false,
	ComparisonKind::LessEqual
};

constexpr DepthStencilStateDescription DepthStencilStateDescription::DepthOnlyGreaterEqual =
{
	true,
	true,
	ComparisonKind::GreaterEqual
};

constexpr DepthStencilStateDescription DepthStencilStateDescription::DepthOnlyGreaterEqualRead =
{
	true,
	false,
	ComparisonKind::GreaterEqual
};

constexpr DepthStencilStateDescription DepthStencilStateDescription::Disabled =
{
	false,
	false,
	ComparisonKind::LessEqual
};

constexpr RasterizerStateDescription RasterizerStateDescription::Default =
{
	FaceCullMode::Back,
	PolygonFillMode::Solid,
	FrontFace::Clockwise,
	true,
	false,
};

constexpr RasterizerStateDescription RasterizerStateDescription::CullNone =
{
	FaceCullMode::None,
	PolygonFillMode::Solid,
	FrontFace::Clockwise,
	true,
	false,
};

unsigned int VertexLayoutDescription::ComputeStride(const std::vector<VertexElementDescription>& elements)
{
	unsigned int computedStride = 0;

	for (const auto& element : elements)
	{
		const auto elementSize = VertexElementFormatToSizeInBytes(element.Format);

		if (element.Offset != 0)
		{
			computedStride = element.Offset + elementSize;
		}
		else
		{
			computedStride += elementSize;
		}
	}

	return computedStride;
}
}
