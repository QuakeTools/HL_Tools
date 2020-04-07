#include <stdexcept>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "game/components/Entities.hpp"

#include "graphics/SceneContext.hpp"

#include "graphics/api/DeviceBuffer.hpp"
#include "graphics/api/FrameBuffer.hpp"
#include "graphics/api/GraphicsDevice.hpp"
#include "graphics/api/ResourceFactory.hpp"

#include "graphics/components/Camera.hpp"

#include "utility/math/MathDefinitions.hpp"

namespace graphics
{
static constexpr glm::vec3 DefaultLookDirection{1, 0, 0};
static constexpr glm::vec3 DefaultUpDirection{0, 0, 1};
static constexpr glm::vec3 DefaultRightDirection{0, 1, 0};

static glm::mat4x4 CreatePerspectiveMatrix(float fov, float aspectRatio, float nearZ, float farZ)
{
	if (fov <= 0.0f || fov >= math::PI<float>)
	{
		throw std::invalid_argument("fov");
	}

	if (nearZ <= 0.0f)
	{
		throw std::out_of_range("near");
	}

	if (farZ <= 0.0f)
	{
		throw std::out_of_range("far");
	}

	float yScale = 1.0f / std::tanf(fov * 0.5f);
	float xScale = yScale / aspectRatio;

	glm::mat4x4 result;

	result[0][0] = xScale;
	result[0][1] = result[0][2] = result[0][3] = 0.0f;

	result[1][1] = yScale;
	result[1][0] = result[1][2] = result[1][3] = 0.0f;

	result[2][0] = result[2][1] = 0.0f;
	auto negFarRange = farZ == std::numeric_limits<float>::infinity() ? -1.0f : farZ / (nearZ - farZ);
	result[2][2] = negFarRange;
	result[2][3] = -1.0f;

	result[3][0] = result[3][1] = result[3][3] = 0.0f;
	result[3][2] = nearZ * negFarRange;

	return result;
}

static glm::mat4x4 CreatePerspective(
	const bool clipSpaceYInverted,
	const bool useReverseDepth,
	float fov,
	float aspectRatio,
	float nearZ, float farZ)
{
	glm::mat4x4 persp;
	if (useReverseDepth)
	{
		persp = CreatePerspectiveMatrix(fov, aspectRatio, farZ, nearZ);
	}
	else
	{
		persp = CreatePerspectiveMatrix(fov, aspectRatio, nearZ, farZ);
	}

	if (clipSpaceYInverted)
	{
		persp *= glm::mat4x4(
			1, 0, 0, 0,
			0, -1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	return persp;
}

//GoldSource's coordinate system points Z up, X forward, and Y left
	//See https://developer.valvesoftware.com/wiki/Coordinates
	//Need to scale X so inputs produce the correct results,
	//And rotate yaw 180 degrees so yaw 0 points to the east
	//This does not look like GoldSource's code because it uses a different approach to view matrix construction
	//GoldSource modifies the global settings by applying the inverse of the rotation and view origin
	//This is the "standard" way of applying view settings by using matrices in normal model-view techniques
	//Pitch goes first because in model viewer yaw is relative to the model, while pitch is relative to the world
static glm::mat4 GetRotationMatrix(const float pitch, const float yaw, const float roll)
{
	return
		glm::rotate(math::PI<float> + yaw, glm::vec3{0, 0, 1}) *
		glm::rotate(pitch, glm::vec3{0, 1, 0}) *
		glm::rotate(roll, glm::vec3{1, 0, 0}) *
		glm::scale(glm::vec3{-1, 1, 1});
}

void SceneContext::UpdateCameraBuffers(api::GraphicsDevice& gd, const components::Camera& camera, const game::components::LocalToWorld& localToWorld)
{
	const auto fbWidth = gd.GetSwapChainFrameBuffer().GetWidth();
	const auto fbHeight = gd.GetSwapChainFrameBuffer().GetHeight();

	glm::mat4x4 projection;

	switch (camera.Projection)
	{
	case components::ProjectionMode::Perspective:
	{
		projection = CreatePerspective(
			gd.IsClipSpaceYInverted(),
			gd.IsDepthRangeZeroToOne(),
			camera.PerspectiveFieldOfView,
			static_cast<float>(fbWidth) / fbHeight,
			camera.NearZ,
			camera.FarZ);
		break;
	}

	case components::ProjectionMode::Orthographic:
	{
		//TODO
		break;
	}

	default: throw std::invalid_argument("Unsupported projection mode");
	}

	const auto rotationQuat = localToWorld.GetRotation();
	const auto position = localToWorld.GetPosition();

	const auto eulerRotation = glm::eulerAngles(rotationQuat);

	//Translate view matrix to GoldSource coordinate system
	const auto lookRotation = GetRotationMatrix(eulerRotation.y, eulerRotation.z, eulerRotation.x);

	const auto lookDirection = glm::vec3(lookRotation * glm::vec4{DefaultLookDirection, 1});

	const auto upDirection = glm::vec3{lookRotation * glm::vec4{ DefaultUpDirection, 1 }};

	const auto view = glm::lookAt(position, position + lookDirection, upDirection);

	gd.UpdateBuffer(ProjectionBuffer, 0, projection);
	gd.UpdateBuffer(ViewBuffer, 0, view);
}

void SceneContext::UpdateLightingBuffers(api::GraphicsDevice& gd)
{
	//TODO
}

void SceneContext::CreateDeviceResources(api::GraphicsDevice& gd)
{
	ProjectionBuffer = gd.GetResourceFactory().CreateBuffer({sizeof(glm::mat4x4), graphics::api::BufferUsage::Uniform | graphics::api::BufferUsage::Dynamic});
	ViewBuffer = gd.GetResourceFactory().CreateBuffer({sizeof(glm::mat4x4), graphics::api::BufferUsage::Uniform | graphics::api::BufferUsage::Dynamic});
}

void SceneContext::DestroyDeviceResources()
{
	ViewBuffer.reset();
	ProjectionBuffer.reset();
}

void SceneContext::RecreateWindowSizedResources(api::GraphicsDevice& gd)
{
	//TODO
}
}
