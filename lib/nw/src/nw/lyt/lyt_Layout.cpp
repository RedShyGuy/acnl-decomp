#include "nw/lyt/lyt_Layout.h"

namespace nw {
namespace lyt {
// 0x004BAA68 slot 0x00 | nintendogs:bytes
nw::lyt::Layout::~Layout()
{
}

// 0x004BA794 slot 0x08 | nintendogs:bytes
void nw::lyt::Layout::Build(const void*, nw::lyt::ResourceAccessor*)
{
}

// 0x004BA678 slot 0x0C | nintendogs:bytes
void nw::lyt::Layout::CreateAnimTransform()
{
}

// 0x004BA5F0 slot 0x10 | nintendogs:bytes
void nw::lyt::Layout::CreateAnimTransform(const void*, nw::lyt::ResourceAccessor*)
{
}

// 0x004BA62C slot 0x14 | libgarden
void nw::lyt::Layout::CreateAnimTransform(nw::lyt::AnimResource const&, nw::lyt::ResourceAccessor*)
{
}

// 0x004BA6D0 slot 0x18 | virtual slot, introduced by nw::lyt::Layout
void nw::lyt::Layout::vf_0x18()
{
}

// 0x004BA200 slot 0x1C | nintendogs:bytes
void nw::lyt::Layout::BindAnimation(nw::lyt::AnimTransform*)
{
}

// 0x004BA224 slot 0x20 | nintendogs:bytes
void nw::lyt::Layout::UnbindAnimation(nw::lyt::AnimTransform*)
{
}

// 0x00137B18 slot 0x24 | slot vf_0x24 of nw::lyt::Layout
void nw::lyt::Layout::UnbindAllAnimation()
{
}

// 0x004BA29C slot 0x28 | virtual slot, introduced by nw::lyt::Layout
void nw::lyt::Layout::vf_0x28()
{
}

// 0x004BA5D0 slot 0x2C | virtual slot, introduced by nw::lyt::Layout
void nw::lyt::Layout::vf_0x2C()
{
}

// 0x004BAA38 slot 0x30 | virtual slot, introduced by nw::lyt::Layout
void nw::lyt::Layout::vf_0x30()
{
}

// 0x004BA1C0 slot 0x34 | slot vf_0x34 of nw::lyt::Layout
void nw::lyt::Layout::CalculateMtx(const nw::lyt::DrawInfo&)
{
}

// 0x004BA730 slot 0x38 | virtual slot, introduced by nw::lyt::Layout
void nw::lyt::Layout::vf_0x38()
{
}

// 0x004B7118 slot 0x3C | virtual slot, introduced by nw::lyt::Layout
void nw::lyt::Layout::vf_0x3C()
{
}

// 0x004BA034 slot 0x40 | nintendogs:callseq
void nw::lyt::Layout::BuildPaneObj(int, const void*, const nw::lyt::ResBlockSet&)
{
}

// 0x001322C8 | nintendogs:bytes [tier A]
nw::lyt::Layout::Layout()
{
}

// 0x0013BB1C | libgarden [tier A]
void nw::lyt::Layout::FreeMemory(void*)
{
}

// 0x004BA010 | libgarden [tier A]
void nw::lyt::Layout::AllocMemory(unsigned int, unsigned char)
{
}

// 0x004BA278 | nintendogs:bytes [tier A]
void nw::lyt::Layout::AllocDeviceMemory(unsigned, unsigned char)
{
}

} // namespace lyt
} // namespace nw
