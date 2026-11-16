#include "collision/dShape.h"
#include "collision/dCapsule.h"

namespace collision {
// ctor candidate(s) 0x006F8D78 (unverified)
collision::Capsule::Capsule() : Shape(KIND_CAPSULE)
{
}

// 0x006F8DA0 slot 0x00
// 0x006F8D9C (deleting dtor)
collision::Capsule::~Capsule()
{
}

// 0x006F8CD8 slot 0x08
void collision::Capsule::Translate(const nn::math::VEC3&)
{
}

} // namespace collision
