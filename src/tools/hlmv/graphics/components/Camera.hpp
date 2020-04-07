#pragma once

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "graphics/GraphicsDefinitions.hpp"

#include "graphics/api/GraphicsDevice.hpp"

namespace graphics::components
{
enum class ProjectionMode
{
	Perspective,
	Orthographic
};

/**
*	@brief A single camera
*	Must be coupled with a @see engine::game::components::Transform component to have a location and orientation
*/
struct Camera final
{
	glm::vec3 BackgroundColor{0.f};

	//Which render passes to render with this camera
	RenderPasses CullingMask = RenderPass::All;

	ProjectionMode Projection = ProjectionMode::Perspective;

	//TODO: define proper initial values

	//Number of units to display in height in orthographic projection
	float OrthographicSize = 128.f;

	//FOV when using perspective projection
	float PerspectiveFieldOfView = glm::radians(65.f);

	float NearZ = 1.f;
	float FarZ = 16384.f;

	graphics::api::Viewport Viewport{0, 0, 1, 1, 0, 1};

	int Order = 0;
};
}
