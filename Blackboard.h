#pragma once
#include "EngineTypes.h"  
#include "NavMesh.h"
#include "RedBall.h"
#include "YellowBall.h"

#include <d3dx9math.h>    
#include <memory>         


struct Blackboard {
    Blackboard() = default;
    ~Blackboard() = default;

    std::shared_ptr<NavMeshItem> getRandomNavMeshItem(float minDistance, float x, float z) const;
    std::shared_ptr<YellowBall> getNearestBall(const D3DXVECTOR3 &position) const;
    NavMeshItemsVectorType getPath(const std::shared_ptr<NavMeshItem>& start, const std::shared_ptr<NavMeshItem>& end);

    void nominateLeaders();
    void cleanUpAfterHit(const std::shared_ptr<YellowBall> &ball);
    void clearDeadRedBalls();

    YellowBallVectorType yellowBalls{};
    YellowBallVectorType yellowBallsLeaders{};
    RedBallVectorType redBalls{};

	const float extraEnergyAfterHiting{ 0.3f };
	const float maxRedBallForce{ 7.0f };
	const float startRedBallEnergy{ 5.0f };
	const float maxRedBallSpeed{ 15.0f };

	const float maxYellowBallSpeed{ 1.0f };
	const float scaredMaxYellowBallSpeed{ 10.0f };
	const float dangerousRadius{ 5.0f };

	const float mouseSensitivity{ 10.0f };
	const float maxMovePlayerSpeed{ 5.0f };

	const int startingBallsCount{ 17 };

	bool isPlayerReloading{ false };

protected:
	void clearLeaders();
	void createNewYellowBall(const D3DXVECTOR3 &position);
	void rearrangeLeaders(const std::shared_ptr<YellowBall> & ball);
	void unsetTargetLeader(const std::shared_ptr<YellowBall> &ball);
	void destroyYellowBall(const std::shared_ptr<YellowBall> & ball);
	void checkWiningCondition();

};
