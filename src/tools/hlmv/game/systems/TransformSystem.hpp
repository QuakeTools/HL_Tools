#pragma once

#include <entt/entity/registry.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "game/components/Entities.hpp"

namespace game::systems
{
inline void UpdateRotations(entt::registry& registry)
{
	//Update rotation components from Euler first
	registry.view<components::RotationEulerXYZ, components::Rotation>().each([](const auto& eulerXYZ, auto& rotation)
		{
			rotation.Value = glm::quat(eulerXYZ.Value);
		});
}

inline void UpdateLocalToWorld(entt::registry& registry)
{
	//Update LocalToWorld for all entities, including entities that have parents
	registry.view<components::LocalToWorld>().each([&registry](const auto entity, components::LocalToWorld& localToWorld)
		{
			if (auto translation = registry.try_get<components::Translation>(entity); translation)
			{
				localToWorld.Value = glm::translate(translation->Value);
			}
			else
			{
				localToWorld.Value = glm::identity<glm::mat4x4>();
			}

			if (auto rotation = registry.try_get<components::Rotation>(entity); rotation)
			{
				localToWorld.Value *= glm::mat4_cast(rotation->Value);
			}

			if (auto scale = registry.try_get<components::Scale>(entity); scale)
			{
				localToWorld.Value *= scale->Value;
			}
		});
}

inline void UpdateLocalToParent(entt::registry& registry)
{
	//Sort by hierarchy: children are larger than their parents, siblings are sorted by order in the chain if possible
	registry.sort<components::LocalToParent>([&registry](const entt::entity lhs, const entt::entity rhs)
		{
			const auto& lhsHierarchy = registry.get<components::Hierarchy>(lhs);
			const auto& rhsHierarchy = registry.get<components::Hierarchy>(rhs);

			if (rhsHierarchy.Parent == lhs ||
				lhsHierarchy.Next == rhs)
			{
				return true;
			}

			if (lhsHierarchy.Parent != rhs &&
				rhsHierarchy.Next != lhs)
			{
				//Consistently sort entities by parent id if parents differ
				//This maintains a relative order between entities
				if (lhsHierarchy.Parent < rhsHierarchy.Parent)
				{
					return true;
				}

				if (lhsHierarchy.Parent == rhsHierarchy.Parent)
				{
					return &lhs < &rhs;
				}
			}

			return false;
		});

	//Update LocalToWorld and LocalToParent for all entities in a hierarchy
	registry.view<components::LocalToParent>()
		.each([&registry](const entt::entity entity, auto& localToParent)
			{
				auto& localToWorld = registry.get<components::LocalToWorld>(entity);
				const auto& hierarchy = registry.get<components::Hierarchy>(entity);

				//At this point LocalToWorld is the LocalToParent transformation
				localToParent.Value = localToWorld.Value;

				if (hierarchy.Parent != entt::null)
				{
					const auto& parentTransform = registry.get<components::LocalToWorld>(hierarchy.Parent);

					localToWorld.Value = parentTransform.Value * localToWorld.Value;
				}
			});
}
}
