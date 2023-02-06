#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <algorithm>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class RenderSystem : public System
{
public:
	RenderSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera)
	{
		// Create a vector with both Sprite and Transform component of all entities
		struct RenderableEntity
		{
			TransformComponent transformComponent;
			SpriteComponent spriteComponent;
		};

		std::vector<RenderableEntity> renderableEntities;

		for (auto entity : GetSystemEntities())
		{
			RenderableEntity renderableEntity;
			renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
			renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();

			// Bypass rendering entities if they are aouside the camera view
			bool isOutsideCameraView = (
				renderableEntity.transformComponent.position.x + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < camera.x ||
				renderableEntity.transformComponent.position.x > camera.x + camera.w ||
				renderableEntity.transformComponent.position.y + (renderableEntity.transformComponent.scale.y * renderableEntity.spriteComponent.height) < camera.y ||
				renderableEntity.transformComponent.position.y > camera.y + camera.h
			);

			// Cull sprites that are outside the camera view (and are not fixed)
			if (isOutsideCameraView && !renderableEntity.spriteComponent.isFixed)
			{
				continue;
			}

			renderableEntities.emplace_back(renderableEntity);
		}


		// Sort the vector by the z-index value
		std::sort(
			renderableEntities.begin(),
			renderableEntities.end(),
			[](const RenderableEntity& a, const RenderableEntity& b) {
				return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
			}
		);

		// Loop all the entities the system is interested in
		for (auto entity : renderableEntities)
		{
			// Update entity position based on its velocity
			const auto transform = entity.transformComponent;
			const auto sprite = entity.spriteComponent;

			// Set the source rectangle of our original stprite texture
			SDL_Rect srcRect = sprite.srcRect;

			// Set the destination rectangle with the x, y position to be rendered
			SDL_Rect dstRec = {
				static_cast<int>(transform.position.x - (!sprite.isFixed ? camera.x : 0)),
				static_cast<int>(transform.position.y - (!sprite.isFixed ? camera.y : 0)),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_RenderCopyEx(
				renderer,
				assetStore->GetTexture(sprite.assetId),
				&srcRect,
				&dstRec,
				transform.rotation,
				NULL,
				sprite.flip
			);
		}
	}
};

#endif