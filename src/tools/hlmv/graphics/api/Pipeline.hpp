#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/vec4.hpp>

#include "Enums.hpp"

namespace graphics::api
{
class ResourceLayout;
class Shader;

struct BlendAttachmentDescription
{
	BlendAttachmentDescription() = default;

	constexpr BlendAttachmentDescription(const BlendAttachmentDescription&) = default;
	constexpr BlendAttachmentDescription& operator=(const BlendAttachmentDescription&) = default;

	constexpr BlendAttachmentDescription(
		bool blendEnabled,
		BlendFactor sourceColorFactor,
		BlendFactor destinationColorFactor,
		BlendFunction colorFunction,
		BlendFactor sourceAlphaFactor,
		BlendFactor destinationAlphaFactor,
		BlendFunction alphaFunction)
		: BlendEnabled(blendEnabled)
		, SourceColorFactor(sourceColorFactor)
		, DestinationColorFactor(destinationColorFactor)
		, ColorFunction(colorFunction)
		, SourceAlphaFactor(sourceAlphaFactor)
		, DestinationAlphaFactor(destinationAlphaFactor)
		, AlphaFunction(alphaFunction)
	{
	}

	bool BlendEnabled;

	BlendFactor SourceColorFactor;
	BlendFactor DestinationColorFactor;
	BlendFunction ColorFunction;

	BlendFactor SourceAlphaFactor;
	BlendFactor DestinationAlphaFactor;
	BlendFunction AlphaFunction;

	static const BlendAttachmentDescription OverrideBlend;

	static const BlendAttachmentDescription AlphaBlend;

	static const BlendAttachmentDescription AdditiveBlend;

	static const BlendAttachmentDescription Disabled;

	static const BlendAttachmentDescription AdditiveOneOneBlend;
};

struct BlendStateDescription
{
	BlendStateDescription() = default;

	constexpr BlendStateDescription(const BlendStateDescription&) = default;
	constexpr BlendStateDescription& operator=(const BlendStateDescription&) = default;

	constexpr BlendStateDescription(const glm::vec4& blendFactor, const BlendAttachmentDescription& attachmentState)
		: BlendFactor(blendFactor)
		, AttachmentState(attachmentState)
	{
	}

	constexpr BlendStateDescription(const BlendAttachmentDescription& attachmentState)
		: BlendStateDescription({}, attachmentState)
	{
	}

	glm::vec4 BlendFactor;
	BlendAttachmentDescription AttachmentState;

	static const BlendStateDescription SingleOverrideBlend;

	static const BlendStateDescription SingleAlphaBlend;

	static const BlendStateDescription SingleAdditiveBlend;

	static const BlendStateDescription SingleDisabled;

	static const BlendStateDescription SingleAdditiveOneOneBlend;
};

struct StencilBehaviorDescription
{
	StencilOperation Fail = StencilOperation::Keep;

	StencilOperation Pass = StencilOperation::Keep;

	StencilOperation DepthFail = StencilOperation::Keep;

	ComparisonKind Comparison = ComparisonKind::Never;

	constexpr StencilBehaviorDescription() = default;

	constexpr StencilBehaviorDescription(
		const StencilOperation fail,
		const StencilOperation pass,
		const StencilOperation depthFail,
		const ComparisonKind comparison)
		: Fail(fail)
		, Pass(pass)
		, DepthFail(depthFail)
		, Comparison(comparison)
	{
	}

	constexpr StencilBehaviorDescription(const StencilBehaviorDescription&) = default;
	constexpr StencilBehaviorDescription& operator=(const StencilBehaviorDescription&) = default;
};

struct DepthStencilStateDescription
{
	bool DepthTestEnabled = false;

	bool DepthWriteEnabled = false;

	ComparisonKind DepthComparison = ComparisonKind::Never;

	bool StencilTestEnabled = false;

	StencilBehaviorDescription StencilFront;

	StencilBehaviorDescription StencilBack;

	std::uint8_t StencilReadMask = 0;

	std::uint8_t StencilWriteMask = 0;

	unsigned int StencilReference = 0;

	DepthStencilStateDescription() = default;

	constexpr DepthStencilStateDescription(const bool depthTestEnabled, const bool depthWriteEnabled, const ComparisonKind depthComparison)
		: DepthTestEnabled(depthTestEnabled)
		, DepthWriteEnabled(depthWriteEnabled)
		, DepthComparison(depthComparison)
	{
	}

