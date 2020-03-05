#pragma once
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <string>

class Game
{
public:
	Game();
	bool Init();
	void Run();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddSprite(class SpriteComponent* component);
	void RemoveSprite(class SpriteComponent* component);

	SDL_Texture* GetTexture(const std::string& fileName);

private:
	void ProcessInput();
	void Update();
	void Render();
	void LoadData();
	void UnloadData();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;

	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// Game specific data
	class Bird* mBird;
};