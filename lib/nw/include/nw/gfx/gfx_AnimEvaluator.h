#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "nw/gfx/gfx_BaseAnimEvaluator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13AnimEvaluatorE @ 0x008D05BC
// vtable 0x00902458 (vptr 0x00902460), offset_to_top 0, 13 entries
class AnimEvaluator : public ::nw::gfx::BaseAnimEvaluator
{
public:
    AnimEvaluator(); // ctor candidate(s) 0x00492770 (unverified)
    virtual ~AnimEvaluator(); // 0x00492918 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x00492840 slot 0x04 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
    virtual void GetRuntimeTypeInfo() const; // 0x00738704 slot 0x08 | slot vf_0x08 of nw::gfx::BaseAnimEvaluator
    virtual void GetResult(void*, int) const; // 0x00738710 slot 0x18 | nintendogs:bytes
    virtual void HasMemberAnim(int) const; // 0x007386D8 slot 0x1C | nintendogs:bytes
    virtual void UpdateCache(); // 0x00492508 slot 0x20 | slot vf_0x20 of nw::gfx::BaseAnimEvaluator
    virtual void ChangeAnim(nw::anim::res::ResAnim); // 0x00491E88 slot 0x24 | slot vf_0x24 of nw::gfx::BaseAnimEvaluator
    virtual void vf_0x28(); // 0x007386FC slot 0x28 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
    virtual void vf_0x2C(); // 0x004926E0 slot 0x2C | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
    virtual void vf_0x30(); // 0x004924E8 slot 0x30 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
    void UpdateCacheNonVirtual(); // 0x0049250C | nintendogs:bytes [tier A]
    void SetCacheBufferPointers(); // 0x00492654 | nintendogs:bytes [tier A]
    void GetCacheBufferSizeNeeded(const nw::anim::res::ResAnim&); // 0x004926E8 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
