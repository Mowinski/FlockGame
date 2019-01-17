#include "Blackboard.h"
#include "Game.h"
#include "Utils.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>


std::shared_ptr<NavMeshItem> Blackboard::getRandomNavMeshItem(float minDistance, float x, float z) const
{
    NavMeshItemsVectorType items{};
    auto cmpFunc = [&minDistance, &x, &z](std::shared_ptr<NavMeshItem> item) { return item->CalculateDistance(x, z) > minDistance; };
    std::copy_if(Game::getInstance()->navMesh->navMeshItems.begin(), Game::getInstance()->navMesh->navMeshItems.end(), std::back_inserter(items), cmpFunc);

    auto randIt = items.begin();
    std::advance(randIt, std::rand() % items.size());
    return *randIt;
}

std::shared_ptr<YellowBall> Blackboard::getNearestBall(const D3DXVECTOR3 &position) const
{
    std::shared_ptr<YellowBall> ret{ nullptr };
    float minLength = (std::numeric_limits<float>::max)();
    for (auto ball : yellowBalls) {
        float diff = D3DXVec3Length(&(ball->getPosition() - position));
        if (diff < minLength) {
            minLength = diff;
            ret = ball;
        }
    }
    return ret;
}

NavMeshItemsVectorType Blackboard::getPath(const std::shared_ptr<NavMeshItem>& start, const std::shared_ptr<NavMeshItem>& end)
{
    NavMeshItemsVectorType path{};
    const float INF = (std::numeric_limits<float>::max)();
    NavMeshItemsVectorType closedSet{};
    NavMeshItemsVectorType openSet{};
    std::map<std::shared_ptr<NavMeshItem>, std::shared_ptr<NavMeshItem>> cameFrom;
    std::map<std::shared_ptr<NavMeshItem>, float> gScore;
    std::map<std::shared_ptr<NavMeshItem>, float> fScore;

    auto heuristic = [&end](std::shared_ptr<NavMeshItem> item) -> float { return D3DXVec3Length(&(end->GetPosition() - item->GetPosition())); };
    auto lowestFScore = [&fScore, INF](std::shared_ptr<NavMeshItem> item1, std::shared_ptr<NavMeshItem> item2) -> int {
        float f1 = fScore.find(item1) == fScore.end() ? INF : fScore[item1];
        float f2 = fScore.find(item2) == fScore.end() ? INF : fScore[item2];
        return f1 < f2;
    };

    gScore[start] = 0;
    fScore[start] = heuristic(start);
    openSet.push_back(start);

    while (!openSet.empty()) {
        auto currentIt = std::min_element(openSet.begin(), openSet.end(), lowestFScore);
        std::shared_ptr<NavMeshItem> current = *currentIt;
        if (current->id == end->id) {
            path.insert(path.begin(), current);
            while (cameFrom.find(current) != cameFrom.end()) {
                current = cameFrom[current];
                if(current->id != start->id) {
                    path.insert(path.begin(), current);
                }
            }
            break;
        }

        closedSet.push_back(current);
        openSet.erase(currentIt);

        for (auto neighbor : current->neighbors) {
            bool isInClosedSet = std::find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end();
            if (isInClosedSet) { continue; }

            if (gScore.find(current) == gScore.end()) { gScore[current] = INF; }
            float tentativeScore = gScore[current] + neighbor->CalculateDistance(current->GetPosition().x, current->GetPosition().z);

            bool isInOpenSet = std::find(openSet.begin(), openSet.end(), neighbor) != openSet.end();

            if (gScore.find(neighbor) == gScore.end()) { gScore[neighbor] = INF; }
            if (!isInOpenSet) {
                openSet.push_back(neighbor);
            } else if (tentativeScore >= gScore[neighbor]) {
                continue;
            }

            cameFrom[neighbor] = current;
            gScore[neighbor] = tentativeScore;
            fScore[neighbor] = tentativeScore + heuristic(neighbor);
        }
    }
    return path;
}

void Blackboard::nominateLeaders()
{
	const size_t ballsCount = yellowBalls.size();
	const short unsigned int leadersCount = static_cast<short unsigned int>(std::ceil(ballsCount / 5.0f));
    std::random_shuffle(yellowBalls.begin(), yellowBalls.end());
	std::for_each(yellowBalls.begin(), yellowBalls.begin() + leadersCount, [this](auto ball) { ball->setLeader(); yellowBallsLeaders.push_back(ball); });
}

void Blackboard::cleanUpAfterHit(const std::shared_ptr<YellowBall> &ball)
{
	if (ball->isBallLeader()) {
		unsetTargetLeader(ball);
		rearrangeLeaders(ball);
	}
	destroyYellowBall(ball);
	checkWiningCondition();
}

void Blackboard::clearDeadRedBalls()
{
	auto redBallIt = std::find_if(redBalls.begin(), redBalls.end(), [](auto item) {return item->isDead(); });

	if (redBallIt != redBalls.end()) {
		createNewYellowBall((*redBallIt)->getPosition());
		redBalls.erase(redBallIt);
	}
}

void Blackboard::clearLeaders()
{
	std::for_each(yellowBallsLeaders.begin(), yellowBallsLeaders.end(), [](auto ball) { ball->unsetLeader(); });
	yellowBallsLeaders.clear();
}

void Blackboard::createNewYellowBall(const D3DXVECTOR3 &position)
{
	std::shared_ptr<NavMeshItem> navMeshItem = Utils::getNearestNavMeshItem(position);
	auto yellowBall = std::make_shared<YellowBall>(navMeshItem, 2.0f);
	yellowBalls.push_back(yellowBall);
}

void Blackboard::rearrangeLeaders(const std::shared_ptr<YellowBall> & ball)
{
	auto it = std::find(yellowBallsLeaders.begin(), yellowBallsLeaders.end(), ball);
	if (it != yellowBallsLeaders.end()) {
		clearLeaders();
		nominateLeaders();
	}
}

void Blackboard::unsetTargetLeader(const std::shared_ptr<YellowBall> &ball)
{
	for (auto b : yellowBalls) {
		if (b->checkTargetLeader(ball)) {
			b->unsetTargetLeader();
		}
	}
}

void Blackboard::destroyYellowBall(const std::shared_ptr<YellowBall> & ball)
{
	auto it = std::find(yellowBalls.begin(), yellowBalls.end(), ball);
	if (it != yellowBalls.end()) {
		yellowBalls.erase(it);
	}
}

void Blackboard::checkWiningCondition()
{
	if (yellowBalls.size() == 0) {
		Game::getInstance()->timer->freezeRoundTime();
	}
}
