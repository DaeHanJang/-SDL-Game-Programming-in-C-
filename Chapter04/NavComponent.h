#pragma once

#include "MoveComponent.h"
#include "Math.h"

class NavComponent : public MoveComponent {
public:
	//먼저 업데이트되도록 갱신 순서값을 낮춤
	NavComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	void StartPath(const class Tile* start);
	void TurnTo(const Vector2& pos);
private:
	const class Tile* mNextNode;
};
