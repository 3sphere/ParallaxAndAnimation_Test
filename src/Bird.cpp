#include "Bird.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include <memory>
#include "Math.h"

Bird::Bird(Game* game) : Actor(game), mRightSpeed(0.0f), mDownSpeed(0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this, 7);
	std::vector<SDL_Texture*> animTextures = {
		game->GetTexture("Assets/BirdSprite/Bird01.png"),
		game->GetTexture("Assets/BirdSprite/Bird02.png"),
		game->GetTexture("Assets/BirdSprite/Bird03.png"),
		game->GetTexture("Assets/BirdSprite/Bird04.png"),
		game->GetTexture("Assets/BirdSprite/Bird05.png"),
		game->GetTexture("Assets/BirdSprite/Bird06.png"),
		game->GetTexture("Assets/BirdSprite/Bird07.png")
	};
	asc->SetFlip(SDL_FLIP_HORIZONTAL);
	asc->SetAnimTextures(animTextures);
	asc->SetAnimFPS(20.0f);
	asc->AddAnim("Fly", 0, 6);
	asc->SetAnim("Fly");
}

void Bird::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	}
	else if (pos.x > 1000.0f)
	{
		pos.x = 1000.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);
}

void Bird::ProcessKeyboard(const uint8_t* state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	if (state[SDL_SCANCODE_D])
	{
		mRightSpeed += 250.0f;
	}
	if (state[SDL_SCANCODE_A])
	{
		mRightSpeed -= 250.0f;
	}
	if (state[SDL_SCANCODE_S])
	{
		mDownSpeed += 300.0f;
	}
	if (state[SDL_SCANCODE_W])
	{
		mDownSpeed -= 300.0f;
	}
}