	constexpr DepthStencilStateDescription(
		const bool depthTestEnabled,
		const bool depthWriteEnabled,
		const ComparisonKind depthComparison,
		const bool stencilTestEnabled,
		const StencilBehaviorDescription stencilFront,
		const StencilBehaviorDescription stencilBack,
		const std::uint8_t stencilReadMask,
		const std::uint8_t stencilWriteMask,
		const unsigned int stencilReference)
		: DepthTestEnabled(depthTestEnabled)
		, DepthWriteEnabled(depthWriteEnabled)
		, DepthComparison(depthComparison)
		, StencilTestEnabled(stencilTestEnabled)
		, StencilFront(stencilFront)
		, StencilBack(stencilBack)
		, StencilReadMask(stencilReadMask)
		, StencilWriteMask(stencilWriteMask)
		, StencilReference(stencilReference)
	{
	}

	constexpr DepthStencilStateDescription(const DepthStencilStateDescription&) = default;
	constexpr DepthStencilStateDescription& operator=(const DepthStencilStateDescription&) = default;

	static const DepthStencilStateDescription DepthOnlyLessEqual;

	static const DepthStencilStateDescription DepthOnlyLessEqualRead;

	static const DepthStencilStateDescription DepthOnlyGreaterEqual;

	static const DepthStencilStateDescription DepthOnlyGreaterEqualRead;

	static const DepthStencilStateDescription Disabled;
};

struct RasterizerStateDescription
{
	const FaceCullMode CullMode;

	const PolygonFillMode FillMode;

	const FrontFace FrontFace;

	const bool DepthClipEnabled;

	const bool ScissorTestEnabled;

	constexpr RasterizerStateDescription(
		const FaceCullMode cullMode,
		const PolygonFillMode fillMode,
		const graphics::api::FrontFace frontFace,
		const bool depthClipEnabled,
		const bool scissorTestEnabled)
		: CullMode(cullMode)
		, FillMode(fillMode)
		, FrontFace(frontFace)
		, DepthClipEnabled(depthClipEnabled)
		, ScissorTestEnabled(scissorTestEnabled)
	{
	}

	static const RasterizerStateDescription Default;

	static const RasterizerStateDescription CullNone;
};

struct VertexElementDescription
{
	VertexElementDescription() = default;
	VertexElementDescription(VertexElementDescription&&) = default;
	VertexElementDescription& operator=(VertexElementDescription&&) = default;

	VertexElementDescription(const VertexElementDescription&) = default;
	VertexElementDescription& operator=(const VertexElementDescription&) = default;

	VertexElementDescription(std::string&& name, const VertexElementSemantic semantic, const VertexElementFormat format, const unsigned int offset = 0)
		: Name(std::move(name))
		, Semantic(semantic)
		, Format(format)
		, Offset(offset)
	{

	}

	std::string Name;
	VertexElementSemantic Semantic;
	VertexElementFormat Format;
	unsigned int Offset = 0;
};

struct VertexLayoutDescription
{
	VertexLayoutDescription(VertexLayoutDescription&&) = default;
	VertexLayoutDescription& operator=(VertexLayoutDescription&&) = default;

	VertexLayoutDescription(const VertexLayoutDescription&) = default;
	VertexLayoutDescription& operator=(const VertexLayoutDescription&) = default;

	VertexLayoutDescription(std::vector<VertexElementDescription>&& elements)
		: Elements(std::move(elements))
		, Stride(ComputeStride(this->Elements))
	{
	}

	VertexLayoutDescription(std::vector<VertexElementDescription>&& elements, unsigned int stride)
		: Elements(std::move(elements))
		, Stride(stride)
	{
	}

	std::vector<VertexElementDescription> Elements;
	unsigned int Stride = 0;

private:
	static unsigned int ComputeStride(const std::vector<VertexElementDescription>& elements);
};

struct ShaderSetDescription
{
	ShaderSetDescription() = delete;

	ShaderSetDescription(ShaderSetDescription&&) = default;
	ShaderSetDescription& operator=(ShaderSetDescription&&) = default;

	std::vector<VertexLayoutDescription> VertexLayouts;
	std::vector<std::shared_ptr<Shader>> Shaders;
};

struct PipelineDescription
{
	BlendStateDescription BlendState;
	DepthStencilStateDescription DepthStencilState;
	RasterizerStateDescription RasterizerState;
	PrimitiveTopology PrimitiveTopology = {};
	ShaderSetDescription ShaderSet;
	std::vector<std::shared_ptr<ResourceLayout>> ResourceLayouts;
};

class Pipeline
{
protected:
	Pipeline() = default;

public:
	Pipeline(Pipeline&&) = delete;
	Pipeline& operator=(Pipeline&&) = delete;

	Pipeline(const Pipeline&) = delete;
	Pipeline& operator=(const Pipeline&) = delete;

	virtual ~Pipeline() = 0 {}
};
}
