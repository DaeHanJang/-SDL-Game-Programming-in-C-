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
	SDL_Texture* mTexture; //그릴 텍스처
	int mDrawOrder; //화가 알고리즘에서 사용하는 그리기 순서
	//텍스처의 너비/높이
	int mTexWidth;
	int mTexHeight;
};
