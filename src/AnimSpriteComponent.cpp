#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) : SpriteComponent(owner, drawOrder), mCurrFrame(0.0f), mAnimFPS(24.0f), mCurrAnim(0, 0), mLooping(true)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		mCurrFrame += mAnimFPS * deltaTime;

		while (mCurrFrame >= mCurrAnim.second + 1)
		{
			if(mLooping)
				mCurrFrame -= mCurrAnim.second - mCurrAnim.first + 1;
			else
			{
				mCurrFrame = mCurrAnim.second;
				break;
			}
		}

		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		SetTexture(mAnimTextures[0]);
		mCurrFrame = 0.0f;
	}
}

void AnimSpriteComponent::AddAnim(const std::string& name, int start, int end)
{
	auto iter = mAnims.find(name);
	if (iter == mAnims.end())
	{
		mAnims[name] = std::pair<int, int>(start, end);
	}
}

void AnimSpriteComponent::SetAnim(const std::string& name, bool looping)
{
	auto iter = mAnims.find(name);
	if (iter != mAnims.end())
	{
		mCurrAnim = mAnims[name];
		mCurrFrame = mAnims[name].first;
	}
	else
	{
		SDL_Log("Animation error: animation %s not found\n", name);
		if (mAnimTextures.size() > 0)
		{
			mCurrAnim.first = 0;
			mCurrAnim.second = mAnimTextures.size() - 1;
			mCurrFrame = 0.0f;
		}
		else
		{
			mCurrAnim.first = 0;
			mCurrAnim.second = 0;
			mCurrFrame = 0.0f;
		}
	}

	mLooping = looping;
}