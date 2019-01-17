#include "AABBCollisionBox.h"




AABBCollisionBox::AABBCollisionBox(const D3DXVECTOR3& minPoint, const D3DXVECTOR3& maxPoint) :
	vecMin(minPoint),
	vecMax(maxPoint)
{
}


bool AABBCollisionBox::isCollide(const D3DXVECTOR3& point) const
{
	if (!(point.x > vecMin.x && point.x < vecMax.x)) { return false; }
	if (!(point.y > vecMin.y && point.y < vecMax.y)) { return false; }
	if (!(point.z > vecMin.z && point.z < vecMax.z)) { return false; }

	return true;
}

bool AABBCollisionBox::isCollide(const AABBCollisionBox& box) const
{
	if (!(vecMax.x > box.vecMin.x && vecMin.x < box.vecMax.x)) { return false; }
	if (!(vecMax.y > box.vecMin.y && vecMin.y < box.vecMax.y)) { return false; }
	if (!(vecMax.z > box.vecMin.z && vecMin.z < box.vecMax.z)) { return false; }

    return true;
}
