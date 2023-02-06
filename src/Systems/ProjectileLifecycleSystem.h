#ifndef PROJECTILE_LIFECYCLE_SYSTEM_H
#define PROJECTILE_LIFECYCLE_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class ProjectileLifecycleSystem : public System
{
public:
	ProjectileLifecycleSystem()
	{
		RequireComponent<ProjectileComponent>();
	}

	void Update()
	{
		for (auto entity : GetSystemEntities())
		{
			const auto projectile = entity.GetComponent<ProjectileComponent>();

			if (SDL_GetTicks()  - projectile.startTime > projectile.duration)
			{
				entity.Kill();
			}
		}
	}
};

#endif
