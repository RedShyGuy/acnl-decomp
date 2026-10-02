#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt7PictureE @ 0x008D089C
// vtable 0x00902D18 (vptr 0x00902D20), offset_to_top 0, 30 entries
class Picture : public ::nw::lyt::Pane
{
public:
    Picture(); // ctor candidate(s) 0x004BBD88 (unverified)
    virtual ~Picture(); // 0x004BBF38 slot 0x00 | slot vf_0x00 of nw::lyt::Pane
    // 0x004BBECC slot 0x04 | slot vf_0x04 of nw::lyt::Pane (deleting dtor)
    virtual void GetRuntimeTypeInfo() const; // 0x0073E394 slot 0x08 | slot vf_0x08 of nw::lyt::Pane
    virtual void GetVtxColor(unsigned) const; // 0x0073E374 slot 0x0C | slot vf_0x0C of nw::lyt::Pane
    virtual void SetVtxColor(unsigned, nw::ut::Color8); // 0x004BBB70 slot 0x10 | slot vf_0x10 of nw::lyt::Pane
    virtual void GetVtxColorElement(unsigned) const; // 0x0073E3A0 slot 0x1C | slot vf_0x1C of nw::lyt::Pane
    virtual void SetVtxColorElement(unsigned, unsigned char); // 0x004BBB80 slot 0x20 | slot vf_0x20 of nw::lyt::Pane
    virtual void GetMaterialNum() const; // 0x0073E384 slot 0x24 | slot vf_0x24 of nw::lyt::Pane
    virtual void GetMaterial(unsigned int); // 0x0073E364 slot 0x28 | libgarden
    virtual void vf_0x64(); // 0x004BBD08 slot 0x64 | virtual slot, introduced by nw::lyt::Pane
    virtual void MakeUniformDataSelf(nw::lyt::DrawInfo*, nw::lyt::Drawer*) const; // 0x0073E3B8 slot 0x68 | nintendogs:callseq
    virtual void Append(nw::lyt::TexMap const&); // 0x004BBBC8 slot 0x70 | libgarden
    virtual void vf_0x74(); // 0x004BBB98 slot 0x74 | virtual slot, introduced by nw::lyt::Picture
    Picture(const nw::lyt::res::Picture*, const nw::lyt::ResBlockSet&); // 0x004BBD88 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace lyt
} // namespace nw
