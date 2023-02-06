#ifndef ASSET_STORE_H
#define ASSET_STORE_H

#include <map>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

class AssetStore
{
private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	// TODO: create a map for audio

public:
	AssetStore();
	~AssetStore();

	void ClearAssets();
	
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* GetTexture(const std::string& assetId);
	std::map<std::string, SDL_Texture*>& GetTextures();

	void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);
	TTF_Font* GetFont(const std::string& assetId);
	std::map<std::string, TTF_Font*>& GetFonts();
};

#endif



