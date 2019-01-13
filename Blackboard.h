#pragma once
#include <vector>
#include <d3dx9.h>

#include "YellowBall.h"
#include "NavMesh.h"
#include "NavMeshItem.h"


class Blackboard {
public:
    Blackboard(std::shared_ptr<NavMesh> _navMesh);
    ~Blackboard();

    YellowBallVectorType yellowBalls{};
    std::shared_ptr<NavMesh> navMesh;

    std::shared_ptr<NavMeshItem> getRandomNavMeshItem();
    std::shared_ptr<NavMeshItem> getRandomNavMeshItem(float minDistance, float x, float z);
    NavMeshItemsVectorType getPath(std::shared_ptr<NavMeshItem> start, std::shared_ptr<NavMeshItem> end);
};