#pragma once

#include "decomp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim10AnimResultE @ 0x008D0B78
// vtable 0x00903564 (vptr 0x0090356C), offset_to_top 0, 2 entries
// The value an animation evaluated for a member, and which of its components it set (bit i:
// component i). The value is big enough for a gfx::CalculatedTransform. Member names are ours.
class AnimResult
{
public:
    // (inline)
    AnimResult() {}
    virtual ~AnimResult(); // 0x004D517C slot 0x00 (name is ours)
    // 0x004D5178 (deleting dtor)

    // (inline)
    void* GetValueBuffer() { return m_ValueBuffer; }
    const void* GetValueBuffer() const { return m_ValueBuffer; }
    bool IsEnabledFlags(u32 flags) const { return (m_EnabledFlags & flags) != 0; }

    u32 m_EnabledFlags;      // 0x04
    u32 m_ValueBuffer[16];   // 0x08
};
ASSERT_SIZE(AnimResult, 0x48);
} // namespace anim
} // namespace nw
