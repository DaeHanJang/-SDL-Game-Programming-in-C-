#pragma once

#include "Actor.h"
#include <vector>

class Grid : public Actor {
public:
	Grid(class Game* game);

	void ProcessClick(int x, int y); //x/y ȭ�� ��ġ���� ���콺 Ŭ�� �ڵ�
	bool FindPath(class Tile* start, class Tile* goal); //A* Ž�� ��� ���
	void BuildTower(); //Ÿ�� �Ǽ�

	//����/��ǥ Ÿ�� ���
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	void UpdateActor(float deltaTime) override;
private:
	void SelectTile(size_t row, size_t col); //Ư�� Ÿ�� ����
	void UpdatePathTiles(class Tile* start); //��ο� �ִ� Ÿ���� �ؽ�ó ������Ʈ

	class Tile* mSelectedTile; //���� ���õ� Ÿ��
	std::vector<std::vector<class Tile*>> mTiles; //���� �� Ÿ�ϵ��� 2���� ����
	float mNextEnemy; //���� �������� �ð�

	//���� ��/��
	const size_t NumRows = 7;
	const size_t NumCols = 16;
	const float StartY = 129.f; //�»�� �𼭸� ���� y ��ǥ
	const float TileSize = 64.f; //�� Ÿ���� �ʺ�/����
	const float EnemyTime = 1.5f; //���� ���̰� �ð�
};
