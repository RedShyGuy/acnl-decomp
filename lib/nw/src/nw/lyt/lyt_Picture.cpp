#include "nw/ut/ut_Color8.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_Picture.h"

namespace nw {
namespace lyt {
// ctor candidate(s) 0x004BBD88 (unverified)
nw::lyt::Picture::Picture()
{
}

// 0x004BBF38 slot 0x00 | slot vf_0x00 of nw::lyt::Pane
nw::lyt::Picture::~Picture()
{
}

// 0x0073E394 slot 0x08 | slot vf_0x08 of nw::lyt::Pane
void nw::lyt::Picture::GetRuntimeTypeInfo() const
{
}

// 0x0073E374 slot 0x0C | slot vf_0x0C of nw::lyt::Pane
void nw::lyt::Picture::GetVtxColor(unsigned) const
{
}

// 0x004BBB70 slot 0x10 | slot vf_0x10 of nw::lyt::Pane
void nw::lyt::Picture::SetVtxColor(unsigned, nw::ut::Color8)
{
}

// 0x0073E3A0 slot 0x1C | slot vf_0x1C of nw::lyt::Pane
void nw::lyt::Picture::GetVtxColorElement(unsigned) const
{
}

// 0x004BBB80 slot 0x20 | slot vf_0x20 of nw::lyt::Pane
void nw::lyt::Picture::SetVtxColorElement(unsigned, unsigned char)
{
}

// 0x0073E384 slot 0x24 | slot vf_0x24 of nw::lyt::Pane
void nw::lyt::Picture::GetMaterialNum() const
{
}

// 0x0073E364 slot 0x28 | libgarden
void nw::lyt::Picture::GetMaterial(unsigned int)
{
}

// 0x004BBD08 slot 0x64 | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Picture::vf_0x64()
{
}

// 0x0073E3B8 slot 0x68 | nintendogs:callseq
void nw::lyt::Picture::MakeUniformDataSelf(nw::lyt::DrawInfo*, nw::lyt::Drawer*) const
{
}

// 0x004BBBC8 slot 0x70 | libgarden
void nw::lyt::Picture::Append(nw::lyt::TexMap const&)
{
}

// 0x004BBB98 slot 0x74 | virtual slot, introduced by nw::lyt::Picture
void nw::lyt::Picture::vf_0x74()
{
}

// 0x004BBD88 | nintendogs:bytes-fuzzy [tier A]
nw::lyt::Picture::Picture(const nw::lyt::res::Picture*, const nw::lyt::ResBlockSet&)
{
}

} // namespace lyt
} // namespace nw
