#ifndef PROJECTILE_COMPONENT_H
#define PROJECTILE_COMPONENT_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

struct ProjectileComponent
{
	bool isFriendly;
	int hitPercentDamage;
	int duration;
	int startTime;

	ProjectileComponent(bool isFriendly = false, int hitPercentDamage = 0,	int duration = 0)
	{
		this->isFriendly = isFriendly;
		this->hitPercentDamage = hitPercentDamage;
		this->duration = duration;
		this->startTime = SDL_GetTicks();
	}
};

#endif
