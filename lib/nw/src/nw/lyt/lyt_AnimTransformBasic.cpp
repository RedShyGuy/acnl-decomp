#include "nw/lyt/lyt_AnimTransform.h"
#include "nw/lyt/lyt_AnimTransformBasic.h"

namespace nw {
namespace lyt {
// 0x004B6B30 slot 0x00 | nintendogs:bytes-fuzzy
nw::lyt::AnimTransformBasic::~AnimTransformBasic()
{
}

// 0x004B6AF8 slot 0x04 | virtual slot, introduced by nw::lyt::AnimTransformBasic
void nw::lyt::AnimTransformBasic::vf_0x04()
{
}

// 0x004B6460 slot 0x08 | nintendogs:bytes
void nw::lyt::AnimTransformBasic::Animate(unsigned, nw::lyt::Pane*)
{
}

// 0x004B673C slot 0x0C | nintendogs:bytes
void nw::lyt::AnimTransformBasic::Animate(unsigned, nw::lyt::Material*)
{
}

// 0x004B606C slot 0x10 | slot vf_0x10 of nw::lyt::AnimTransformBasic
void nw::lyt::AnimTransformBasic::SetResource(const nw::lyt::res::AnimationBlock*, nw::lyt::ResourceAccessor*)
{
}

// 0x004B607C slot 0x14 | nintendogs:callseq
void nw::lyt::AnimTransformBasic::vf_0x14()
{
}

// 0x004B61E4 slot 0x18 | nintendogs:bytes
void nw::lyt::AnimTransformBasic::Bind(nw::lyt::Pane*, bool, bool)
{
}

// 0x004B6374 slot 0x1C | virtual slot, introduced by nw::lyt::AnimTransformBasic
void nw::lyt::AnimTransformBasic::vf_0x1C()
{
}

// 0x004B6AC0 | nintendogs:bytes [tier A]
nw::lyt::AnimTransformBasic::AnimTransformBasic()
{
}

} // namespace lyt
} // namespace nw
