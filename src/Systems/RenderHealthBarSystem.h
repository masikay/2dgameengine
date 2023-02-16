#ifndef RENDER_HEALTH_BAR_SYSTEM_H
#define RENDER_HEALTH_BAR_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/HealthComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>

class RenderHealthBarSystem : public System
{
public:
	const SDL_Color red = { 255, 0, 0 };
	const SDL_Color green = { 0, 255, 0 };
	const SDL_Color orange = { 255, 172, 28 };
	const SDL_Color gray = { 152, 152, 152 };

	RenderHealthBarSystem()
	{

		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<HealthComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera)
	{
		// Loop all the entities the system is interested in
		for (auto entity : GetSystemEntities())
		{
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();
			const auto health = entity.GetComponent<HealthComponent>();
			
			SDL_Color color = health.healthPercentage > 70 ? green : (health.healthPercentage > 30 ? orange : red);

			if (entity.BelongsToGroup("enemies"))
			{
				const int barWidth = 27;

				int healthLabelPositionX = transform.position.x + (sprite.width * transform.scale.x) + 5;
				int healthLabelPositionY = transform.position.y - 10;

				SDL_Surface* surface = TTF_RenderText_Blended(
					assetStore->GetFont("pico8-font-5"),
					(std::to_string(health.healthPercentage) + "%").c_str(),
					color);

				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_FreeSurface(surface);

				int labelWidth = 0;
				int labelHeight = 0;

				SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

				SDL_Rect dstRect = {
					static_cast<int>(healthLabelPositionX - (sprite.isFixed ? 0 : camera.x)),
					static_cast<int>(healthLabelPositionY - (sprite.isFixed ? 0 : camera.y)),
					labelWidth,
					labelHeight
				};

				SDL_RenderCopy(
					renderer,
					texture,
					NULL,
					&dstRect
				);

				SDL_Rect fullHealthBar = {
					static_cast<int>(healthLabelPositionX - 3 - (sprite.isFixed ? 0 : camera.x)),
					static_cast<int>(healthLabelPositionY + 12 - (sprite.isFixed ? 0 : camera.y)),
					barWidth,
					5
				};

				int remainingHealthWidth = static_cast<int>(health.healthPercentage * barWidth / 100);

				if (remainingHealthWidth > barWidth)
				{
					remainingHealthWidth = barWidth;
				}

				SDL_Rect actualHealthBar = {
					static_cast<int>(healthLabelPositionX - 3 - (sprite.isFixed ? 0 : camera.x)),
					static_cast<int>(healthLabelPositionY + 12 - (sprite.isFixed ? 0 : camera.y)),
					remainingHealthWidth,
					5
				};

				SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, gray.a);
				SDL_RenderDrawRect(renderer, &fullHealthBar);
				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(renderer, &actualHealthBar);
			}
			else if (entity.HasTag("player"))
			{
				const int barWidth = 150;

				int healthBarPositionX = 10;
				int healthBarPositionY = 10;

				SDL_Surface* surface = TTF_RenderText_Blended(
					assetStore->GetFont("pico8-font-8"),
					(std::to_string(health.healthPercentage) + "%").c_str(),
					color);

				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_FreeSurface(surface);

				int labelWidth = 0;
				int labelHeight = 0;

				SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

				SDL_Rect dstRect = {
					static_cast<int>(healthBarPositionX + barWidth + 5),
					static_cast<int>(healthBarPositionY),
					labelWidth,
					labelHeight
				};

				SDL_RenderCopy(
					renderer,
					texture,
					NULL,
					&dstRect
				);

				SDL_DestroyTexture(texture);

				SDL_Rect fullHealthBar = {
					healthBarPositionX,
					healthBarPositionY,
					barWidth,
					labelHeight
				};

				int remainingHealthWidth = static_cast<int>(health.healthPercentage * barWidth / 100);

				SDL_Rect actualHealthBar = {
					healthBarPositionX,
					healthBarPositionY,
					remainingHealthWidth,
					labelHeight
				};

				SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, gray.a);
				SDL_RenderDrawRect(renderer, &fullHealthBar);
				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(renderer, &actualHealthBar);
			}
		}
	}
};

#endif