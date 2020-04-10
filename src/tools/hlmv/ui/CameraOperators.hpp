#pragma once

#include <entt/entity/registry.hpp>

#include <glm/vec2.hpp>

#include <QMouseEvent>

namespace ui
{
/**
*	@brief Maps mouse and keyboard inputs to the camera
*/
class CameraOperator
{
protected:
	CameraOperator() = default;

public:
	virtual ~CameraOperator() = 0 {}

	virtual void Reset()
	{
		_oldCoordinates = glm::vec2{0.f};
		_trackedButtons = 0;
	}

	virtual void MouseEvent(entt::registry& registry, entt::entity cameraEntity, QMouseEvent& event) = 0;

	virtual void UpdateCamera(entt::registry& registry, entt::entity cameraEntity) {}

protected:
	glm::vec2 _oldCoordinates{0.f};
	Qt::MouseButtons _trackedButtons{};
};

/**
*	@brief Camera operator that allows the user to rotate the camera around a position
*/
struct ArcballCameraOperator final : public CameraOperator
{
	ArcballCameraOperator() = default;
	~ArcballCameraOperator() = default;

	void MouseEvent(entt::registry& registry, entt::entity cameraEntity, QMouseEvent& event) override;

	void UpdateCamera(entt::registry& registry, entt::entity cameraEntity) override;
};

/**
*	@brief Camera operator that allows the user to move in the 3D world freely using the mouse and keyboard
*/
struct FreeLookCameraOperator final : public CameraOperator
{
	FreeLookCameraOperator() = default;
	~FreeLookCameraOperator() = default;

	void MouseEvent(entt::registry& registry, entt::entity cameraEntity, QMouseEvent& event) override;
};

/**
*	@brief Camera operator that allows the user view an object in first person view
*/
struct FirstPersonCameraOperator final : public CameraOperator
{
	FirstPersonCameraOperator() = default;
	~FirstPersonCameraOperator() = default;

	void MouseEvent(entt::registry& registry, entt::entity cameraEntity, QMouseEvent& event) override;

	void UpdateCamera(entt::registry& registry, entt::entity cameraEntity) override;
};
}
