#pragma once

#include <entt/signal/sigh.hpp>

namespace graphics
{
class RenderContext;
class SceneRenderer;

class Scene final
{
	friend class SceneRenderer;

public:
	Scene() = default;

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	~Scene() = default;

private:
	entt::sigh<void(RenderContext&)> _collectObjectsSignal;

public:
	entt::sink<void(RenderContext&)> CollectObjectsSink{_collectObjectsSignal};
};
}
