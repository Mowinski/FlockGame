#pragma once
#include "NavMesh.h"
#include "NavMeshItem.h"
#include <memory>

enum YellowBallState {
    IDLE,
    MOVE_TO,
    SCARED,
};

class YellowBallAI {
public:
    YellowBallAI(Renderable* _actor, std::shared_ptr<NavMeshItem> item);
    ~YellowBallAI();

    D3DXVECTOR3 OnUpdate(float deltaTime);

protected:
    void SelectNewGoal();
    YellowBallState state;
    std::shared_ptr<NavMesh> navMesh;
    std::shared_ptr<NavMeshItem> goal;
    NavMeshItemsVectorType path;
    Renderable* actor;
};

