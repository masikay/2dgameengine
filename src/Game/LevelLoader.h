#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include <sol/sol.hpp>
#include <memory>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader();

	void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int levelNumber);
};

#endif

