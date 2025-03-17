#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"

Grid::Grid(class Game* game) : Actor(game), mSelectedTile(nullptr) {
	//7행 16열
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++) mTiles[i].resize(NumCols);

	//타일들 생성
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(Vector2(TileSize / 2.f + j * TileSize, StartY + i * TileSize));
		}
	}

	//시작/목표 타일 설정
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	//인접 리스트 설정
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			if (i > 0) mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
			if (i < NumRows - 1) mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
			if (j > 0) mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
			if (j < NumCols - 1) mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
		}
	}

	//경로 찾기(역으로)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	mNextEnemy = EnemyTime;
}

void Grid::SelectTile(size_t row, size_t col) {
	//옳바른 선택인지 확인
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase) {
		//이전 항목 선택 취소
		if (mSelectedTile) mSelectedTile->ToggleSelect();
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}

void Grid::ProcessClick(int x, int y) {
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0) {
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
			SelectTile(y, x);
	}
}

//A* 경로 탐색 구현
bool Grid::FindPath(Tile* start, Tile* goal) {
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			mTiles[i][j]->g = 0.f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;

	Tile* current = start;
	current->mInClosedSet = true;

	do {
		for (Tile* neighbor : current->mAdjacent) {
			if (neighbor->mBlocked) continue;

			if (!neighbor->mInClosedSet) {
				if (!neighbor->mInOpenSet) {
					neighbor->mParent = current;
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->mInOpenSet = true;
				}
				else {
					float newG = current->g + TileSize;
					if (newG < neighbor->g) {
						neighbor->mParent = current;
						neighbor->h = newG;
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		if (openSet.empty()) break;

		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[](Tile* a, Tile* b) { return a->f < b->f; }
		);
		current = *iter;
		openSet.erase(iter);
		current->mInOpenSet = false;
		current->mInClosedSet = true;
	} while (current != goal);

	return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(class Tile* start) {
	//모든 타일들 기본으로 재설정(시작/목표 제외)
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15))
				mTiles[i][j]->SetTileState(Tile::EDefault);
		}
	}
	Tile* t = start->mParent;
	while (t != GetEndTile()) {
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}

void Grid::BuildTower() {
	if (mSelectedTile && !mSelectedTile->mBlocked) {
		mSelectedTile->mBlocked = true;
		if (FindPath(GetEndTile(), GetStartTile())) {
			Tower* t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else {
			//이 타워는 경로를 차단하므로 지을 수 없음
			mSelectedTile->mBlocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

Tile* Grid::GetStartTile() { return mTiles[3][0]; }

Tile* Grid::GetEndTile() { return mTiles[3][15]; }

void Grid::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//새로운 적을 소환할 시간이라면
	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.f) {
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}
