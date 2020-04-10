#pragma once

#include <entt/entity/registry.hpp>

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <QSharedPointer>
#include <QWidget>

#include "ui_CameraInfo.h"

#include "game/World.hpp"
#include "game/components/Entities.hpp"

#include "graphics/components/Camera.hpp"

#include "ui/EditorAsset.hpp"

#include "utility/math/CoordinateSystem.hpp"

namespace ui::dockpanels
{
class CameraInfo final : public QWidget
{
	Q_OBJECT

public:
	CameraInfo(QWidget* parent = nullptr)
		: QWidget(parent)
	{
		_ui.setupUi(this);

		setEnabled(false);
	}

	~CameraInfo() = default;

	QSize sizeHint() const override
	{
		return QSize(300, -1);
	}

public slots:
	void UpdateCameraInfo(const QSharedPointer<EditorAsset>& asset)
	{
		const auto enabled = asset && asset->GetCurrentCamera() != entt::null;

		setEnabled(enabled);

		if (enabled)
		{
			auto& registry = *asset->GetWorld()->GetRegistry();

			const auto cameraEntity = asset->GetCurrentCamera();

			const auto& localToWorld = registry.get<game::components::LocalToWorld>(cameraEntity);
			const auto& rotationEulerXYZ = registry.get<game::components::RotationEulerXYZ>(cameraEntity);

			//Calculate the absolute rotation of the camera without using LocalToWorld to ensure the values are accurate for each axis of rotation
			auto rotationInDegrees = rotationEulerXYZ.Value;

			for (auto parent = game::components::GetParent(registry, cameraEntity); parent != entt::null; parent = game::components::GetParent(registry, parent))
			{
				if (auto parentRotation = registry.try_get<game::components::RotationEulerXYZ>(parent); parentRotation)
				{
					rotationInDegrees += parentRotation->Value;
				}
			}

			rotationInDegrees = math::FixAngles(rotationInDegrees);

			SetVector(
				localToWorld.GetPosition(),
				_ui.PositionX,
				_ui.PositionY,
				_ui.PositionZ);

			SetVector(
				rotationInDegrees,
				_ui.RotationX,
				_ui.RotationY,
				_ui.RotationZ);

			SetVector(
				localToWorld.GetForward(),
				_ui.ForwardX,
				_ui.ForwardY,
				_ui.ForwardZ);

			SetVector(
				localToWorld.GetRight(),
				_ui.RightX,
				_ui.RightY,
				_ui.RightZ);

			SetVector(
				localToWorld.GetUp(),
				_ui.UpX,
				_ui.UpY,
				_ui.UpZ);
		}
		else
		{
			_ui.PositionX->setText({});
			_ui.PositionY->setText({});
			_ui.PositionZ->setText({});

			_ui.RotationX->setText({});
			_ui.RotationY->setText({});
			_ui.RotationZ->setText({});

			_ui.ForwardX->setText({});
			_ui.ForwardY->setText({});
			_ui.ForwardZ->setText({});

			_ui.RightX->setText({});
			_ui.RightY->setText({});
			_ui.RightZ->setText({});

			_ui.UpX->setText({});
			_ui.UpY->setText({});
			_ui.UpZ->setText({});
		}
	}

private:
	static void SetVector(const glm::vec3& vector, QLineEdit* x, QLineEdit* y, QLineEdit* z)
	{
		x->setText(QString::number(vector.x));
		y->setText(QString::number(vector.y));
		z->setText(QString::number(vector.z));
	}

private:
	Ui_CameraInfo _ui;
};
}
