#include "collision/dWorld_Body.h"

// 0x006F89B4 (name is ours)
collision::World::Body::Body() : m_pShape(NULL), m_Unk8(0), m_UnkC(0)
{
}

// 0x006F899C (name is ours)
collision::World::Body::Body(Shape* shape, u32 unk8, u32 unkC) : m_pShape(shape), m_Unk8(unk8), m_UnkC(unkC)
{
}

// 0x006F89DC slot 0x00
// 0x006F89D4 (deleting dtor)
collision::World::Body::~Body()
{
    // empty (in the original too)
}
