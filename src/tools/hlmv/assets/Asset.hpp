#pragma once

#include <entt/core/fwd.hpp>
#include <entt/core/type_info.hpp>

/**
*	@brief Base class for all assets
*/
struct Asset
{
	virtual ~Asset() = 0 {}

	virtual entt::id_type GetTypeId() const = 0;
};

/**
*	@brief Base class for all assets that provides an implementation for GetTypeId
*	Pass the most derived type as TDerived
*/
template<typename TDerived>
struct TAsset : public Asset
{
	entt::id_type GetTypeId() const override final { return entt::type_info<TDerived>::id(); }
};
