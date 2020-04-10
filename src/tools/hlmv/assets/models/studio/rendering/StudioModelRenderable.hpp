#pragma once

#include "graphics/IRenderable.hpp"

#include "engine/models/studio/EditableStudioModel.hpp"

namespace models::studio
{
class StudioModelRenderable final : public graphics::IRenderable
{
public:
	StudioModelRenderable(models::studio::EditableStudioModel* model)
		: _model(model)
	{
	}

	~StudioModelRenderable() = default;

	std::uint8_t GetRenderPasses() const override
	{
	}

	graphics::RenderOrderKey GetRenderOrderKey(const glm::vec3& cameraPosition) override
	{
	}

	void Render(graphics::api::GraphicsDevice& gd, graphics::SceneContext& sc, const graphics::RenderPasses::RenderPasses renderPass) override
	{
	}

private:
	models::studio::EditableStudioModel* _model;
};
}
