#pragma once

#include <memory>

#include <QObject>
#include <QSharedPointer>

#include "assets/AssetTypes.hpp"

#include "ui/EditorAsset.hpp"

namespace ui
{
/**
*	@brief Stores program state
*/
class ProgramState final : public QObject
{
	Q_OBJECT

public:
	ProgramState(QObject* parent = nullptr)
		: QObject(parent)
		, _assetTypes(std::make_unique<AssetTypes>())
		, _assetList(new EditorAssetList(this))
	{
	}

	~ProgramState() = default;

	AssetTypes* GetAssetTypes() { return _assetTypes.get(); }

	EditorAssetList* GetAssets() { return _assetList; }

private:
	std::unique_ptr<AssetTypes> _assetTypes;

	EditorAssetList* _assetList;
};
}
