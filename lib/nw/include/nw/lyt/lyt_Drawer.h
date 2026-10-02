#pragma once

#include "decomp.h"
#include "nw/font/font_RectDrawer.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt6DrawerE @ 0x008D087C
// vtable 0x00902C20 (vptr 0x00902C28), offset_to_top 0, 7 entries
class Drawer : public ::nw::font::RectDrawer
{
public:
    Drawer(); // ctor candidate(s) 0x004B9FE8 (unverified)
    virtual ~Drawer(); // 0x004D8734 slot 0x00 | slot vf_0x00 of nw::font::RectDrawer
    virtual void vf_0x04(); // 0x004BA000 slot 0x04 | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x08(); // 0x004BA244 slot 0x08 | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x0C(); // 0x004D8658 slot 0x0C | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x10(); // 0x004D8594 slot 0x10 | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x14(); // 0x004D8558 slot 0x14 | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x18(); // 0x004D84F4 slot 0x18 | virtual slot, introduced by nw::font::RectDrawer
    void Initialize(nw::lyt::GraphicsResource&, void*); // 0x004B8AEC | nintendogs:callseq [tier A]
    void SetUpTexEnv(const nw::lyt::Material*); // 0x004B8B84 | nintendogs:callseq [tier A]
    void SetUpTextBox(const nw::lyt::TextBox*, const nw::lyt::Material*, const nw::lyt::DrawInfo&); // 0x004B9000 | nintendogs:callseq [tier A]
    void SetUpTextures(const nw::lyt::Material*, bool); // 0x004B910C | nintendogs:callseq-callee [tier A]
    void SetUpBlendMode(const nw::lyt::Material*); // 0x004B92A0 | nintendogs:callseq [tier A]
    void SetUpVtxColors(const nw::ut::Color8*, unsigned char); // 0x004B93B0 | nintendogs:bytes [tier A]
    void UniformAndDraw(); // 0x004B9680 | nintendogs:callseq [tier A]
    void SetUpTexEnvType3(const nw::lyt::Material*); // 0x004B97E0 | nintendogs:callseq [tier A]
    void SetUpTexEnvType2(const nw::lyt::Material*); // 0x004B98F8 | nintendogs:callseq-callee [tier A]
    void SetUpGLTexEnvUser(const nw::lyt::Material*); // 0x004B9AB0 | nintendogs:callseq-callee [tier A]
    void SetUpTextureCoords(const nn::math::VEC4*, int); // 0x004B9E74 | nintendogs:callseq [tier A]
    void SetUpMtx(const nn::math::MTX34&); // 0x004B9F1C | nintendogs:bytes [tier A]
    void SetUpQuad(const nw::lyt::Size&, const nn::math::VEC2&); // 0x004B9FA8 | nintendogs:bytes [tier A]
    void CalcTextureCoords(const nw::lyt::Material*, const nn::math::VEC2(*)[4], nn::math::VEC4*) const; // 0x0073CEB0 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace lyt
} // namespace nw
