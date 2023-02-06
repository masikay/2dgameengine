#ifndef PROJECTILE_EMITTER_COMPONENT_H
#define PROJECTILE_EMITTER_COMPONENT_H

#include <glm/glm.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

struct ProjectileEmitterComponent
{
	glm::vec2 projectileVelocity;
	int repeatFrequency;
	int projectileDuration;
	int hitPercentDamage;
	bool isFriendly;
	int lastEmissionTime;

	ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), int repeatFrequency = 0, int projectileDuration = 10000, int hitPercentDamage = 10, bool isFriendly = false)
	{
		this->projectileVelocity = projectileVelocity;
		this->repeatFrequency = repeatFrequency;
		this->projectileDuration = projectileDuration;
		this->hitPercentDamage = hitPercentDamage;
		this->isFriendly = isFriendly;
		this->lastEmissionTime = SDL_GetTicks();
	}
};

#endif
