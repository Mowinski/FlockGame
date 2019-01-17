#pragma once
#include "NavMeshItem.h"
#include <d3dx9math.h>
#include <memory>

namespace Utils {
	std::shared_ptr<NavMeshItem> getNearestNavMeshItem(const D3DXVECTOR3& position);

	/* vector is modified by truncate function! */
	void truncate(D3DXVECTOR3& vector, float maxLength);
}