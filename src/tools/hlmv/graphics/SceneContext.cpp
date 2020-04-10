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

#include "utility/math/CoordinateSystem.hpp"
#include "utility/math/MathDefinitions.hpp"

namespace graphics
{
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

	const auto position = localToWorld.GetPosition();

	const auto view = glm::lookAt(position, position + localToWorld.GetForward(), localToWorld.GetUp());

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
