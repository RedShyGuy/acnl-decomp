#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt7TextBoxE @ 0x008D08A8
// vtable 0x00902D98 (vptr 0x00902DA0), offset_to_top 0, 32 entries
class TextBox : public ::nw::lyt::Pane
{
public:
    TextBox(); // ctor candidate(s) 0x004BC5C4 (unverified)
    virtual ~TextBox(); // 0x004BC824 slot 0x00 | libgarden
    // 0x004BC774 slot 0x04 | slot vf_0x04 of nw::lyt::Pane (deleting dtor)
    virtual void GetRuntimeTypeInfo() const; // 0x0073E784 slot 0x08 | slot vf_0x08 of nw::lyt::Pane
    virtual void GetVtxColor(unsigned) const; // 0x0073E5E0 slot 0x0C | nintendogs:bytes
    virtual void SetVtxColor(unsigned, nw::ut::Color8); // 0x004BBF98 slot 0x10 | slot vf_0x10 of nw::lyt::Pane
    virtual void GetVtxColorElement(unsigned) const; // 0x0073E790 slot 0x1C | mk7dlp:bytes
    virtual void SetVtxColorElement(unsigned, unsigned char); // 0x004BC38C slot 0x20 | nintendogs:bytes
    virtual void GetMaterialNum() const; // 0x0073E6B4 slot 0x24 | slot vf_0x24 of nw::lyt::Pane
    virtual void GetMaterial(unsigned int); // 0x0073E5D0 slot 0x28 | slot vf_0x28 of nw::lyt::Pane
    virtual void vf_0x64(); // 0x004BC508 slot 0x64 | virtual slot, introduced by nw::lyt::Pane
    virtual void MakeUniformDataSelf(nw::lyt::DrawInfo*, nw::lyt::Drawer*) const; // 0x0073E7A8 slot 0x68 | libgarden
    virtual void vf_0x6C(); // 0x004BC430 slot 0x6C | virtual slot, introduced by nw::lyt::Pane
    virtual void AllocStringBuffer(unsigned short, unsigned long); // 0x0013B9BC slot 0x70 | libgarden
    virtual void vf_0x74(); // 0x004BC210 slot 0x74 | virtual slot, introduced by nw::lyt::TextBox
    virtual void vf_0x78(); // 0x004BBFAC slot 0x78 | virtual slot, introduced by nw::lyt::TextBox
    virtual void SetString(char16_t const*, unsigned short, unsigned short); // 0x004BC5C0 slot 0x7C | libgarden
    void SetupTextWriter(nw::font::TextWriterBase<wchar_t>*); // 0x004BC098 | nintendogs:callseq [tier A]
    void SetupDrawCharData(nw::lyt::Drawer*); // 0x004BC2D0 | nintendogs:callseq [tier A]
    TextBox(const nw::lyt::res::TextBox*, const nw::lyt::ResBlockSet&); // 0x004BC5C4 | nintendogs:callseq-callee [tier A]
    void AdjustTextPos(const nw::lyt::Size&, bool) const; // 0x0073E5F4 | nintendogs:bytes [tier A]
    void GetTextGlobalMtx(nn::math::MTX34*) const; // 0x0073E6C4 | nintendogs:bytes [tier A]
};
} // namespace lyt
} // namespace nw
