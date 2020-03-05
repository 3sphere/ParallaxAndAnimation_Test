#pragma once
#include "Actor.h"

class Character : public Actor
{
public:
	enum State {
		Walk, Jump, Punch
	};
	Character(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
	float GetRightSpeed() const { return mRightSpeed; }

private:
	float mRightSpeed;
	State mState;
};