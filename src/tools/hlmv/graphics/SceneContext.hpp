#pragma once

#include <memory>

namespace game::components
{
struct LocalToWorld;
}

namespace graphics
{
class Scene;

namespace api
{
class DeviceBuffer;
class GraphicsDevice;
}

namespace components
{
struct Camera;
}

class SceneContext final
{
public:
	SceneContext() = default;
	SceneContext(const SceneContext&) = delete;
	SceneContext& operator=(const SceneContext&) = delete;
	~SceneContext() = default;

	Scene* GetScene() { return _currentScene; }

	void SetScene(Scene* scene)
	{
		_currentScene = scene;
	}

	void UpdateCameraBuffers(api::GraphicsDevice& gd, const components::Camera& camera, const game::components::LocalToWorld& localToWorld);

	void UpdateLightingBuffers(api::GraphicsDevice& gd);

	void CreateDeviceResources(api::GraphicsDevice& gd);

	void DestroyDeviceResources();

	void RecreateWindowSizedResources(api::GraphicsDevice& gd);

	std::shared_ptr<api::DeviceBuffer> ProjectionBuffer;
	std::shared_ptr<api::DeviceBuffer> ViewBuffer;

private:
	Scene* _currentScene = nullptr;
};
}
