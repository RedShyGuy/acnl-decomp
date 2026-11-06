#pragma once

#include "decomp.h"
#include "Sound/dSoundObjSymbolTree.h"

// The sound of the tree in the center of the town for the game, like the other AudioObj classes but without
// virtual functions and without RTTI (the class and all names are ours). Nothing in the program refers to it
// directly.
class AudioObjSymbolTree
{
public:
    AudioObjSymbolTree(); // 0x002D60A8
    void Start(const nn::math::VEC3* world); // 0x002D6004
    s8 IsNear(); // 0x002D603C
    void SetIsIgnoreMaster(bool isIgnoreMaster); // 0x002D6044
    void SetPosition(const nn::math::VEC3* world); // 0x002D6048
    void Stop(); // 0x002D6078

    SoundObjSymbolTree m_Obj; // 0x00
    bool m_IsStarted;         // 0x7C
};
ASSERT_SIZE(AudioObjSymbolTree, 0x80);
