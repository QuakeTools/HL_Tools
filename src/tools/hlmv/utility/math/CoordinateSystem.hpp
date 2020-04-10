#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace math
{
constexpr glm::vec3 DefaultForwardDirection{1, 0, 0};
constexpr glm::vec3 DefaultRightDirection{0, 1, 0};
constexpr glm::vec3 DefaultUpDirection{0, 0, 1};
constexpr double MaxAngle = 360.0;

constexpr glm::vec3 GetPosition(const glm::mat4x4& matrix)
{
	return matrix[3];
}

inline glm::vec3 GetForwardVector(const glm::mat4x4& matrix)
{
	return glm::normalize(glm::vec3(matrix[0]));
}

inline glm::vec3 GetRightVector(const glm::mat4x4& matrix)
{
	return glm::normalize(glm::vec3(matrix[1]));
}

inline glm::vec3 GetUpVector(const glm::mat4x4& matrix)
{
	return glm::normalize(glm::vec3(matrix[2]));
}

/**
*	@brief Gets the rotation as a quaternion from pitch, yaw and roll
*/
inline glm::quat PitchYawRollToRotation(const glm::vec3& vector)
{
	return 
		glm::vec3
		{
			glm::radians(vector.z),
			glm::radians(-vector.x),
			glm::radians(vector.y)
		};
}

/**
*	@brief Gets the rotation as pitch, yaw and roll from a quaternion
*/
inline glm::vec3 RotationToPitchYawRoll(const glm::quat& rotation)
{
	const auto euler = glm::eulerAngles(rotation);

	return
		{
			glm::degrees(-euler.y),
			glm::degrees(euler.z),
			glm::degrees(euler.x)
		};
}

inline float FixAngle(double angle)
{
	//Use high precision when fixing angles to reduce errors
	angle = std::fmod(angle, MaxAngle);

	while (angle < 0)
	{
		angle += MaxAngle;
	}

	return static_cast<float>(angle);
}

inline glm::vec3 FixAngles(const glm::vec3& angles)
{
	return {FixAngle(angles.x), FixAngle(angles.y), FixAngle(angles.z)};
}
}
