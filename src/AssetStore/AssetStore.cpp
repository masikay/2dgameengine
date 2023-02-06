#include "AssetStore.h"
#include "../Logger/Logger.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

AssetStore::AssetStore()
{
	Logger::Log("AssetStore constructor called!");
}

AssetStore::~AssetStore()
{
	ClearAssets();
	Logger::Log("AssetStore destructor called!");
}

void AssetStore::ClearAssets()
{
	for (auto texture : textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();

	for (auto font : fonts)
	{
		TTF_CloseFont(font.second);
	}
	fonts.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath)
{
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (!surface)
	{
		Logger::Err("Could not open image file " + filePath);
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	
	// Add the texture to the map
	textures.emplace(assetId, texture);

	Logger::Log("New texture added to the Asset Store with id = " + assetId);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId)
{
	return textures[assetId];
}

std::map<std::string, SDL_Texture*>& AssetStore::GetTextures()
{
	return textures;
}

void AssetStore::AddFont(const std::string& assetId, const std::string& filePath, int fontSize)
{
	TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);

	if (!font)
	{
		Logger::Err("Could not open font file " + filePath);
		return;
	}
	
	fonts.emplace(assetId, font);

	Logger::Log("New font added to the Asset Store with id = " + assetId);
}

TTF_Font* AssetStore::GetFont(const std::string& assetId)
{
	return fonts[assetId];
}

std::map<std::string, TTF_Font*>& AssetStore::GetFonts()
{
	return fonts;
}
