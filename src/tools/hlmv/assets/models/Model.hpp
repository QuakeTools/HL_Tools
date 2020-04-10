#pragma once

#include "assets/Asset.hpp"

namespace models
{
struct Model : public Asset
{
};

template<typename TDerived>
struct TModel : public Model
{
	entt::id_type GetTypeId() const override final { return entt::type_info<TDerived>::id(); }
};
}
