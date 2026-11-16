#include "collision/dShape.h"
#include "collision/dAABB.h"

namespace collision {
// 0x006F8910
collision::AABB::AABB() : Shape(KIND_AABB)
{
}

// 0x006F892C slot 0x00
// 0x006F8928 (deleting dtor)
collision::AABB::~AABB()
{
}

// 0x006F87AC slot 0x08
void collision::AABB::Translate(const nn::math::VEC3&)
{
}

} // namespace collision
