#include "ui/dockpanels/AssetExplorer.hpp"

namespace ui::dockpanels
{
constexpr int AssetRole = Qt::UserRole;

constexpr char AssetPropertyName[] = "Asset";

constexpr int FileNameColumn = 0;
constexpr int ReloadColumn = 1;
constexpr int CloseColumn = 2;

AssetExplorer::AssetExplorer(EditorAssetList* assets, QWidget* parent)
	: QWidget(parent)
	, _assets(assets)
{
	_ui.setupUi(this);

	_ui.LoadedAssets->setColumnWidth(FileNameColumn, 100);

	auto header = _ui.LoadedAssets->horizontalHeader();
	header->setSectionResizeMode(FileNameColumn, QHeaderView::ResizeMode::Stretch);
	header->setSectionResizeMode(ReloadColumn, QHeaderView::ResizeMode::Fixed);
	header->setSectionResizeMode(CloseColumn, QHeaderView::ResizeMode::Fixed);
	
	auto assetList = _assets->GetAssets();

	_ui.LoadedAssets->setRowCount(assetList.size());

	int row = 0;

	for (auto asset : assetList)
	{
		AddAsset(row++, asset);
	}

	OnCurrentAssetChanged(_assets->GetCurrentAsset());

	connect(_assets, &EditorAssetList::AssetAdded, this, &AssetExplorer::OnAssetAdded);
	connect(_assets, &EditorAssetList::AssetRemoved, this, &AssetExplorer::OnAssetRemoved);
	connect(_assets, &EditorAssetList::CurrentAssetChanged, this, &AssetExplorer::OnCurrentAssetChanged);

	connect(_ui.LoadedAssets, &QTableWidget::currentCellChanged, this, &AssetExplorer::OnCurrentCellChanged);
}

AssetExplorer::~AssetExplorer() = default;

void AssetExplorer::OnAssetAdded(const QSharedPointer<EditorAsset>& asset)
{
	const auto rowIndex = _ui.LoadedAssets->rowCount();

	_ui.LoadedAssets->setRowCount(rowIndex + 1);

	AddAsset(rowIndex, asset);
}

void AssetExplorer::OnAssetRemoved(const QSharedPointer<EditorAsset>& asset)
{
	for (int i = 0; i < _ui.LoadedAssets->rowCount(); ++i)
	{
		auto item = _ui.LoadedAssets->item(i, FileNameColumn);

		if (item->data(AssetRole).value<QSharedPointer<EditorAsset>>() == asset)
		{
			_ui.LoadedAssets->removeRow(i);
			disconnect(asset.get(), &EditorAsset::FileNameChanged, this, &AssetExplorer::OnAssetFileNameChanged);
			break;
		}
	}
}

void AssetExplorer::OnCurrentAssetChanged(const QSharedPointer<EditorAsset>& asset)
{
	QTableWidgetItem* current = nullptr;

	for (int i = 0; i < _ui.LoadedAssets->rowCount(); ++i)
	{
		auto item = _ui.LoadedAssets->item(i, FileNameColumn);

		if (item->data(AssetRole).value<QSharedPointer<EditorAsset>>() == asset)
		{
			current = item;
			break;
		}
	}

	_ui.LoadedAssets->setCurrentItem(current);
}

void AssetExplorer::OnAssetFileNameChanged(const QString& fileName)
{
	auto asset = qobject_cast<EditorAsset*>(sender());

	for (int i = 0; i < _ui.LoadedAssets->rowCount(); ++i)
	{
		auto item = _ui.LoadedAssets->item(i, FileNameColumn);

		if (item->data(AssetRole).value<QSharedPointer<EditorAsset>>().get() == asset)
		{
			item->setText(fileName);
			break;
		}
	}
}

void AssetExplorer::OnCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
	QSharedPointer<EditorAsset> asset;

	if (currentRow != -1)
	{
		auto current = _ui.LoadedAssets->item(currentRow, FileNameColumn);
		asset = current->data(AssetRole).value<QSharedPointer<EditorAsset>>();
	}

	_assets->SetCurrentAsset(asset);
}

void AssetExplorer::AddAsset(const int row, const QSharedPointer<EditorAsset>& asset)
{
	connect(asset.get(), &EditorAsset::FileNameChanged, this, &AssetExplorer::OnAssetFileNameChanged);

	auto item = new QTableWidgetItem(asset->GetFileName());

	item->setData(AssetRole, QVariant::fromValue(asset));

	_ui.LoadedAssets->setItem(row, FileNameColumn, item);

	auto style = this->style();

	auto reloadButton = new QPushButton(style->standardIcon(QStyle::StandardPixmap::SP_BrowserReload), {}, _ui.LoadedAssets);

	reloadButton->setFixedWidth(40);

	reloadButton->setProperty(AssetPropertyName, QVariant::fromValue(asset));

	connect(reloadButton, &QPushButton::clicked, this, &AssetExplorer::OnReloadAsset);

	_ui.LoadedAssets->setIndexWidget(_ui.LoadedAssets->model()->index(row, ReloadColumn), reloadButton);

	auto closeButton = new QPushButton(style->standardIcon(QStyle::StandardPixmap::SP_DockWidgetCloseButton), {}, _ui.LoadedAssets);

	closeButton->setFixedWidth(40);

	closeButton->setProperty(AssetPropertyName, QVariant::fromValue(asset));

	connect(closeButton, &QPushButton::clicked, this, &AssetExplorer::OnCloseAsset);

	_ui.LoadedAssets->setIndexWidget(_ui.LoadedAssets->model()->index(row, CloseColumn), closeButton);
}

void AssetExplorer::OnReloadAsset()
{
	auto button = sender();

	auto asset = button->property(AssetPropertyName).value<QSharedPointer<EditorAsset>>();

	emit _assets->ReloadAsset(asset);
}

void AssetExplorer::OnCloseAsset()
{
	auto button = sender();

	auto asset = button->property(AssetPropertyName).value<QSharedPointer<EditorAsset>>();

	emit _assets->CloseAsset(asset);
}
}
