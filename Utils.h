#pragma once
#include "d3dx9math.h" 
#include "NavMeshItem.h"

#include <memory>

std::shared_ptr<NavMeshItem> getNearestNavMeshItem(const D3DXVECTOR3& position);
float lerp(float start, float end, float percent);
void truncate(D3DXVECTOR3& vector, float maxLength);