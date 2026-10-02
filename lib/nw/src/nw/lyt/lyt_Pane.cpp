#include "nw/ut/ut_Color8.h"
#include "nw/lyt/internal/lyt_PaneBase.h"
#include "nw/lyt/lyt_Pane.h"

namespace nw {
namespace lyt {
// ctor candidate(s) 0x004B7E78 (unverified)
nw::lyt::Pane::Pane()
{
}

// 0x004B7FC4 slot 0x00 | nintendogs:bytes
nw::lyt::Pane::~Pane()
{
}

// 0x0073CE08 slot 0x08 | slot vf_0x08 of nw::lyt::Pane
void nw::lyt::Pane::GetRuntimeTypeInfo() const
{
}

// 0x0073CD54 slot 0x0C | slot vf_0x0C of nw::lyt::Pane
void nw::lyt::Pane::GetVtxColor(unsigned) const
{
}

// 0x004B7470 slot 0x10 | slot vf_0x10 of nw::lyt::Pane
void nw::lyt::Pane::SetVtxColor(unsigned, nw::ut::Color8)
{
}

// 0x0073CD68 slot 0x14 | mk7dlp:bytes
void nw::lyt::Pane::GetColorElement(unsigned) const
{
}

// 0x004B7904 slot 0x18 | nintendogs:bytes
void nw::lyt::Pane::SetColorElement(unsigned, unsigned char)
{
}

// 0x0073CE14 slot 0x1C | slot vf_0x1C of nw::lyt::Pane
void nw::lyt::Pane::GetVtxColorElement(unsigned) const
{
}

// 0x004B7BE8 slot 0x20 | slot vf_0x20 of nw::lyt::Pane
void nw::lyt::Pane::SetVtxColorElement(unsigned, unsigned char)
{
}

// 0x0073CD60 slot 0x24 | slot vf_0x24 of nw::lyt::Pane
void nw::lyt::Pane::GetMaterialNum() const
{
}

// 0x0073CC58 slot 0x28 | slot vf_0x28 of nw::lyt::Pane
void nw::lyt::Pane::GetMaterial(unsigned int)
{
}

// 0x004B7890 slot 0x2C | nintendogs:bytes
void nw::lyt::Pane::FindPaneByName(const char*, bool)
{
}

// 0x004B7994 slot 0x30 | nintendogs:bytes
void nw::lyt::Pane::FindMaterialByName(const char*, bool)
{
}

// 0x004B7D84 slot 0x34 | nintendogs:bytes
void nw::lyt::Pane::Animate(unsigned)
{
}

// 0x004B7334 slot 0x38 | nintendogs:bytes
void nw::lyt::Pane::AnimateSelf(unsigned)
{
}

// 0x004B7870 slot 0x3C | nintendogs:bytes
void nw::lyt::Pane::BindAnimation(nw::lyt::AnimTransform*, bool, bool)
{
}

// 0x004B7920 slot 0x40 | nintendogs:bytes
void nw::lyt::Pane::UnbindAnimation(nw::lyt::AnimTransform*, bool)
{
}

// 0x004B7BEC slot 0x44 | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Pane::vf_0x44()
{
}

// 0x004BEF64 slot 0x48 | nintendogs:bytes
void nw::lyt::Pane::UnbindAnimationSelf(nw::lyt::AnimTransform*)
{
}

// 0x004BEEC0 slot 0x4C | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Pane::vf_0x4C()
{
}

// 0x004BEF00 slot 0x50 | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Pane::vf_0x50()
{
}

// 0x004B7A58 slot 0x54 | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Pane::vf_0x54()
{
}

// 0x004B7B20 slot 0x58 | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Pane::vf_0x58()
{
}

// 0x004B7474 slot 0x5C | libgarden
void nw::lyt::Pane::CalculateMtx(nw::lyt::DrawInfo const&)
{
}

// 0x004B7C00 slot 0x60 | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Pane::vf_0x60()
{
}

// 0x004B7E74 slot 0x64 | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Pane::vf_0x64()
{
}

// 0x0073CE1C slot 0x68 | slot vf_0x68 of nw::lyt::Pane
void nw::lyt::Pane::MakeUniformDataSelf(nw::lyt::DrawInfo*, nw::lyt::Drawer*) const
{
}

// 0x004B7DF0 slot 0x6C | virtual slot, introduced by nw::lyt::Pane
void nw::lyt::Pane::vf_0x6C()
{
}

// 0x004B73FC | libgarden [tier A]
void nw::lyt::Pane::AppendChild(nw::lyt::Pane*)
{
}

// 0x004B7420 | mk7dlp:bytes [tier B]
void nw::lyt::Pane::InsertChild(nw::lyt::Pane*, nw::lyt::Pane*)
{
}

// 0x004B7450 | libgarden [tier A]
void nw::lyt::Pane::RemoveChild(nw::lyt::Pane*)
{
}

// 0x004B7984 | nintendogs:callgraph [tier A]
void nw::lyt::Pane::AddAnimationLink(nw::lyt::AnimationLink*)
{
}

// 0x004B7C78 | nintendogs:bytes-fuzzy [tier A]
void nw::lyt::Pane::Init()
{
}

// 0x004B7E78 | nintendogs:callseq-callee [tier A]
nw::lyt::Pane::Pane(const nw::lyt::res::Pane*)
{
}

// 0x0073CC60 | mk7dlp:bytes [tier B]
void nw::lyt::Pane::GetMaterial() const
{
}

// 0x0073CC9C | nintendogs:callgraph [tier A]
void nw::lyt::Pane::GetPaneRect() const
{
}

// 0x0073CE24 | nintendogs:callgraph [tier A]
void nw::lyt::Pane::GetVtxPos() const
{
}

} // namespace lyt
} // namespace nw
