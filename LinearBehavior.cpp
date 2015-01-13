#include "LinearBehavior.h"
#include "Tank.h"

LinearBehavior::LinearBehavior(Tank* tank) : Behavior(tank)
{

}

void LinearBehavior::Update(float dt)
{
	using namespace DirectX::SimpleMath;
	Vector2 direction = -(Vector2(tank->GetPosition().x,tank->GetPosition().y) - tank->GetWaypoints()->front());
	direction.Normalize();

	tank->SetDirection(Vector3(direction.x,direction.y,tank->GetDirection().z));
}