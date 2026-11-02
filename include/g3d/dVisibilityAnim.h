#pragma once

#include "decomp.h"
#include "g3d/dBaseAnim.h"

namespace g3d {
class Model;

// a visibility animation of a model (names are ours)
// RTTI N3g3d14VisibilityAnimE @ 0x008D0CE4
// vtable 0x00903A44 (vptr 0x00903A4C), offset_to_top 0, 8 entries
class VisibilityAnim : public ::g3d::BaseAnim
{
public:
    VisibilityAnim(); // 0x004EEEF0
    virtual ~VisibilityAnim(); // 0x004EEF48 slot 0x00
    // 0x004EEF28 (deleting dtor)
    void Finalize() override; // 0x004EEEA0 slot 0x14
    // (the number of member animations is always 64)
    bool CreateAnimObject(nw::anim::res::ResAnim anim, s32 maxMemberAnims) override; // 0x004EEE00 slot 0x18
    bool Bind() override; // 0x004EECDC slot 0x1C

    // the evaluator of the animation in a heap of all that the heap of allocator can give, bound to the visibility
    // group of the model (heap name "VisibilityAnim::build"); false if it could not be made
    bool Build(Model* model, nw::anim::res::ResAnim anim, ssys::ma::Allocator* allocator); // 0x004EED10

    Model* m_pModel; // 0x14
};
ASSERT_SIZE(VisibilityAnim, 0x18);
} // namespace g3d
