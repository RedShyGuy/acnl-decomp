#include "collision/dTriangle.h"

namespace collision {
// 0x006F9030 (name is ours)
collision::Triangle::Triangle() : Shape(KIND_TRIANGLE)
{
}

// 0x006F904C slot 0x00
// 0x006F9048 (deleting dtor)
collision::Triangle::~Triangle()
{
    // empty (in the original too)
}

// 0x006F8F98 slot 0x08 (name is ours)
void collision::Triangle::Translate(const nn::math::VEC3& offset)
{
    m_Vertices[0].x += offset.x;
    m_Vertices[0].y += offset.y;
    m_Vertices[0].z += offset.z;
    m_Vertices[1].x += offset.x;
    m_Vertices[1].y += offset.y;
    m_Vertices[1].z += offset.z;
    m_Vertices[2].x += offset.x;
    m_Vertices[2].y += offset.y;
    m_Vertices[2].z += offset.z;
}
} // namespace collision
