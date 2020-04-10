#pragma once

#include <entt/entity/registry.hpp>

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "game/components/Entities.hpp"

#include "graphics/GraphicsDefinitions.hpp"

#include "graphics/api/GraphicsDevice.hpp"

#include "utility/math/CoordinateSystem.hpp"
#include "utility/math/MathDefinitions.hpp"

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
	glm::vec4 BackgroundColor{0.f, 0.f, 0.f, 1.f};

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

/**
*	@brief Used by the arcball camera type to define the orbit position
*/
struct ArcballParameters final
{
	glm::vec3 Position{0.f};
	float Distance{0.f};
	float Pitch{0.f};
	float Yaw{0.f};
};

/**
*	@brief Sets the arcball parameters for the given camera on its transform components
*/
inline void SetArcballCameraParameters(entt::registry& registry, entt::entity cameraEntity)
{
	const auto& arcballParameters = registry.get<ArcballParameters>(cameraEntity);
	auto& translation = registry.get<game::components::Translation>(cameraEntity);
	auto& rotationXYZ = registry.get<game::components::RotationEulerXYZ>(cameraEntity);

	//First create a vector that represents a position at distance without any rotation applied
	auto cameraPosition = math::DefaultForwardDirection * -arcballParameters.Distance;

	//Now apply rotation
	cameraPosition =
		glm::rotate(glm::radians(arcballParameters.Yaw), math::DefaultUpDirection) *
		glm::rotate(glm::radians(-arcballParameters.Pitch), math::DefaultRightDirection) *
		glm::vec4{cameraPosition, 1};

	//Make relative to target position
	cameraPosition += arcballParameters.Position;

	//Set camera variables
	translation.Value = cameraPosition;
	rotationXYZ.Value.x = arcballParameters.Pitch;
	rotationXYZ.Value.y = arcballParameters.Yaw;
}
}
