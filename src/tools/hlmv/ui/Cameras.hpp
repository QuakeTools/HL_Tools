#pragma once

#include <memory>
#include <vector>

#include <entt/entity/entity.hpp>

#include <QObject>

#include "ui/CameraOperators.hpp"
#include "ui/EditorAsset.hpp"

namespace ui
{
class Cameras final : public QObject
{
	Q_OBJECT
public:
	Cameras(EditorAssetList* assets, QObject* parent = nullptr)
		: QObject(parent)
		, _assets(assets)
	{
		_cameraOperators.emplace_back(std::make_unique<ArcballCameraOperator>());
		_cameraOperators.emplace_back(std::make_unique<FreeLookCameraOperator>());
		_cameraOperators.emplace_back(std::make_unique<FirstPersonCameraOperator>());

		_currentCameraOperator = _cameraOperators[0].get();
	}

	CameraOperator* GetCurrentCameraOperator() { return _currentCameraOperator; }

	int GetCurrentCameraOperatorIndex() const
	{
		int index = 0;

		for (const auto& cameraOperator : _cameraOperators)
		{
			if (cameraOperator.get() == _currentCameraOperator)
			{
				return index;
			}

			++index;
		}

		return -1;
	}

	void SetCameraOperatorByIndex(int index)
	{
		auto newOperator = _cameraOperators[index].get();

		if (newOperator != _currentCameraOperator)
		{
			_currentCameraOperator->Reset();

			_currentCameraOperator = newOperator;
		}
	}

signals:
	void CameraInfoChanged();

private:
	EditorAssetList* const _assets;

	std::vector<std::unique_ptr<CameraOperator>> _cameraOperators;

	CameraOperator* _currentCameraOperator{};
};
}
