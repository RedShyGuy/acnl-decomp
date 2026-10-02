#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_AnimGroup.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004B3784 (unverified)
nw::gfx::AnimGroup::AnimGroup()
{
}

// 0x004B38EC slot 0x00 | nintendogs:bytes
nw::gfx::AnimGroup::~AnimGroup()
{
}

// 0x004B3808 slot 0x04 | virtual slot, introduced by nw::gfx::AnimGroup
void nw::gfx::AnimGroup::vf_0x04()
{
}

// 0x004B2900 | nintendogs:callgraph [tier A]
void nw::gfx::AnimGroup::Initialize(bool)
{
}

// 0x004B36E0 | nintendogs:bytes [tier A]
void nw::gfx::AnimGroup::GetMemorySizeForInitialize(nw::os::MemorySizeCalculator*, nw::anim::res::ResAnimGroup, bool)
{
}

// 0x004B3784 | nintendogs:callgraph [tier A]
nw::gfx::AnimGroup::AnimGroup(nw::anim::res::ResAnimGroup, nw::gfx::SceneNode*, nw::os::IAllocator*)
{
}

} // namespace gfx
} // namespace nw
