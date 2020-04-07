#include <entt/entity/registry.hpp>

#include "game/components/Entities.hpp"

#include "graphics/Scene.hpp"
#include "graphics/SceneContext.hpp"
#include "graphics/SceneRenderer.hpp"

#include "graphics/api/FrameBuffer.hpp"
#include "graphics/api/GraphicsDevice.hpp"

#include "graphics/components/Camera.hpp"

namespace graphics
{
void SceneRenderer::RenderAllStages(entt::registry& registry, Scene& scene, api::GraphicsDevice& gd, SceneContext& sc)
{
	sc.SetScene(&scene);

	const auto depthClear = gd.IsDepthRangeZeroToOne() ? 0.f : 1.f;

	gd.ClearDepthStencil(depthClear);
	sc.UpdateLightingBuffers(gd);

	const auto fbWidth = gd.GetSwapChainFrameBuffer().GetWidth();
	const auto fbHeight = gd.GetSwapChainFrameBuffer().GetHeight();

	registry.sort<components::Camera>([](const auto& lhs, const auto& rhs)
		{
			if (lhs.Order != rhs.Order)
			{
				return lhs.Order < rhs.Order;
			}

			return &lhs < &rhs;
		});

	registry.view<components::Camera>().each([=, &registry, &scene, &gd, &sc](const entt::entity entity, const components::Camera& camera)
		{
			const auto& localToWorld = registry.get<game::components::LocalToWorld>(entity);

			auto viewport = camera.Viewport;

			//Resize the viewport values from viewport space [0..1] to window space [0..size]
			viewport.X *= fbWidth;
			viewport.Y *= fbHeight;
			viewport.Width *= fbWidth;
			viewport.Height *= fbHeight;

			gd.SetViewport(viewport);
			gd.SetScissorRect(viewport.X, viewport.Y, viewport.Width, viewport.Height);
			gd.ClearColorTarget({camera.BackgroundColor, 1});

			sc.UpdateCameraBuffers(gd, camera, localToWorld);

			const auto position = localToWorld.GetPosition();

			for (RenderPasses pass = 0; pass < sizeof(RenderPasses) * 8; ++pass)
			{
				const auto mask = static_cast<RenderPass::RenderPass>(1 << pass);

				if ((camera.CullingMask & mask) != 0)
				{
					Render(registry, scene, gd, sc, mask, position);
				}
			}
		});

	//Free any remaining memory
	ClearVisibleObjects();
}

void SceneRenderer::Render(
	entt::registry& registry,
	Scene& scene,
	api::GraphicsDevice& gd,
	SceneContext& sc,
	const RenderPass::RenderPass pass,
	const glm::vec3& viewPosition)
{
	_renderQueue.Clear();

	ClearVisibleObjects();

	CollectFreeObjects(registry, scene, sc, pass);

	_renderQueue.AddRange(_visibleObjects, viewPosition);

	_renderQueue.Sort();

	for (auto renderable : _renderQueue)
	{
		renderable->Render(gd, sc, pass);
	}
}

void SceneRenderer::CollectFreeObjects(entt::registry& registry, Scene& scene, SceneContext& sc, const RenderPass::RenderPass renderPass)
{
	RenderContext context{scene, sc, registry, _allocator, _visibleObjects, renderPass};

	scene._collectObjectsSignal.publish(context);
}

void SceneRenderer::ClearVisibleObjects()
{
	for (auto renderable : _visibleObjects)
	{
		renderable->~IRenderable();
	}

	_visibleObjects.clear();

	//TODO: if more independent object types are allocated from the allocator this will need to be moved
	_allocator.Clear();
}
}
