#ifndef RENDER_TEXT_SYSTEM_H
#define RENDER_TEXT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TextLabelComponent.h"
#include "../AssetStore/AssetStore.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class RenderTextSystem : public System
{
public:
	RenderTextSystem()
	{
		RequireComponent<TextLabelComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera)
	{
		// Loop all the entities the system is interested in
		for (auto entity : GetSystemEntities())
		{
			auto textLabel = entity.GetComponent<TextLabelComponent>();

			SDL_Surface* surface = TTF_RenderText_Blended(
				assetStore->GetFont(textLabel.assetId),
				textLabel.text.c_str(),
				textLabel.color);

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			int labelWidth = 0;
			int labelHeight = 0;

			SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

			SDL_Rect dstRect = {
				static_cast<int>(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
				static_cast<int>(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
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
		}
	}
};

#endif
