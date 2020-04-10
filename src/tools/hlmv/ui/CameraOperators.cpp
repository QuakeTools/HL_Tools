#include <glm/trigonometric.hpp>

#include "game/components/Entities.hpp"

#include "ui/CameraOperators.hpp"

#include "utility/math/CoordinateSystem.hpp"

namespace ui
{
void FreeLookCameraOperator::MouseEvent(entt::registry& registry, entt::entity cameraEntity, QMouseEvent& event)
{
	switch (event.type())
	{
	case QEvent::MouseButtonPress:
	{
		_oldCoordinates.x = event.x();
		_oldCoordinates.y = event.y();

		_trackedButtons |= event.button();

		event.accept();
		break;
	}

	case QEvent::MouseButtonRelease:
	{
		_trackedButtons &= ~event.button();
		event.accept();
		break;
	}

	case QEvent::MouseMove:
	{
		if (_trackedButtons & event.buttons())
		{
			auto& translation = registry.get<game::components::Translation>(cameraEntity);
			auto& rotationXYZ = registry.get<game::components::RotationEulerXYZ>(cameraEntity);
			const auto& localToWorld = registry.get<game::components::LocalToWorld>(cameraEntity);

			if (event.buttons() & Qt::MouseButton::LeftButton)
			{
				rotationXYZ.Value.x -= static_cast<float>(event.y() - _oldCoordinates.y);
				rotationXYZ.Value.y -= static_cast<float>(event.x() - _oldCoordinates.x);

				_oldCoordinates.x = event.x();
				_oldCoordinates.y = event.y();
			}
			else if (event.buttons() & Qt::MouseButton::RightButton)
			{
				//Slide the camera along the current view direction
				const auto direction = localToWorld.GetForward();

				translation.Value += direction * -(static_cast<float>(event.y() - _oldCoordinates.y));

				_oldCoordinates.x = event.x();
				_oldCoordinates.y = event.y();
			}
		}

		event.accept();
		break;
	}

	default: break;
	}
}
}
