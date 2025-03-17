#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder)
	: MoveComponent(owner, updateOrder), mNextNode(nullptr) {}

void NavComponent::Update(float deltaTime) {
	if (mNextNode) {
		//���� ������ �����ߴٸ� �̵��� �� ������ ��� �ش� ������ ���� ȸ��
		Vector2 diff = mOwner->GetPosition() - mNextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.f)) {
			mNextNode = mNextNode->GetParent();
			TurnTo(mNextNode->GetPosition());
		}
	}
	MoveComponent::Update(deltaTime); //���͸� ������Ŵ
}

void NavComponent::StartPath(const Tile* start) {
	mNextNode = start->GetParent();
	TurnTo(mNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos) {
	Vector2 dir = pos - mOwner->GetPosition(); //���Ϸ��� �������� ���͸� ����
	//�� ������ �� ���� ������ atan2(+y�� ȭ�� �Ʒ� �����̱� ������ y���� ������Ŵ)
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}
