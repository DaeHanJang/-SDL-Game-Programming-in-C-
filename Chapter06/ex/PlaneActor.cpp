#include "PlaneActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* game) : Actor(game) {
	SetScale(10.f);
	MeshComponent* mc = new MeshComponent(this, 1);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh"));
}
