#pragma once

#include <entt/entity/registry.hpp>
#include <entt/signal/sigh.hpp>

namespace game
{
/**
*	@brief Represents a single world
*/
class World final
{
public:
	World() = default;
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	~World() = default;

	entt::registry* GetRegistry() { return &_registry; }

	void Update();

private:
	entt::registry _registry;

	entt::sigh<void(entt::registry&)> _updateSignal;

public:
	entt::sink<void(entt::registry&)> UpdateSink{_updateSignal};
};
}
