#pragma once

#include "decomp.h"
#include "g3d/dTransformNode.h"

namespace g3d {
// a light of the scene (no vtable of its own in the binary: only built as a base; names are ours)
// RTTI N3g3d9BaseLightE @ 0x008D0DA4
class BaseLight : public ::g3d::TransformNode
{
public:
    // (inline)
    BaseLight() : m_Index(-1) {}

    s32 m_Index; // 0x8 (-1: none; only the lights of the game use it)
};
ASSERT_SIZE(BaseLight, 0xC);
} // namespace g3d
