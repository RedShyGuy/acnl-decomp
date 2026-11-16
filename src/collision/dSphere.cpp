#include "collision/dShape.h"
#include "collision/dSphere.h"

namespace collision {
// ctor candidate(s) 0x006F8C84, 0x006F8CAC (unverified)
collision::Sphere::Sphere() : Shape(KIND_SPHERE)
{
}

// 0x006F8CD4 slot 0x00
// 0x006F8CD0 (deleting dtor)
collision::Sphere::~Sphere()
{
}

// 0x006F8C2C slot 0x08
void collision::Sphere::Translate(const nn::math::VEC3&)
{
}

} // namespace collision
