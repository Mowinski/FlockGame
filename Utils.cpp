#include "Utils.h"
#include "EngineTypes.h"
#include "Game.h"
#include <algorithm>

namespace Utils {
	std::shared_ptr<NavMeshItem> getNearestNavMeshItem(const D3DXVECTOR3 & position)
	{
		NavMeshItemsVectorType navMeshItem = Game::GetInstance()->navMesh->navMeshItems;

		auto cmpFn = [&position](std::shared_ptr<NavMeshItem> item1, std::shared_ptr<NavMeshItem> item2) {
			float d1 = D3DXVec3Length(&(item1->GetPosition() - position));
			float d2 = D3DXVec3Length(&(item2->GetPosition() - position));
			return d1 < d2;
		};
		auto navMeshItemIt = std::min_element(navMeshItem.begin(), navMeshItem.end(), cmpFn);

		return *navMeshItemIt;

	}

	float lerp(float start, float end, float percent)
	{
		return (start + percent * (end - start));
	}

	void truncate(D3DXVECTOR3 & vector, float maxLength)
	{
		float length = D3DXVec3Length(&vector);
		if (length > maxLength) {
			D3DXVec3Normalize(&vector, &vector);
			vector *= maxLength;
		}
	}
}