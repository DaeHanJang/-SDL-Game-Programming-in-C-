#pragma once

#include "Actor.h"
#include <vector>

class Grid : public Actor {
public:
	Grid(class Game* game);

	void ProcessClick(int x, int y); //x/y 화면 위치에서 마우스 클릭 핸들
	bool FindPath(class Tile* start, class Tile* goal); //A* 탐색 경로 사용
	void BuildTower(); //타워 건설

	//시작/목표 타일 얻기
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	void UpdateActor(float deltaTime) override;
private:
	void SelectTile(size_t row, size_t col); //특정 타일 선택
	void UpdatePathTiles(class Tile* start); //경로에 있는 타일의 텍스처 업데이트

	class Tile* mSelectedTile; //현재 선택된 타일
	std::vector<std::vector<class Tile*>> mTiles; //격자 안 타일들의 2차원 벡터
	float mNextEnemy; //다음 적까지의 시간

	//격자 행/열
	const size_t NumRows = 7;
	const size_t NumCols = 16;
	const float StartY = 129.f; //좌상단 모서리 시작 y 좌표
	const float TileSize = 64.f; //각 타일의 너비/높이
	const float EnemyTime = 1.5f; //적들 사이간 시간
};
