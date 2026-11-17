#include "collision/dWorld.h"
#include "collision/dWorld_Body.h"

namespace collision {
// 0x006F8930 (name is ours)
collision::World::Hit::Hit()
{
    // (only the contacts are constructed)
}

// 0x006F8BE4 (name is ours)
collision::World::World()
{
    m_pCallback = NULL;
    Clear();
}

// 0x006F8C28 (name is ours)
collision::World::~World()
{
    // empty (in the original too)
}

// 0x006F89E0 (name is ours)
void collision::World::Add(Body* body)
{
    m_pBodies[m_Num] = body;
    m_Num++;
}

// 0x006F8BA4 (name is ours)
void collision::World::Clear()
{
    m_Num = 0;
    for (u32 i = 0; i < BODY_NUM; i++) {
        m_pBodies[i] = NULL;
    }
}

// 0x006F89FC (name is ours)
s32 collision::World::Test(Hit* hits, u32 size, Body* body)
{
    s32 num = 0;
    for (u32 i = 0; i < m_Num; i++) {
        Body* other = m_pBodies[i];
        if ((other->m_Group & body->m_Mask) == 0 || (body->m_Group & other->m_Mask) == 0) {
            continue;
        }
        Hit& hit = hits[num];
        const s32 contacts = Collide(hit.contacts, sizeof(hit.contacts), body->m_pShape, other->m_pShape);
        if (contacts > 0) {
            hit.num = contacts;
            num++;
            hit.pBody = other;
        }
        if (size <= num * sizeof(Hit)) {
            break;
        }
    }
    return num;
}

// 0x006F8AA4 (name is ours)
void collision::World::Update()
{
    if (m_pCallback != NULL) {
        for (s32 i = 0; i < static_cast<s32>(m_Num) - 1; i++) {
            Body* a = m_pBodies[i];
            for (u32 j = i + 1; j < m_Num; j++) {
                Body* b = m_pBodies[j];
                if ((b->m_Group & a->m_Mask) != 0 && (b->m_Mask & a->m_Group) != 0) {
                    m_pCallback(a, b);
                }
            }
        }
    }
    Clear();
}
} // namespace collision
