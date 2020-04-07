#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "graphics/GraphicsDefinitions.hpp"

#include "graphics/api/DeviceBuffer.hpp"
#include "graphics/api/Texture.hpp"

namespace models
{
class Model;
}

namespace game
{
struct Renderable
{
	/**
	*	@brief Which render passes this renderable requires
	*/
	std::uint8_t RenderPasses;

	std::shared_ptr<models::Model> Model;
};
}
