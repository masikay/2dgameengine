#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem : public System
{
public:
	CollisionSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}

	void Update(std::unique_ptr<EventBus>& eventBus)
	{
		// Check all entities that have a boxcollider
		// to see if they are colliding with each other
		auto entities = GetSystemEntities();

		for (auto i = entities.begin(); i != entities.end(); i++)
		{
			Entity a = *i;
			auto aTransform = a.GetComponent<TransformComponent>();
			auto& aCollider = a.GetComponent<BoxColliderComponent>();

			for (auto j = i + 1; j != entities.end(); j++)
			{
				Entity b = *j;
				auto bTransform = b.GetComponent<TransformComponent>();
				auto& bCollider = b.GetComponent<BoxColliderComponent>();

				bool haveCollision = Check_AABB_Collision(
					aTransform.position.x + aCollider.offset.x,
					aTransform.position.y + aCollider.offset.y,
					aCollider.width * aTransform.scale.x,
					aCollider.height * aTransform.scale.y,
					bTransform.position.x + bCollider.offset.x,
					bTransform.position.y + bCollider.offset.y, 
					bCollider.width * bTransform.scale.x,
					bCollider.height * bTransform.scale.y
				);

				if (haveCollision)
				{
					aCollider.collision = true;
					bCollider.collision = true;
					//Logger::Log("Collision detected between entity id " + std::to_string(a.GetId()) + " and entity id " + std::to_string(b.GetId()));
					
					eventBus->EmitEvent<CollisionEvent>(a, b);
				}
				else
				{
					aCollider.collision = false;
					bCollider.collision = false;
				}
			}
		}
	}

	bool Check_AABB_Collision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
	{
		return (
			aX < bX + bW &&
			aX + aW > bX &&
			aY < bY + bH &&
			aY + aH > bY
			);	
	}
};

#endif
