#include "Blackboard.h"

#include <algorithm>
#include <map>
#include <d3dx9.h>
#include <limits>

Blackboard::Blackboard( std::shared_ptr<NavMesh> _navMesh) : navMesh(_navMesh)
{
}


Blackboard::~Blackboard()
{
}

std::shared_ptr<NavMeshItem> Blackboard::getRandomNavMeshItem()
{
    return navMesh->getRandom();
}

std::shared_ptr<NavMeshItem> Blackboard::getRandomNavMeshItem(float minDistance, float x, float z)
{
    NavMeshItemsVectorType items{};
    auto cmpFunc = [&minDistance, &x, &z](std::shared_ptr<NavMeshItem> item) { return item->CalculateDist(x, z) > minDistance; };
    std::copy_if(navMesh->navMeshItems.begin(), navMesh->navMeshItems.end(), std::back_inserter(items), cmpFunc);

    auto randIt = items.begin();
    std::advance(randIt, std::rand() % items.size());
    return *randIt;
}

std::shared_ptr<YellowBall> Blackboard::getNearestBall(const D3DXVECTOR3& position) const
{
    std::shared_ptr<YellowBall> ret;
	float minLength = (std::numeric_limits<float>::max)();
	for (auto ball : yellowBalls) {
		float diff = D3DXVec3Length(&(ball->GetPosition() - position));
		if (diff < minLength) {
			minLength = diff;
			ret = ball;
		}
	}
	return ret;
}

NavMeshItemsVectorType Blackboard::getPath(std::shared_ptr<NavMeshItem> start, std::shared_ptr<NavMeshItem> end)
{
    NavMeshItemsVectorType path{};
    const float INF = (std::numeric_limits<float>::max)();
    NavMeshItemsVectorType closedSet{};
    NavMeshItemsVectorType openSet{};
    std::map< std::shared_ptr<NavMeshItem>, std::shared_ptr<NavMeshItem> > cameFrom;
    std::map<std::shared_ptr<NavMeshItem>, float> gScore;
    std::map<std::shared_ptr<NavMeshItem>, float> fScore;

    auto heuristic = [&end](std::shared_ptr<NavMeshItem> item) -> float { return D3DXVec3Length(&(end->position - item->position)); };
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
            float tentativeScore = gScore[current] + neighbor->CalculateDist(current->position.x, current->position.z);

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

std::shared_ptr<NavMeshItem> Blackboard::getNextStep(const D3DXVECTOR3 & start, std::shared_ptr<NavMeshItem> end)
{
	std::shared_ptr<NavMeshItem> startNavMesh;
	float diff = (std::numeric_limits<float>::max)();
	for (auto item : navMesh->navMeshItems) {
		float length = D3DXVec3Length(&(item->GetPosition() - start));
		if (length < diff) {
			diff = length;
			startNavMesh = item;
		}
	}

	return getPath(startNavMesh, end)[0];
}
