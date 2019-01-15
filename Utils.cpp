#include "Utils.h"
#include "Game.h"
#include "EngineTypes.h"

#include <algorithm>

std::shared_ptr<NavMeshItem> getNearestNavMeshItem(const D3DXVECTOR3 & position)
{
	NavMeshItemsVectorType navMeshItem = Game::GetInstance()->blackboard->navMesh->navMeshItems;

	auto cmpFn = [&position](std::shared_ptr<NavMeshItem> item1, std::shared_ptr<NavMeshItem> item2) {
		float d1 = D3DXVec3Length(&(item1->position - position));
		float d2 = D3DXVec3Length(&(item2->position - position));
		return d1 < d2;
	};
	auto navMeshItemIt = std::min_element(navMeshItem.begin(), navMeshItem.end(), cmpFn);

	return *navMeshItemIt;

}
