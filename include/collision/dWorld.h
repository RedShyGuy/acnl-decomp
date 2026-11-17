#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// A list of up to 100 bodies that are tested against each other (names are ours; the game has several worlds as
// globals of other files, e.g. 0x009880A8 of bgcheck). The bodies are added again in each frame: Update calls the
// callback for each pair of bodies whose groups and masks match and empties the list.
class World
{
public:
    class Body;

    // the bodies that touch a body (Test)
    struct Hit
    {
        Hit(); // 0x006F8930

        Body* pBody;          // 0x00
        s32 num;              // 0x04 the contacts
        Contact contacts[4];  // 0x08
    };

    // called by Update for a pair of bodies
    typedef void (*Callback)(Body* a, Body* b);

    static const u32 BODY_NUM = 100;

    World(); // 0x006F8BE4
    ~World(); // 0x006F8C28

    // (no test of the number)
    void Add(Body* body); // 0x006F89E0
    void Clear(); // 0x006F8BA4
    // the bodies that touch body into hits (size bytes): their number
    s32 Test(Hit* hits, u32 size, Body* body); // 0x006F89FC
    // the callback for each pair of bodies that may touch, then the list is emptied
    void Update(); // 0x006F8AA4

    Callback m_pCallback;         // 0x000
    u32 m_Num;                    // 0x004
    Body* m_pBodies[BODY_NUM];    // 0x008
};
ASSERT_SIZE(World, 0x198);
} // namespace collision
