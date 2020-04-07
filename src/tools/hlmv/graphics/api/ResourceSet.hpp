#pragma once

#include <memory>
#include <vector>

namespace graphics::api
{
class BindableResource;
class ResourceLayout;

struct ResourceSetDescription
{
	ResourceSetDescription() = delete;
	ResourceSetDescription(ResourceSetDescription&&) = default;
	~ResourceSetDescription() = default;

	ResourceSetDescription(const std::shared_ptr<ResourceLayout>& layout, std::vector<std::shared_ptr<BindableResource>>&& resources)
		: Layout(layout)
		, Resources(std::move(resources))
	{
	}

	std::shared_ptr<ResourceLayout> Layout;
	std::vector<std::shared_ptr<BindableResource>> Resources;
};

class ResourceSet final
{
public:
	ResourceSet(ResourceSetDescription&& description)
		: Layout(std::move(description.Layout))
		, Resources(std::move(description.Resources))
	{
	}

	const std::shared_ptr<ResourceLayout> Layout;
	const std::vector<std::shared_ptr<BindableResource>> Resources;
};
}
