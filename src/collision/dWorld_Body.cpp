#include "collision/dWorld_Body.h"

// 0x006F89B4 (name is ours)
collision::World::Body::Body() : m_pShape(NULL), m_Group(0), m_Mask(0)
{
}

// 0x006F899C (name is ours)
collision::World::Body::Body(Shape* shape, u32 group, u32 mask) : m_pShape(shape), m_Group(group), m_Mask(mask)
{
}

// 0x006F89DC slot 0x00
// 0x006F89D4 (deleting dtor)
collision::World::Body::~Body()
{
    // empty (in the original too)
}

// 0x006F8990 (name is ours)
void collision::World::Body::Set(Shape* shape, u32 group, u32 mask)
{
    m_pShape = shape;
    m_Group = group;
    m_Mask = mask;
}

// 0x006F8954 (name is ours)
void collision::World::Body::SetMask(u32 mask)
{
    m_Mask = mask;
}
