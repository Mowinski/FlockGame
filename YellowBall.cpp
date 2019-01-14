#include "YellowBall.h"
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
    force = ai->OnUpdate(deltaTime) * 10.0f;
    D3DXVECTOR3 acceleration{ force / weight };
    speed = acceleration * deltaTime;
    D3DXVECTOR3 diffPosition{ speed * deltaTime };

    position += diffPosition;
}

bool YellowBall::OnInit()
{
    return true;
}

D3DXVECTOR3 YellowBall::GetPosition() const
{
    return position;
}

std::shared_ptr<NavMeshItem> YellowBall::GetCurrentNavMeshItem() const
{
	return ai->GetCurrentNavMeshItem();
}
