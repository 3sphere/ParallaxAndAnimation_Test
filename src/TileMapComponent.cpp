#include "TileMapComponent.h"
#include <fstream>
#include "Actor.h"
#include <sstream>

TileMapComponent::TileMapComponent(Actor* owner, int width, int height, int size, int drawOrder) : 
	SpriteComponent(owner, drawOrder), mTileMapWidth(width), mTileMapHeight(height), mTileSize(size)
{

}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		int tileMapRows = mTileMapHeight / mTileSize;
		int tileMapColumns = mTileMapWidth / mTileSize;

		int width = static_cast<int>(mTileMap[0].size() * mTileSize * mOwner->GetScale());
		int height = static_cast<int>(mTileMap.size() * mTileSize * mOwner->GetScale());

		SDL_Rect destRect, srcRect;

		for (int j = 0; j < mTileMap.size(); ++j)
		{
			for (int i = 0; i < mTileMap[0].size(); ++i)
			{
				destRect.w = static_cast<int>(mTileSize * mOwner->GetScale());
				destRect.h = static_cast<int>(mTileSize * mOwner->GetScale());
				destRect.x = static_cast<int>(mOwner->GetPosition().x - width / 2 + i * mTileSize * mOwner->GetScale());
				destRect.y = static_cast<int>(mOwner->GetPosition().y - height / 2 + j * mTileSize * mOwner->GetScale());

				srcRect.x = (mTileMap[j][i] % tileMapColumns) * mTileSize;
				srcRect.y = static_cast<int>(mTileMap[j][i] / tileMapColumns) * mTileSize;
				srcRect.w = mTileSize;
				srcRect.h = mTileSize;

				SDL_RenderCopyEx(renderer,
					mTexture,
					&srcRect,
					&destRect, 
					mOwner->GetRotation() * Pi / 180.0f,
					nullptr,
					SDL_FLIP_NONE);
			}
		}
	}
}

void TileMapComponent::LoadTileMapFromFile(const std::string& fileName)
{
	mTileMap.clear();
	std::ifstream file(fileName);

	for (std::string line; std::getline(file, line); )
	{
		std::vector<int> temp;
		std::istringstream ss(line);

		for (int i; ss >> i;)
		{
			temp.push_back(i);
			if (ss.peek() == ',')
				ss.ignore();
		}

		mTileMap.push_back(std::move(temp));
	}
}