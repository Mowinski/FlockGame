#include "RedBall.h"
#include "Game.h"
#include "CPR_Framework.h"


RedBall::RedBall(D3DXVECTOR3 _position, D3DXVECTOR3 _lookDir) : position{ _position }, lookDir{ _lookDir }
{
    energy = 100.0f;
}


RedBall::~RedBall()
{
}

void RedBall::OnRender()
{
    Render(Game::GetInstance()->loader->GetMesh("unitsphere"), position, rotation, scale, color);
}

void RedBall::OnUpdate(float deltaTime)
{
    if (energy <= 0.0f) { return; }
    position += lookDir * deltaTime;
    energy -= 1.0f * deltaTime;
}

bool RedBall::OnInit()
{
    return false;
}

D3DXVECTOR3 RedBall::GetPosition() const
{
    return position;
}
