#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <SDL_image.h>
#include "SpriteComponent.h"
#include "Bird.h"
#include "BGSpriteComponent.h"
#include "TileMapComponent.h"

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mUpdatingActors(false)
{

}

bool Game::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Failed to initialise SDL. Error: %s\n", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("SDL Game", 100, 100, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window. Error: %s\n", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer. Error: %s\n", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		SDL_Log("Failed to initialise SDL_image. Error: %s\n", IMG_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::Run()
{
	while (mIsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mBird->ProcessKeyboard(state);
}

void Game::Update()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// Update actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Add pending actors to current actors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Clear dead actors
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::Render()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// Render sprites
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	// Bird
	mBird = new Bird(this);
	mBird->SetPosition(Vector2(100.0f, 384.0f));
	mBird->SetScale(3.0f);

	// Background actor
	Actor* background = new Actor(this);
	background->SetPosition(Vector2(512.0f, 384.0f));
	// Sky background
	BGSpriteComponent* bg = new BGSpriteComponent(background, 1);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgTextures = {
		GetTexture("Assets/Background/sky.png")
	};
	bg->SetBGTextures(bgTextures);
	bg->SetScrollSpeed(0.0f);
	// Clouds behind mountain
	bg = new BGSpriteComponent(background, 2);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgTextures = {
		GetTexture("Assets/Background/clouds_BG.png"),
		GetTexture("Assets/Background/clouds_BG.png")
	};
	bg->SetBGTextures(bgTextures);
	bg->SetScrollSpeed(-20.0f);
	// Mountain
	bg = new BGSpriteComponent(background, 3);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgTextures = {
		GetTexture("Assets/Background/mountains.png"),
		GetTexture("Assets/Background/mountains.png")
	};
	bg->SetBGTextures(bgTextures);
	bg->SetScrollSpeed(-40.0f);
	// Clouds in front of mountain
	for (int i = 3; i >= 1; i--)
	{
		std::string filename = "Assets/Background/clouds_MG_" + std::to_string(i) + ".png";
		bg = new BGSpriteComponent(background, 4 + (3-i) * 2);
		bg->SetScreenSize(Vector2(1024.0f, 768.0f));
		bgTextures = {
			GetTexture(filename),
			GetTexture(filename)
		};
		bg->SetBGTextures(bgTextures);
		bg->SetScrollSpeed(-90.0f + (i - 1) * 15.0f);
	}
	
}

void Game::UnloadData()
{
	// Delete actors
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Delete textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// 'swap and pop' to avoid copies that would be created when using erase()
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* component)
{
	int drawOrder = component->GetDrawOrder();
	auto iter = mSprites.begin();

	for (; iter != mSprites.end(); ++iter)
	{
		if (drawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	mSprites.insert(iter, component);
}

void Game::RemoveSprite(SpriteComponent* component)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), component);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surface = IMG_Load(fileName.c_str());
		if (!surface)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}
		SDL_FreeSurface(surface);

		mTextures.emplace(fileName.c_str(), tex);
	}

	return tex;
}