#include "YellowBall.h"
#include "YellowBallAI.h"
#include "Game.h"


YellowBall::YellowBall(std::shared_ptr<NavMeshItem> item) : scale{ ballSize, ballSize, ballSize }
{
    position = item->position;
    position.y = 2.0f;
    ai = std::make_shared<YellowBallAI>(this, item);
}


YellowBall::~YellowBall()
{
}

void YellowBall::OnRender()
{
    Render(Game::GetInstance()->loader->GetMesh("unitsphere"), position, rotation, scale, color);
}

void YellowBall::OnUpdate(float deltaTime)
{
	speed = ai->OnUpdate(deltaTime);
	position += speed * deltaTime ;
}

bool YellowBall::OnInit()
{
    return true;
}

D3DXVECTOR3 YellowBall::GetPosition() const
{
    return position;
}

D3DXVECTOR3 YellowBall::GetSpeed() const
{
	return speed;
}

std::shared_ptr<YellowBall> YellowBall::seeAnyLeader() const
{
	std::shared_ptr<YellowBall> ret = nullptr;

	if (D3DXVec3Length(&speed) == 0.0f) return ret;
	float shortestDistance = visibilityDistance;

	for (auto leader : Game::GetInstance()->blackboard->yellowBallsLeaders) {
		D3DXVECTOR3 diffVec = leader->GetPosition() - GetPosition();
		float distance = D3DXVec3Length(&diffVec);
		if (distance < shortestDistance) {
			ret = leader;
			shortestDistance = distance;
		}
	}
	return ret;
}

std::shared_ptr<NavMeshItem> YellowBall::GetCurrentNavMeshItem() const
{
	return ai->GetCurrentNavMeshItem();
}

void YellowBall::SetLeader()
{
	isLeader = true;
	color = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
}

void YellowBall::UnsetLeader()
{
	isLeader = false;
	color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
}
