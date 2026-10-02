#include "nw/ut/ut_Color8.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_TextBox.h"

namespace nw {
namespace lyt {
// ctor candidate(s) 0x004BC5C4 (unverified)
nw::lyt::TextBox::TextBox()
{
}

// 0x004BC824 slot 0x00 | libgarden
nw::lyt::TextBox::~TextBox()
{
}

// 0x0073E784 slot 0x08 | slot vf_0x08 of nw::lyt::Pane
void nw::lyt::TextBox::GetRuntimeTypeInfo() const
{
}

// 0x0073E5E0 slot 0x0C | nintendogs:bytes
void nw::lyt::TextBox::GetVtxColor(unsigned) const
{
}

// 0x004BBF98 slot 0x10 | slot vf_0x10 of nw::lyt::Pane
void nw::lyt::TextBox::SetVtxColor(unsigned, nw::ut::Color8)
{
}

// 0x0073E790 slot 0x1C | mk7dlp:bytes
void nw::lyt::TextBox::GetVtxColorElement(unsigned) const
{
}

// 0x004BC38C slot 0x20 | nintendogs:bytes
void nw::lyt::TextBox::SetVtxColorElement(unsigned, unsigned char)
{
}

// 0x0073E6B4 slot 0x24 | slot vf_0x24 of nw::lyt::Pane
void nw::lyt::TextBox::GetMaterialNum() const
{
}

// 0x0073E5D0 slot 0x28 | slot vf_0x28 of nw::lyt::Pane
void nw::lyt::TextBox::GetMaterial(unsigned int)
{
}

// 0x004BC508 slot 0x64 | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::TextBox::vf_0x64()
{
}

// 0x0073E7A8 slot 0x68 | libgarden
void nw::lyt::TextBox::MakeUniformDataSelf(nw::lyt::DrawInfo*, nw::lyt::Drawer*) const
{
}

// 0x004BC430 slot 0x6C | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::TextBox::vf_0x6C()
{
}

// 0x0013B9BC slot 0x70 | libgarden
void nw::lyt::TextBox::AllocStringBuffer(unsigned short, unsigned long)
{
}

// 0x004BC210 slot 0x74 | virtual slot, introduced by nw::lyt::TextBox
void nw::lyt::TextBox::vf_0x74()
{
}

// 0x004BBFAC slot 0x78 | virtual slot, introduced by nw::lyt::TextBox
void nw::lyt::TextBox::vf_0x78()
{
}

// 0x004BC5C0 slot 0x7C | libgarden
void nw::lyt::TextBox::SetString(char16_t const*, unsigned short, unsigned short)
{
}

// 0x004BC098 | nintendogs:callseq [tier A]
void nw::lyt::TextBox::SetupTextWriter(nw::font::TextWriterBase<wchar_t>*)
{
}

// 0x004BC2D0 | nintendogs:callseq [tier A]
void nw::lyt::TextBox::SetupDrawCharData(nw::lyt::Drawer*)
{
}

// 0x004BC5C4 | nintendogs:callseq-callee [tier A]
nw::lyt::TextBox::TextBox(const nw::lyt::res::TextBox*, const nw::lyt::ResBlockSet&)
{
}

// 0x0073E5F4 | nintendogs:bytes [tier A]
void nw::lyt::TextBox::AdjustTextPos(const nw::lyt::Size&, bool) const
{
}

// 0x0073E6C4 | nintendogs:bytes [tier A]
void nw::lyt::TextBox::GetTextGlobalMtx(nn::math::MTX34*) const
{
}

} // namespace lyt
} // namespace nw
