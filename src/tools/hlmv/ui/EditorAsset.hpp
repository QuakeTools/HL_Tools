#pragma once

#include <memory>

#include <entt/entity/entity.hpp>

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVector>

#include "assets/Asset.hpp"

#include "game/World.hpp"

namespace ui
{
/**
*	@brief Represents a single loaded asset
*/
class EditorAsset final : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool HasUnsavedChanged READ HasUnsavedChanges WRITE SetHasUnsavedChanges NOTIFY UnsavedChangesChanged)

public:
	EditorAsset(const QString& fileName, std::unique_ptr<Asset>&& asset, QObject* parent = nullptr)
		: QObject(parent)
		, _fileName(fileName)
		, _asset(std::move(asset))
	{
	}

	~EditorAsset() = default;

	QString GetFileName() const { return _fileName; }

	Asset* GetAsset() { return _asset.get(); }

	game::World* GetWorld() { return &_world; }

	entt::entity GetCurrentCamera() { return _currentCamera; }

	void SetCurrentCamera(entt::entity entity)
	{
		_currentCamera = entity;
	}

	bool HasUnsavedChanges() const { return _hasUnsavedChanges; }

	void SetHasUnsavedChanges(bool value)
	{
		const bool oldState = _hasUnsavedChanges;

		_hasUnsavedChanges = value;

		if (oldState != _hasUnsavedChanges)
		{
			emit UnsavedChangesChanged(_hasUnsavedChanges);
		}
	}

signals:
	void FileNameChanged(const QString& fileName);

	void UnsavedChangesChanged(bool state);

public slots:
	void SetFileName(const QString& value)
	{
		_fileName = value;

		emit FileNameChanged(_fileName);
	}

private:
	QString _fileName;

	std::unique_ptr<Asset> _asset;

	game::World _world;

	entt::entity _currentCamera{entt::null};

	bool _hasUnsavedChanges = false;
};

/**
*	@brief Stores the list of loaded assets and emits signals when assets are added and removed
*/
class EditorAssetList final : public QObject
{
	Q_OBJECT

public:
	EditorAssetList(QObject* parent = nullptr)
		: QObject(parent)
	{
	}

	~EditorAssetList() = default;

	QVector<QSharedPointer<EditorAsset>> GetAssets() const { return _assets; }

	QSharedPointer<EditorAsset> GetCurrentAsset() { return _currentAsset; }

signals:
	void AssetAdded(const QSharedPointer<EditorAsset>& asset);

	void AssetRemoved(const QSharedPointer<EditorAsset>& asset);

	void CurrentAssetChanged(const QSharedPointer<EditorAsset>& current, const QSharedPointer<EditorAsset>& previous);

	/**
	*	@brief Try to reload the asset
	*/
	void ReloadAsset(const QSharedPointer<EditorAsset>& asset);

	/**
	*	@brief Try to close the asset
	*/
	void CloseAsset(const QSharedPointer<EditorAsset>& asset);

public slots:
	void AddAsset(const QSharedPointer<EditorAsset>& asset)
	{
		if (!asset)
		{
			return;
		}

		if (_assets.contains(asset))
		{
			return;
		}

		_assets.append(asset);
		emit AssetAdded(asset);
	}

	void RemoveAsset(const QSharedPointer<EditorAsset>& asset)
	{
		if (const auto index = _assets.indexOf(asset); index != -1)
		{
			_assets.remove(index);
			emit AssetRemoved(asset);

			if (asset == _currentAsset)
			{
				SetCurrentAsset({});
			}
		}
	}

	void RemoveAllAssets()
	{
		//Remove back to front to avoid unnecessary shifting of elements
		while (!_assets.empty())
		{
			auto asset = _assets.back();

			_assets.remove(_assets.size() - 1);
			emit AssetRemoved(asset);
		}

		SetCurrentAsset({});
	}

	void SetCurrentAsset(const QSharedPointer<EditorAsset>& asset)
	{
		if (asset && !_assets.contains(asset))
		{
			return;
		}

		if (asset == _currentAsset)
		{
			return;
		}

		const auto previous = _currentAsset;

		_currentAsset = asset;

		emit CurrentAssetChanged(_currentAsset, previous);
	}

private:
	QVector<QSharedPointer<EditorAsset>> _assets;

	QSharedPointer<EditorAsset> _currentAsset;
};
}
