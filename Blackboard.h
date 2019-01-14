#pragma once
#include <vector>
#include <d3dx9.h>

#include "YellowBall.h"
#include "RedBall.h"
#include "NavMesh.h"
#include "NavMeshItem.h"


class Blackboard {
public:
    Blackboard(std::shared_ptr<NavMesh> _navMesh);
    ~Blackboard();

    YellowBallVectorType yellowBalls{};
    RedBallVectorType redBalls{};
    std::shared_ptr<NavMesh> navMesh;

    std::shared_ptr<NavMeshItem> getRandomNavMeshItem();
    std::shared_ptr<NavMeshItem> getRandomNavMeshItem(float minDistance, float x, float z);
    std::shared_ptr<YellowBall> getNearestBall(const D3DXVECTOR3& position) const;
    NavMeshItemsVectorType getPath(std::shared_ptr<NavMeshItem> start, std::shared_ptr<NavMeshItem> end);
	std::shared_ptr<NavMeshItem> getNextStep(const D3DXVECTOR3& start, std::shared_ptr<NavMeshItem> end);
};
