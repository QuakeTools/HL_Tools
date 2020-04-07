#pragma once

#include "game/components/Model.hpp"

#include "models/studio/EditableStudioModel.hpp"

namespace game
{
/**
*	@brief Contains studio model data
*/
struct StudioModelData
{
	std::unique_ptr<models::studio::EditableStudioModel> Model;
};

/**
*	@brief Contains studio model device resources
*/
struct StudioModelDeviceResources
{
	//TODO
	int i; //TODO: remove this variable once there are some resources (empty component optimizations remove this component from iterations)
};
}
