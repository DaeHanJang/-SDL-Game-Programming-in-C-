#include "TileMapComponent.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Actor.h"

TileMapComponent::TileMapComponent(class Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder), mTMSize(0) {}

void TileMapComponent::Draw(SDL_Renderer* renderer) {
	for (int y = 0; y < mTileMap.size(); y++) {
		for (int x = 0; x < mTileMap[y].size(); x++) {
			if (mTileMap[y][x] == -1) continue;

			SDL_Rect rdr;
			rdr.w = static_cast<int>(mTMSize);
			rdr.h = static_cast<int>(mTMSize);
			rdr.x = static_cast<int>(mTMSize * x);
			rdr.y = static_cast<int>(mTMSize * y);
			SDL_Rect tmr;
			tmr.w = static_cast<int>(mTMSize);
			tmr.h = static_cast<int>(mTMSize);
			tmr.x = static_cast<int>(mTMSize * (mTileMap[y][x] % (mTexWidth / mTMSize)));
			tmr.y = static_cast<int>(mTMSize * (mTileMap[y][x] / (mTexWidth / mTMSize)));

			SDL_RenderCopyEx(renderer, mTexture, &tmr, &rdr, -Math::ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
		}
	}
}

void TileMapComponent::SetTMTextures(SDL_Texture* textures, int size) {
	SetTMSize(size);
	SetTexture(textures);
}

void TileMapComponent::SetTileMap(const char* name) {
	int index = 0;
	std::fstream fs;
	fs.open(name, std::ios::in);
	while (!fs.eof()) {
		std::string str = "", temp = "";
		std::getline(fs, str);
		if (!str.empty()) mTileMap.emplace_back(std::vector<int>());
		std::istringstream iss(str);
		while (std::getline(iss, temp, ',')) mTileMap[index].push_back(std::stoi(temp));
		index++;
	}
	fs.close();
}
