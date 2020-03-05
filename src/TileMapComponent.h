#pragma once
#include "SpriteComponent.h"
#include <string>
#include <vector>

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(class Actor* owner, int width, int height, int size, int drawOrder = 100);
	void Draw(SDL_Renderer* renderer) override;
	void LoadTileMapFromFile(const std::string& fileName);

private:
	std::vector<std::vector<int>> mTileMap;

	// Dimensions in pixels
	int mTileMapWidth;
	int mTileMapHeight;
	int mTileSize;
};