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

    D3DXVECTOR3 diff = path[0]->GetPosition() - actor->GetPosition();
    float distance = diff.x*diff.x + diff.z*diff.z;

    if (distance < 0.1f && path.size() > 0) {
        path.erase(path.begin());
    }
    if (distance < 0.1f && path.size() == 0) {
        state = YellowBallState::IDLE;
    }
    if (state == YellowBallState::MOVE_TO) {
        diff.y = 0.0f;
        D3DXVec3Normalize(&force, &diff);
    }
    return force;
}

std::shared_ptr<NavMeshItem> YellowBallAI::GetCurrentNavMeshItem() const
{
	return path[0];
}

void YellowBallAI::SelectNewGoal()
{
    std::shared_ptr<NavMeshItem> newGoal = Game::GetInstance()->blackboard->getRandomNavMeshItem(20.0f, goal->position.x, goal->position.z);
    path = Game::GetInstance()->blackboard->getPath(goal, newGoal);
    goal = newGoal;
}
