#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "nw/gfx/gfx_AnimObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx17BaseAnimEvaluatorE @ 0x008D0694
// vtable 0x009026F8 (vptr 0x00902700), offset_to_top 0, 13 entries
class BaseAnimEvaluator : public ::nw::gfx::AnimObject
{
public:
    BaseAnimEvaluator(); // ctor address unknown
    virtual ~BaseAnimEvaluator(); // 0x0049D324 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x0049D2A8 slot 0x04 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
    virtual void GetRuntimeTypeInfo() const; // 0x0073A864 slot 0x08 | slot vf_0x08 of nw::gfx::BaseAnimEvaluator
    virtual void TryBind(nw::gfx::AnimGroup*); // 0x0049CE90 slot 0x0C | nintendogs:callgraph
    virtual void Release(); // 0x0049CE84 slot 0x10 | slot vf_0x10 of nw::gfx::BaseAnimEvaluator
    virtual void UpdateFrame(); // 0x0049CE2C slot 0x14 | nintendogs:bytes
    virtual void GetResult(void*, int) const; // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void HasMemberAnim(int) const; // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void UpdateCache(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void ChangeAnim(nw::anim::res::ResAnim); // 0x0049CD9C slot 0x24 | slot vf_0x24 of nw::gfx::BaseAnimEvaluator
    virtual void vf_0x28(); // 0x0073A85C slot 0x28 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
    virtual void vf_0x2C(); // 0x0073A870 slot 0x2C | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
    virtual void vf_0x30(); // 0x0049CE80 slot 0x30 | virtual slot, introduced by nw::gfx::BaseAnimEvaluator
};
} // namespace gfx
} // namespace nw
