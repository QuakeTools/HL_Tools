#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <entt/core/fwd.hpp>

#include "assets/AssetType.hpp"

struct AssetTypes final
{
	AssetTypes() = default;
	~AssetTypes() = default;

	std::vector<AssetType*> GetAssetTypes() const
	{
		std::vector<AssetType*> assetTypes;
		assetTypes.reserve(_assetTypes.size());

		std::transform(_assetTypes.begin(), _assetTypes.end(), std::back_inserter(assetTypes), [](const auto& type)
			{
				return type.second.get();
			});

		return assetTypes;
	}

	void Add(std::unique_ptr<AssetType>&& assetType)
	{
		if (!assetType)
		{
			throw std::invalid_argument("Asset type must be valid");
		}

		if (_assetTypes.find(assetType->GetTypeId()) != _assetTypes.end())
		{
			throw std::invalid_argument("Asset type already registered");
		}

		_assetTypes.emplace(assetType->GetTypeId(), std::move(assetType));
	}

	AssetType* FindById(const entt::id_type typeId) const
	{
		if (auto it = _assetTypes.find(typeId); it != _assetTypes.end())
		{
			return it->second.get();
		}

		return nullptr;
	}

private:
	std::unordered_map<entt::id_type, std::unique_ptr<AssetType>> _assetTypes;
};
