#pragma once
#include <memory>
#include <d3dx9.h>

#include "NavMeshItem.h"

std::shared_ptr<NavMeshItem> getNearestNavMeshItem(const D3DXVECTOR3& position);