#ifndef PROJECTILE_EMIT_SYSTEM_H
#define PROJECTILE_EMIT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class ProjectileEmitSystem : public System
{
public:
	ProjectileEmitSystem()
	{
		RequireComponent<ProjectileEmitterComponent>();
		RequireComponent<TransformComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubsrcibeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.symbol == SDLK_SPACE)
		{
			Logger::Log("Space pressed!");

			for (auto entity : GetSystemEntities())
			{
				if (entity.HasComponent<CameraFollowComponent>())
				{
					const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
					const auto transform = entity.GetComponent<TransformComponent>();
					const auto rigidBody = entity.GetComponent<RigidBodyComponent>();

					glm::vec2 projectilePosition = transform.position;

					if (entity.HasTag("player"))
					{
						const auto sprite = entity.GetComponent<SpriteComponent>();

						projectilePosition.x += (transform.scale.x * sprite.width / 2);
						projectilePosition.y += (transform.scale.y * sprite.height / 2);
					}

					glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
					int directionX = 0;
					int directionY = 0;

					if (rigidBody.velocity.x > 0) directionX = 1;
					if (rigidBody.velocity.x < 0) directionX = -1;
					if (rigidBody.velocity.y > 0) directionY = 1;
					if (rigidBody.velocity.y < 0) directionY = -1;

					projectileVelocity.x *= directionX;
					projectileVelocity.y *= directionY;

					Entity projectile = entity.registry->CreateEntity();
					projectile.Group("projectiles");
					projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1, 1), 0.0);
					projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
					projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
					projectile.AddComponent<BoxColliderComponent>(4, 4);
					projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
				}
			}
		}
	}

	void Update(std::unique_ptr<Registry>& registry)
	{
		for (auto entity : GetSystemEntities())
		{
			auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
			const auto transform = entity.GetComponent<TransformComponent>();

			// If emission frequency is zero, bypass reemission logic
			if (projectileEmitter.repeatFrequency == 0)
			{
				continue;
			}

			// Check if it is time to emit a new projectile
			if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency)
			{
				glm::vec2 projectilePosition = transform.position;

				if (entity.HasComponent<SpriteComponent>())
				{
					const auto sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += (transform.scale.x * sprite.width / 2);
					projectilePosition.y += (transform.scale.y * sprite.height / 2);
				}

				// Add a new projectile entity to the registry
				Entity projectile = registry->CreateEntity();
				projectile.Group("projectiles");
				projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1, 1), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
				projectile.AddComponent<BoxColliderComponent>(4, 4);
				projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

				// Update the projectile emitter component last emission to the current milliseconds
				projectileEmitter.lastEmissionTime = SDL_GetTicks();
			}
		}
	}
};

#endif
