#pragma once

#include "decomp.h"
#include "g3d/dFrameController.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "ssys/ma/dHeapAllocator.h"

namespace nw {
namespace gfx {
class AnimObject;
class BaseAnimEvaluator;
} // namespace gfx
} // namespace nw

namespace ssys {
namespace ma {
class Allocator;
} // namespace ma
} // namespace ssys

namespace g3d {
// an animation with its evaluator, built in a heap of its own (names are ours; the heap names "...Anim::build" give
// the name of Build)
// RTTI N3g3d8BaseAnimE @ 0x008D0D8C
// vtable 0x00903C04 (vptr 0x00903C0C), offset_to_top 0, 8 entries
class BaseAnim : public ::g3d::FrameController
{
public:
    // (inline)
    BaseAnim() : m_pAnimObject(NULL) {}
    virtual ~BaseAnim(); // 0x004F2DCC slot 0x00
    // 0x004F2DAC (deleting dtor)
    nw::gfx::BaseAnimEvaluator* GetEvaluator() override; // 0x004F2CB8 slot 0x08
    DECOMP_EMIT_COPY virtual nw::gfx::BaseAnimEvaluator* GetAnimObject() { return m_pAnimObject; } // 0x007474F8 slot 0x0C
    // another animation for the evaluator (bound again)
    virtual void ChangeAnim(nw::anim::res::ResAnim anim); // 0x004F2D2C slot 0x10
    // destroys the evaluator and the heap
    virtual void Finalize(); // 0x004F2CE0 slot 0x14
    // the evaluator in the heap (maxMemberAnims 0: all of the animation)
    virtual bool CreateAnimObject(nw::anim::res::ResAnim anim, s32 maxMemberAnims) = 0; // slot 0x18
    // binds the evaluator to the animation group of its target
    virtual bool Bind() = 0; // slot 0x1C

    // (inline; a copy for the game)
    DECOMP_EMIT_COPY ssys::ma::HeapAllocator* GetHeapAllocator() { return &m_HeapAllocator; } // 0x00746E84
    // the next frame
    void UpdateFrame(); // 0x004F2CA8
    // a member animation that does not animate its group member any more
    void UnbindMemberAnim(s32 animIdx); // 0x004F2CC0

    nw::gfx::BaseAnimEvaluator* m_pAnimObject;   // 0x08
    ssys::ma::HeapAllocator m_HeapAllocator;     // 0x0C the heap of the evaluator
};
ASSERT_SIZE(BaseAnim, 0x14);
} // namespace g3d
