#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Enums.hpp"

namespace graphics::api
{
struct ResourceLayoutElementDescription
{
	ResourceLayoutElementDescription() = default;
	ResourceLayoutElementDescription(ResourceLayoutElementDescription&&) = default;
	ResourceLayoutElementDescription& operator=(ResourceLayoutElementDescription&&) = default;
	~ResourceLayoutElementDescription() = default;

	ResourceLayoutElementDescription(std::string&& name, const ResourceKind kind, const std::uint8_t stages)
		: Name(std::move(name))
		, Kind(kind)
		, Stages(stages)
	{
	}

	std::string Name;
	ResourceKind Kind = ResourceKind::UniformBuffer;
	std::uint8_t Stages = ShaderStages::None;
};

struct ResourceLayoutDescription
{
	ResourceLayoutDescription() = default;

	ResourceLayoutDescription(ResourceLayoutDescription&&) = default;
	ResourceLayoutDescription& operator=(ResourceLayoutDescription&&) = default;

	ResourceLayoutDescription(std::vector<ResourceLayoutElementDescription>&& elements)
		: Elements(std::move(elements))
	{
	}

	std::vector<ResourceLayoutElementDescription> Elements;
};

class ResourceLayout final
{
public:
	ResourceLayout(ResourceLayoutDescription&& description)
		: _elements(std::move(description.Elements))
	{
	}

	const std::vector<ResourceLayoutElementDescription>& GetElements() const { return _elements; }

private:
	const std::vector<ResourceLayoutElementDescription> _elements;
};
}
