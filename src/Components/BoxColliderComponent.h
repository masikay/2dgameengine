#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include <glm/glm.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

struct BoxColliderComponent
{
	int width;
	int height;
	glm::vec2 offset;
	bool collision;


	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0))
	{
		this->width = width;
		this->height = height;
		this->offset = offset;
		this->collision = false;
	}
};

#endif
