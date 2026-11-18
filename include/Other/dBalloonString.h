#pragma once

#include "decomp.h"
#include "Bs/dBsGrRenderer.h"
#include "Bs/dBsGrRenderer_Functor.h"
#include "math/dAngle16.h"
#include "nn/math/math_MTX34.h"

namespace sead {
class Heap;
} // namespace sead

// The string of a balloon in the hand of a human (class name from the RTTI; the members and the non-virtual functions
// are ours and only named where ToolBank shows them).
// RTTI 13BalloonString @ 0x008CB888
// vtable 0x008EEC50 (vptr 0x008EEC58), offset_to_top 0, 7 entries
class BalloonString : public ::BsGrRenderer::Functor
{
public:
    // a new string in heap (alignment 4)
    static BalloonString* Create(sead::Heap* heap); // 0x00213160

    virtual ~BalloonString(); // 0x00213808 slot 0x00
    // 0x002137C0 slot 0x04 (deleting dtor)
    virtual void vf_0x08(); // 0x00213358 slot 0x08 | virtual slot, introduced by BalloonString
    // the matrix of the balloon at the end of the string from the matrix of the hand (slot 0x10/0x14/0x18 by the state)
    virtual const nn::math::MTX34* vf_0x0C(const nn::math::MTX34* matrix); // 0x002134C4 slot 0x0C
    virtual void vf_0x10(); // 0x00213518 slot 0x10 | virtual slot, introduced by BalloonString
    virtual void vf_0x14(); // 0x002115FC slot 0x14 | virtual slot, introduced by BalloonString
    virtual void vf_0x18(); // 0x00212040 slot 0x18 | virtual slot, introduced by BalloonString

    // (true; adds the string to the renderer when it is shown)
    bool Func21348C(); // 0x0021348C
    // (inline; a copy for ToolBank) destroys the string in place (the memory is freed by the caller)
    DECOMP_EMIT_COPY void Destroy() { this->~BalloonString(); } // 0x0021350C
    // the string is released in direction (60 frames)
    void Release(math::Angle16 direction); // 0x00213790

    // (inline) the string is released
    bool IsReleased() const { return m_IsReleased; }

    u8 m_Unknown04[0x1664 - 0x04]; // 0x0004
    u16 m_ReleaseFrames;           // 0x1664
    math::Angle16 m_Direction;     // 0x1666
    u8 m_Color;                    // 0x1668 (0xFF: the color of the model)
    u8 m_Unknown1669;              // 0x1669
    bool m_IsReleased;             // 0x166A
};
ASSERT_OFFSET(BalloonString, m_ReleaseFrames, 0x1664);
