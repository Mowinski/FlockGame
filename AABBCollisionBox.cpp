#include "AABBCollisionBox.h"




AABBCollisionBox::AABBCollisionBox(D3DXVECTOR3 p0, D3DXVECTOR3 p1) : vecMin(p0), vecMax(p1)
{
}

AABBCollisionBox::~AABBCollisionBox()
{

}

bool AABBCollisionBox::isCollide(const D3DXVECTOR3& point) const
{
    bool isXCollide = point.x > vecMin.x && point.x < vecMax.x;
    bool isYCollide = point.y > vecMin.y && point.y < vecMax.y;
    bool isZCollide = point.z > vecMin.z && point.z < vecMax.z;

    return isXCollide && isYCollide && isZCollide;
}

bool AABBCollisionBox::isCollide(const AABBCollisionBox& box) const
{
    bool isXCollide = vecMax.x > box.vecMin.x && vecMin.x < box.vecMax.x;
    bool isYCollide = vecMax.y > box.vecMin.y && vecMin.y < box.vecMax.y;
    bool isZCollide = vecMax.z > box.vecMin.z && vecMin.z < box.vecMax.z;

    return isXCollide && isYCollide && isZCollide;
}
