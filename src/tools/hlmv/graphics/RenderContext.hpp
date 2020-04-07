#pragma once

#include <memory_resource>
#include <vector>

#include <entt/entity/registry.hpp>

#include "graphics/GraphicsDefinitions.hpp"

namespace graphics
{
struct IRenderable;
class Scene;
class SceneContext;

/**
*	@brief Provides a means of allocating objects meant to live only as long as the current frame
*/
struct RenderContextAllocator final
{
	RenderContextAllocator() = default;
	RenderContextAllocator(const RenderContextAllocator&) = delete;
	RenderContextAllocator& operator=(const RenderContextAllocator&) = delete;
	~RenderContextAllocator() = default;

	template<typename T, typename... Args>
	[[nodiscard]] T* Allocate(Args&&... args)
	{
		//TODO: use polymorphic_allocator::new_object instead once C++20 is in use
		auto memory = _resource.allocate(sizeof(T), alignof(T));

		auto instance = new (memory) T(std::forward<Args>(args)...);

		return instance;
	}

	void Clear()
	{
		_resource.release();
	}

private:
	std::pmr::unsynchronized_pool_resource _resource;
};

struct RenderContext final
{
	Scene& Scene;
	SceneContext& SceneContext;
	entt::registry& Registry;
	RenderContextAllocator& Allocator;
	std::vector<IRenderable*>& Renderables;

	const RenderPasses RenderPass;

	RenderContext(graphics::Scene& scene, graphics::SceneContext& sceneContext,
		entt::registry& registry,
		RenderContextAllocator& allocator,
		std::vector<IRenderable*>& renderables,
		const RenderPasses renderPass)
		: Scene(scene)
		, SceneContext(sceneContext)
		, Registry(registry)
		, Allocator(allocator)
		, Renderables(renderables)
		, RenderPass(renderPass)
	{
	}
};
}
