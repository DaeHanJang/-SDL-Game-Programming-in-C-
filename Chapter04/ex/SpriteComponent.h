#pragma once

#include "Component.h"
#include "SDL.h"

class SpriteComponent : public Component {
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100); //그리기 순서값이 낮을 수록 더 뒤쪽에서 그림
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }
protected:
	SDL_Texture* mTexture;
	int mDrawOrder;
	int mTexWidth;
	int mTexHeight;
};
