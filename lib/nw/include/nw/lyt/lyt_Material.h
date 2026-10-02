#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt8MaterialE @ 0x008D08C8
// vtable 0x00902EA8 (vptr 0x00902EB0), offset_to_top 0, 11 entries
class Material
{
public:
    Material(); // ctor candidate(s) 0x004BE1C8 (unverified)
    virtual ~Material(); // 0x004BE8A8 slot 0x00 | libgarden
    virtual void vf_0x04(); // 0x004BE864 slot 0x04 | virtual slot, introduced by nw::lyt::Material
    virtual void vf_0x08(); // 0x004BCC18 slot 0x08 | virtual slot, introduced by nw::lyt::Material
    virtual void UnbindAnimation(nw::lyt::AnimTransform*); // 0x004BDFDC slot 0x0C | slot vf_0x0C of nw::lyt::Material
    virtual void vf_0x10(); // 0x004BE104 slot 0x10 | virtual slot, introduced by nw::lyt::Material
    virtual void Animate(); // 0x004BE14C slot 0x14 | nintendogs:bytes
    virtual void vf_0x18(); // 0x004BDFF4 slot 0x18 | virtual slot, introduced by nw::lyt::Material
    virtual void vf_0x1C(); // 0x004BDFFC slot 0x1C | virtual slot, introduced by nw::lyt::Material
    virtual void vf_0x20(); // 0x004BE0BC slot 0x20 | virtual slot, introduced by nw::lyt::Material
    virtual void vf_0x24(); // 0x004BE0E0 slot 0x24 | virtual slot, introduced by nw::lyt::Material
    virtual void vf_0x28(); // 0x004BCC34 slot 0x28 | virtual slot, introduced by nw::lyt::Material
    void ReserveMem(unsigned char, unsigned char, unsigned char, unsigned char, bool, bool); // 0x004BC900 | libgarden [tier A]
    void GetTexSRTAry(); // 0x004BCBA8 | libgarden [tier A]
    void SetTexMapNum(unsigned char); // 0x004BCBBC | nintendogs:bytes [tier A]
    void SetTevStageNum(unsigned char); // 0x004BDE54 | nintendogs:bytes [tier A]
    void SetColorElement(unsigned, unsigned char); // 0x004BDF9C | nintendogs:bytes [tier A]
    void AddAnimationLink(nw::lyt::AnimationLink*); // 0x004BDFE4 | nintendogs:callgraph [tier A]
    void GetTexCoordGenAry(); // 0x004BE004 | nintendogs:callseq-callee [tier A]
    void SetTexCoordGenNum(unsigned char); // 0x004BE02C | nintendogs:bytes [tier A]
    void Init(); // 0x004BE114 | nintendogs:bytes [tier A]
    Material(const nw::lyt::res::Material*, const nw::lyt::ResBlockSet&); // 0x004BE1C8 | nintendogs:bytes-fuzzy [tier A]
    void GetTevStageAry() const; // 0x0073E874 | nintendogs:bytes [tier A]
    void GetBlendModePtr() const; // 0x0073E8C0 | nintendogs:bytes [tier A]
    void GetAlphaComparePtr() const; // 0x0073E928 | nintendogs:bytes [tier A]
};
} // namespace lyt
} // namespace nw
