#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_AnimTransform.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt18AnimTransformBasicE @ 0x008D0850
// vtable 0x00902B4C (vptr 0x00902B54), offset_to_top 0, 8 entries
class AnimTransformBasic : public ::nw::lyt::AnimTransform
{
public:
    virtual ~AnimTransformBasic(); // 0x004B6B30 slot 0x00 | nintendogs:bytes-fuzzy
    virtual void vf_0x04(); // 0x004B6AF8 slot 0x04 | virtual slot, introduced by nw::lyt::AnimTransformBasic
    virtual void Animate(unsigned, nw::lyt::Pane*); // 0x004B6460 slot 0x08 | nintendogs:bytes
    virtual void Animate(unsigned, nw::lyt::Material*); // 0x004B673C slot 0x0C | nintendogs:bytes
    virtual void SetResource(const nw::lyt::res::AnimationBlock*, nw::lyt::ResourceAccessor*); // 0x004B606C slot 0x10 | slot vf_0x10 of nw::lyt::AnimTransformBasic
    virtual void vf_0x14(); // 0x004B607C slot 0x14 | nintendogs:callseq
    virtual void Bind(nw::lyt::Pane*, bool, bool); // 0x004B61E4 slot 0x18 | nintendogs:bytes
    virtual void vf_0x1C(); // 0x004B6374 slot 0x1C | virtual slot, introduced by nw::lyt::AnimTransformBasic
    AnimTransformBasic(); // 0x004B6AC0 | nintendogs:bytes [tier A]
};
} // namespace lyt
} // namespace nw
