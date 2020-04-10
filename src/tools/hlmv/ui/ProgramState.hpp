#pragma once

#include <memory>

#include <QObject>
#include <QSharedPointer>

#include "assets/AssetTypes.hpp"

#include "ui/Cameras.hpp"
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
		, _cameras(new Cameras(_assetList, this))
	{
	}

	~ProgramState() = default;

	AssetTypes* GetAssetTypes() { return _assetTypes.get(); }

	EditorAssetList* GetAssets() { return _assetList; }

	Cameras* GetCameras() { return _cameras; }

private:
	std::unique_ptr<AssetTypes> _assetTypes;

	EditorAssetList* _assetList;

	Cameras* _cameras;
};
}
