#include "YellowBallAI.h"
#include "Game.h"


YellowBallAI::YellowBallAI(Renderable* _actor, std::shared_ptr<NavMeshItem> item) :
    navMesh(Game::GetInstance()->navMesh),
    state(YellowBallState::IDLE),
    goal(item),
    actor(_actor)
{
}

YellowBallAI::~YellowBallAI()
{
}

D3DXVECTOR3 YellowBallAI::OnUpdate(float deltaTime)
{
    D3DXVECTOR3 force{ 0.0f, 0.0f, 0.0f };

    if (state == YellowBallState::IDLE) {
        SelectNewGoal();
        state = YellowBallState::MOVE_TO;
    }

    D3DXVECTOR3 diff = goal->GetPosition() - actor->GetPosition();

    if ( (diff.x*diff.x + diff.z*diff.z) < 0.1f) {
        state = YellowBallState::IDLE;
    }
    if (state == YellowBallState::MOVE_TO) {
        *D3DXVec3Normalize(&force, &diff);
    }
    force.y = 0;
    return force;
}

void YellowBallAI::SelectNewGoal()
{
    goal = goal->GetRandomNeighbor();
}
