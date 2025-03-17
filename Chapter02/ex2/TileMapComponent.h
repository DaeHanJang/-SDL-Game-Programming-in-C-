#pragma once

#include "SpriteComponent.h"
#include <vector>

class TileMapComponent : public SpriteComponent {
public:
	TileMapComponent(class Actor* owner, int drawOrder = 100);

	void Draw(SDL_Renderer* renderer) override;
	void SetTMTextures(SDL_Texture* textures, int size);

	void SetTileMap(const char* name);
	int GetTMSize() const { return mTMSize; }
	void SetTMSize(int size) { mTMSize = size; }
private:
	std::vector<std::vector<int>> mTileMap;
	int mTMSize;
};
