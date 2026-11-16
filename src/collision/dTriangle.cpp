#include "collision/dShape.h"
#include "collision/dTriangle.h"

namespace collision {
// 0x006F9030
collision::Triangle::Triangle() : Shape(KIND_TRIANGLE)
{
}

// 0x006F904C slot 0x00
// 0x006F9048 (deleting dtor)
collision::Triangle::~Triangle()
{
}

// 0x006F8F98 slot 0x08
void collision::Triangle::Translate(const nn::math::VEC3&)
{
}

} // namespace collision
