#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder)
	: MoveComponent(owner, updateOrder), mNextNode(nullptr) {}

void NavComponent::Update(float deltaTime) {
	if (mNextNode) {
		//다음 지점에 도착했다면 이동할 새 지점을 얻고 해당 지점을 향해 회전
		Vector2 diff = mOwner->GetPosition() - mNextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.f)) {
			mNextNode = mNextNode->GetParent();
			TurnTo(mNextNode->GetPosition());
		}
	}
	MoveComponent::Update(deltaTime); //액터를 전진시킴
}

void NavComponent::StartPath(const Tile* start) {
	mNextNode = start->GetParent();
	TurnTo(mNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos) {
	Vector2 dir = pos - mOwner->GetPosition(); //향하려는 점으로의 벡터를 구함
	//새 각도는 이 방향 벡터의 atan2(+y는 화면 아래 방향이기 때문에 y값을 반전시킴)
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}
