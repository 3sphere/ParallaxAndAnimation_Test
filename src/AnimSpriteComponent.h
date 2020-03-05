#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <utility>
#include <map>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override;
	void AddAnim(const std::string& name, int start, int end);
	
	// Getters/setters
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnim(const std::string& name, bool looping = true);
	std::pair<int, int> GetCurrAnim() const { return mCurrAnim; }
	std::map<std::string, std::pair<int, int>> GetAnims() const { return mAnims; }

private:
	std::vector<SDL_Texture*> mAnimTextures;
	std::map<std::string, std::pair<int, int>> mAnims;
	float mCurrFrame;
	float mAnimFPS;
	std::pair<int, int> mCurrAnim;
	bool mLooping;
};