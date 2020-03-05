#include "Character.h"
#include "Game.h"
#include "AnimSpriteComponent.h"

Character::Character(Game* game) : Actor(game), mRightSpeed(0.0f), mState(Walk)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> textures = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png"),
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png")
	};
	asc->SetAnimTextures(textures);
	asc->SetAnimFPS(6.0f);
	asc->AddAnim("Walk", 0, 5);
	asc->AddAnim("Jump", 6, 14);
	asc->AddAnim("Punch", 15, 17);
	asc->SetAnim("Jump", false);
}

void Character::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	if (pos.x < 50.0f)
	{
		pos.x = 50.0f;
	}
	if (pos.x > 1000.0f)
	{
		pos.x = 1000.0f;
	}
	SetPosition(pos);
}

void Character::ProcessKeyboard(const uint8_t* state)
{
	mRightSpeed = 0.0f;
	if (state[SDL_SCANCODE_D])
	{
		mRightSpeed += 200.0f;
	}
	if (state[SDL_SCANCODE_A])
	{
		mRightSpeed -= 200.0f;
	}
}