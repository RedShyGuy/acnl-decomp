#pragma once

#include "decomp.h"
#include "nw/lyt/internal/lyt_PaneBase.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt4PaneE @ 0x008D0868
// vtable 0x00902B98 (vptr 0x00902BA0), offset_to_top 0, 28 entries
class Pane : public ::nw::lyt::internal::PaneBase
{
public:
    Pane(); // ctor candidate(s) 0x004B7E78 (unverified)
    virtual ~Pane(); // 0x004B7FC4 slot 0x00 | nintendogs:bytes
    // 0x004B7FB0 slot 0x04 | slot vf_0x04 of nw::lyt::Pane (deleting dtor)
    virtual void GetRuntimeTypeInfo() const; // 0x0073CE08 slot 0x08 | slot vf_0x08 of nw::lyt::Pane
    virtual void GetVtxColor(unsigned) const; // 0x0073CD54 slot 0x0C | slot vf_0x0C of nw::lyt::Pane
    virtual void SetVtxColor(unsigned, nw::ut::Color8); // 0x004B7470 slot 0x10 | slot vf_0x10 of nw::lyt::Pane
    virtual void GetColorElement(unsigned) const; // 0x0073CD68 slot 0x14 | mk7dlp:bytes
    virtual void SetColorElement(unsigned, unsigned char); // 0x004B7904 slot 0x18 | nintendogs:bytes
    virtual void GetVtxColorElement(unsigned) const; // 0x0073CE14 slot 0x1C | slot vf_0x1C of nw::lyt::Pane
    virtual void SetVtxColorElement(unsigned, unsigned char); // 0x004B7BE8 slot 0x20 | slot vf_0x20 of nw::lyt::Pane
    virtual void GetMaterialNum() const; // 0x0073CD60 slot 0x24 | slot vf_0x24 of nw::lyt::Pane
    virtual void GetMaterial(unsigned int); // 0x0073CC58 slot 0x28 | slot vf_0x28 of nw::lyt::Pane
    virtual void FindPaneByName(const char*, bool); // 0x004B7890 slot 0x2C | nintendogs:bytes
    virtual void FindMaterialByName(const char*, bool); // 0x004B7994 slot 0x30 | nintendogs:bytes
    virtual void Animate(unsigned); // 0x004B7D84 slot 0x34 | nintendogs:bytes
    virtual void AnimateSelf(unsigned); // 0x004B7334 slot 0x38 | nintendogs:bytes
    virtual void BindAnimation(nw::lyt::AnimTransform*, bool, bool); // 0x004B7870 slot 0x3C | nintendogs:bytes
    virtual void UnbindAnimation(nw::lyt::AnimTransform*, bool); // 0x004B7920 slot 0x40 | nintendogs:bytes
    virtual void vf_0x44(); // 0x004B7BEC slot 0x44 | virtual slot, introduced by nw::lyt::Pane
    virtual void UnbindAnimationSelf(nw::lyt::AnimTransform*); // 0x004BEF64 slot 0x48 | nintendogs:bytes
    virtual void vf_0x4C(); // 0x004BEEC0 slot 0x4C | virtual slot, introduced by nw::lyt::Pane
    virtual void vf_0x50(); // 0x004BEF00 slot 0x50 | virtual slot, introduced by nw::lyt::Pane
    virtual void vf_0x54(); // 0x004B7A58 slot 0x54 | virtual slot, introduced by nw::lyt::Pane
    virtual void vf_0x58(); // 0x004B7B20 slot 0x58 | virtual slot, introduced by nw::lyt::Pane
    virtual void CalculateMtx(nw::lyt::DrawInfo const&); // 0x004B7474 slot 0x5C | libgarden
    virtual void vf_0x60(); // 0x004B7C00 slot 0x60 | virtual slot, introduced by nw::lyt::Pane
    virtual void vf_0x64(); // 0x004B7E74 slot 0x64 | virtual slot, introduced by nw::lyt::Pane
    virtual void MakeUniformDataSelf(nw::lyt::DrawInfo*, nw::lyt::Drawer*) const; // 0x0073CE1C slot 0x68 | slot vf_0x68 of nw::lyt::Pane
    virtual void vf_0x6C(); // 0x004B7DF0 slot 0x6C | virtual slot, introduced by nw::lyt::Pane
    void AppendChild(nw::lyt::Pane*); // 0x004B73FC | libgarden [tier A]
    void InsertChild(nw::lyt::Pane*, nw::lyt::Pane*); // 0x004B7420 | mk7dlp:bytes [tier B]
    void RemoveChild(nw::lyt::Pane*); // 0x004B7450 | libgarden [tier A]
    void AddAnimationLink(nw::lyt::AnimationLink*); // 0x004B7984 | nintendogs:callgraph [tier A]
    void Init(); // 0x004B7C78 | nintendogs:bytes-fuzzy [tier A]
    Pane(const nw::lyt::res::Pane*); // 0x004B7E78 | nintendogs:callseq-callee [tier A]
    void GetMaterial() const; // 0x0073CC60 | mk7dlp:bytes [tier B]
    void GetPaneRect() const; // 0x0073CC9C | nintendogs:callgraph [tier A]
    void GetVtxPos() const; // 0x0073CE24 | nintendogs:callgraph [tier A]
};
} // namespace lyt
} // namespace nw
