#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"

class DamageSystem : public System
{
public:

	DamageSystem()
	{
		RequireComponent<BoxColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubsrcibeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& event)
	{
		Entity a = event.a;
		Entity b = event.b;
		//Logger::Log("Collision event emitted: " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

		if (a.BelongsToGroup("projectiles") && b.HasTag("player"))
		{
			OnProjectileHitsPlayer(a, b); // "a" is the projectile, "b" is the player
		}

		if (b.BelongsToGroup("projectiles") && a.HasTag("player"))
		{
			OnProjectileHitsPlayer(b, a); // "b" is the projectile, "a" is the player
		}

		if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies"))
		{
			OnProjectileHitsEnemy(a, b); // "a" is the projectile, "b" is the enemy
		}

		if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies"))
		{
			OnProjectileHitsEnemy(b, a); // "b" is the projectile, "a" is the enemy
		}
	}

	void OnProjectileHitsPlayer(Entity projectile, Entity player)
	{
		auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

		if (!projectileComponent.isFriendly)
		{

			// Reduce the health of the player by the projectile hitPercentDamage
			auto& health = player.GetComponent<HealthComponent>();
			health.healthPercentage -= projectileComponent.hitPercentDamage;

			if (health.healthPercentage <= 0)
			{
				player.Kill();
			}

			// Kill the projectile
			projectile.Kill();
		}
	}

	void OnProjectileHitsEnemy(Entity projectile, Entity enemy)
	{
		auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

		if (projectileComponent.isFriendly)
		{
			// Reduce the health of the player by the projectile hitPercentDamage
			auto& health = enemy.GetComponent<HealthComponent>();
			health.healthPercentage -= projectileComponent.hitPercentDamage;

			if (health.healthPercentage <= 0)
			{
				enemy.Kill();
			}

			// Kill the projectile
			projectile.Kill();
		}
	}

	void Update()
	{

	}
};

#endif
