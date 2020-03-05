#pragma once
#include <vector>
#include "Math.h"

class Actor
{
public:
	enum State { EActive, EPaused, EDead};
	Actor(class Game* game);
	virtual ~Actor();
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// getters/setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }
	class Game* GetGame() { return mGame; }

private:
	State mState;
	Vector2 mPosition; // Centre position of actor
	float mScale;	   // Uniform scale of actor
	float mRotation;   // Rotation angle in radians
	std::vector<class Component*> mComponents;
	class Game* mGame;
};