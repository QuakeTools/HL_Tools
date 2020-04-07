#pragma once

#include <vector>

#include <entt/entity/registry.hpp>

#include "graphics/RenderContext.hpp"
#include "RenderQueue.hpp"

namespace graphics
{
struct IRenderable;
class Scene;

/**
*	@brief Handles the rendering of a scene by enumerating all active cameras and rendering them in order
*/
class SceneRenderer final
{
public:
	SceneRenderer() = default;
	SceneRenderer(const SceneRenderer&) = delete;
	SceneRenderer& operator=(const SceneRenderer&) = delete;
	~SceneRenderer() = default;

	void RenderAllStages(entt::registry& registry, Scene& scene, api::GraphicsDevice& gd, SceneContext& sc);

private:
	void Render(
		entt::registry& registry,
		Scene& scene,
		api::GraphicsDevice& gd,
		SceneContext& sc,
		const RenderPass::RenderPass pass,
		const glm::vec3& viewPosition);

	void CollectFreeObjects(entt::registry& registry, Scene& scene, SceneContext& sc, const RenderPass::RenderPass renderPass);

	void ClearVisibleObjects();

private:
	RenderQueue _renderQueue;

	RenderContextAllocator _allocator;

	std::vector<IRenderable*> _visibleObjects;
};
}
