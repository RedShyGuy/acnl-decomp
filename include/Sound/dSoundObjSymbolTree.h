#pragma once

#include "decomp.h"
#include "Sound/dSoundObjHold.h"

// RTTI 18SoundObjSymbolTree @ 0x008CC8F8
// vtable 0x008F4B4C (vptr 0x008F4B54), offset_to_top 0, 14 entries
// The tree in the center of the town: its sound (SE_ENV_SYMBOL_TREE_*) depends on the size of the tree (member names are ours).
class SoundObjSymbolTree : public ::SoundObjHold
{
public:
    enum
    {
        SIZE_NUM = 8,
        LEVEL_NUM = 7,
    };

    SoundObjSymbolTree(); // 0x002E9610
    // 0x002E9694
    // 0x002E9640 (deleting dtor)
    virtual ~SoundObjSymbolTree();
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002E93C8 slot 0x08
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002E93F0 slot 0x10

    void SetSize(u32 size, u32 level); // 0x002E9578

    static const u8 s_LevelNums[SIZE_NUM];
    static const s8 s_Sounds[SIZE_NUM][LEVEL_NUM];

    bool m_IsNear;    // 0x70 for a while near the tree
    u32 m_NearFrames; // 0x74
    s8 m_Sound;       // 0x78 -1: none
    u8 m_Size;        // 0x79
};
ASSERT_SIZE(SoundObjSymbolTree, 0x7C);
