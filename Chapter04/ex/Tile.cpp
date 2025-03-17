#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"

Tile::Tile(class Game* game)
	: Actor(game), mParent(nullptr), f(0.f), g(0.f), h(0.f), 
	mBlocked(false), mSprite(nullptr), mTileState(EDefault), mSelected(false) {
	mSprite = new SpriteComponent(this);
	UpdateTexture();
}

void Tile::SetTileState(TileState state) {
	mTileState = state;
	UpdateTexture();
}

void Tile::ToggleSelect() {
	mSelected = !mSelected;
	UpdateTexture();
}

void Tile::UpdateTexture() {
	std::string text;
	switch (mTileState) {
		case EStart:
			if (mSelected) text = "Assets/TileTanSelected.png";
			else text = "Assets/TileTan.png";
			break;
		case EBase:
			if (mSelected) text = "Assets/TileGreenSelected.png";
			else text = "Assets/TileGreen.png";
			break;
		case EPath:
			if (mSelected) text = "Assets/TileGreySelected.png";
			else text = "Assets/TileGrey.png";
			break;
		case EDefault:
			if (mSelected) text = "Assets/TileBrownSelected.png";
			else text = "Assets/TileBrown.png";
			break;
	}
	mSprite->SetTexture(GetGame()->GetTexture(text));
}
