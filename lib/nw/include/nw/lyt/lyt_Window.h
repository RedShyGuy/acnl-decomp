#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt6WindowE @ 0x008D0890
// vtable 0x00902C90 (vptr 0x00902C98), offset_to_top 0, 32 entries
class Window : public ::nw::lyt::Pane
{
public:
    Window(); // ctor candidate(s) 0x004BB690 (unverified)
    virtual ~Window(); // 0x004BB9AC slot 0x00 | slot vf_0x00 of nw::lyt::Pane
    // 0x004BB8D8 slot 0x04 | slot vf_0x04 of nw::lyt::Pane (deleting dtor)
    virtual void GetRuntimeTypeInfo() const; // 0x0073D4F8 slot 0x08 | slot vf_0x08 of nw::lyt::Pane
    virtual void GetVtxColor(unsigned) const; // 0x0073D394 slot 0x0C | slot vf_0x0C of nw::lyt::Pane
    virtual void SetVtxColor(unsigned, nw::ut::Color8); // 0x004BB22C slot 0x10 | slot vf_0x10 of nw::lyt::Pane
    virtual void GetVtxColorElement(unsigned) const; // 0x0073D504 slot 0x1C | mk7dlp:bytes
    virtual void SetVtxColorElement(unsigned, unsigned char); // 0x004BB314 slot 0x20 | nintendogs:bytes
    virtual void GetMaterialNum() const; // 0x0073D4E8 slot 0x24 | slot vf_0x24 of nw::lyt::Pane
    virtual void GetMaterial(unsigned int); // 0x0073D35C slot 0x28 | slot vf_0x28 of nw::lyt::Pane
    virtual void FindMaterialByName(const char*, bool); // 0x004BB23C slot 0x30 | slot vf_0x30 of nw::lyt::Pane
    virtual void vf_0x64(); // 0x004BB32C slot 0x64 | virtual slot, introduced by nw::lyt::Pane
    virtual void MakeUniformDataSelf(nw::lyt::DrawInfo*, nw::lyt::Drawer*) const; // 0x0073D51C slot 0x68 | slot vf_0x68 of nw::lyt::Pane
    virtual void vf_0x70(); // 0x004BB150 slot 0x70 | virtual slot, introduced by nw::lyt::Window
    virtual void vf_0x74(); // 0x004BB43C slot 0x74 | virtual slot, introduced by nw::lyt::Window
    virtual void vf_0x78(); // 0x004BAD98 slot 0x78 | virtual slot, introduced by nw::lyt::Window
    virtual void vf_0x7C(); // 0x004BAF8C slot 0x7C | virtual slot, introduced by nw::lyt::Window
    Window(const nw::lyt::res::Window*, const nw::lyt::ResBlockSet&); // 0x004BB690 | nintendogs:callseq [tier A]
};
} // namespace lyt
} // namespace nw
