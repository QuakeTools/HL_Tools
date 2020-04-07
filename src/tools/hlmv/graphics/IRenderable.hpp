#pragma once

#include <glm/vec3.hpp>

#include "graphics/GraphicsDefinitions.hpp"

namespace graphics
{
class SceneContext;

namespace api
{
class GraphicsDevice;
}

struct IRenderable
{
	virtual ~IRenderable() = 0 {}

	virtual RenderOrderKey GetRenderOrderKey(const glm::vec3& cameraPosition) = 0;

	virtual void Render(api::GraphicsDevice& gd, SceneContext& sc, const RenderPass::RenderPass renderPass) = 0;
};
}
