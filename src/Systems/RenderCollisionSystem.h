#ifndef RENDER_COLLISION_SYSTEM_H
#define RENDER_COLLISION_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include <SDL2/SDL.h>

class RenderCollisionSystem: public System
{
public:
	RenderCollisionSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}

	void Update(SDL_Renderer* renderer, SDL_Rect& camera)
	{
		// Loop all the entities the system is interested in
		for (auto entity: GetSystemEntities())
		{
			// Update entity position based on its velocity
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto collider = entity.GetComponent<BoxColliderComponent>();

			SDL_Rect boundingBox = {
				static_cast<int>(transform.position.x + collider.offset.x - camera.x),
				static_cast<int>(transform.position.y + collider.offset.y - camera.y),
				static_cast<int>(collider.width * transform.scale.x),
				static_cast<int>(collider.height * transform.scale.y)
			};

			SDL_SetRenderDrawColor(renderer, (collider.collision ? 255 : 0), (collider.collision ? 0 : 255) , 0, 255);
			SDL_RenderDrawRect(renderer,&boundingBox);
		}
	}
};

#endif