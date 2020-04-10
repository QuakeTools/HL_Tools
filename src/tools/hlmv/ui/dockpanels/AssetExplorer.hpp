#pragma once

#include <QPushButton>
#include <QWidget>

#include "ui_AssetExplorer.h"

#include "ui/EditorAsset.hpp"

namespace ui::dockpanels
{
/**
*	@brief Displays the list of loaded assets
*/
class AssetExplorer final : public QWidget
{
	Q_OBJECT

public:
	AssetExplorer(EditorAssetList* assets, QWidget* parent = nullptr);
	~AssetExplorer();

private slots:
	void OnAssetAdded(const QSharedPointer<EditorAsset>& asset);

	void OnAssetRemoved(const QSharedPointer<EditorAsset>& asset);

	void OnCurrentAssetChanged(const QSharedPointer<EditorAsset>& asset);

	void OnAssetFileNameChanged(const QString& fileName);

	void OnCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

	void OnReloadAsset();

	void OnCloseAsset();

private:
	void AddAsset(const int row, const QSharedPointer<EditorAsset>& asset);
	
private:
	Ui_AssetExplorer _ui;

	EditorAssetList* _assets;
};
}
