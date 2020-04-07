#include "game/World.hpp"

namespace game
{
void World::Update()
{
	_updateSignal.publish(_registry);
}
}
