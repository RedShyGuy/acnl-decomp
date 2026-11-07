#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Types.h"
#include "ssys/ma/lyt/dFrameCtrl.h"
#include "ssys/ma/lyt/dResAccInterface.h"

namespace ssys {
namespace ma {
namespace lyt {
// an animation of a layout file (BCLAN) with its frame (names after libgarden except Finalize)
// RTTI N4ssys2ma3lyt9AnimationE @ 0x008D24A8
// vtable 0x00907334 (vptr 0x0090733C), offset_to_top 0, 3 entries
class Animation : public ::ssys::ma::lyt::FrameCtrl
{
public:
    DECOMP_EMIT_COPY DECOMP_NOINLINE Animation() // 0x00126200 | libgarden [tier A]
        : m_IsBound(0), m_EntryCount(0), m_pEntries(NULL), m_IsDescendingBind(false)
    {
    }
    virtual ~Animation(); // 0x0056A374 slot 0x00 | libgarden

    // the animation name of resAcc; false if there is none
    bool Initialize(const char* name, const ResAccInterface* resAcc); // 0x0056A0CC | libgarden [tier A]
    // whether a one-shot animation has reached its end
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsDone() const // 0x00750DEC | libgarden [tier A]
    {
        if (m_Loop == LOOP_FLAG_ONCE) {
            return m_Frame >= m_FrameCount - 1.0f;
        }
        if (m_Loop == (LOOP_FLAG_ONCE | LOOP_FLAG_REVERSE)) {
            return !(m_Frame > 0.0f);
        }
        return false;
    }
    void IncreaseFrame(); // 0x0056A02C | libgarden [tier A]
    // deletes the transform and the links (name is ours)
    void Finalize(); // 0x00569FB0

    bool IsBound() const { return m_IsBound == 1; }
    bool IsUnbound() const { return m_IsBound == 0; }
    bool Finish()
    {
        if (IsDone()) {
            return true;
        }
        IncreaseFrame();
        return false;
    }

    u8 m_IsBound;                        // 0x1C
    u16 m_EntryCount;                    // 0x1E
    nw::lyt::AnimationLink* m_pEntries;  // 0x20
    bool m_IsDescendingBind;             // 0x24
};
ASSERT_SIZE(Animation, 0x28);
} // namespace lyt
} // namespace ma
} // namespace ssys
