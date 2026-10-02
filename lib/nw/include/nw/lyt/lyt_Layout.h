#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt6LayoutE @ 0x008D0888
// vtable 0x00902C44 (vptr 0x00902C4C), offset_to_top 0, 17 entries
class Layout
{
public:
    virtual ~Layout(); // 0x004BAA68 slot 0x00 | nintendogs:bytes
    // 0x004BAA54 slot 0x04 | slot vf_0x04 of nw::lyt::Layout (deleting dtor)
    virtual void Build(const void*, nw::lyt::ResourceAccessor*); // 0x004BA794 slot 0x08 | nintendogs:bytes
    virtual void CreateAnimTransform(); // 0x004BA678 slot 0x0C | nintendogs:bytes
    virtual void CreateAnimTransform(const void*, nw::lyt::ResourceAccessor*); // 0x004BA5F0 slot 0x10 | nintendogs:bytes
    virtual void CreateAnimTransform(nw::lyt::AnimResource const&, nw::lyt::ResourceAccessor*); // 0x004BA62C slot 0x14 | libgarden
    virtual void vf_0x18(); // 0x004BA6D0 slot 0x18 | virtual slot, introduced by nw::lyt::Layout
    virtual void BindAnimation(nw::lyt::AnimTransform*); // 0x004BA200 slot 0x1C | nintendogs:bytes
    virtual void UnbindAnimation(nw::lyt::AnimTransform*); // 0x004BA224 slot 0x20 | nintendogs:bytes
    virtual void UnbindAllAnimation(); // 0x00137B18 slot 0x24 | slot vf_0x24 of nw::lyt::Layout
    virtual void vf_0x28(); // 0x004BA29C slot 0x28 | virtual slot, introduced by nw::lyt::Layout
    virtual void vf_0x2C(); // 0x004BA5D0 slot 0x2C | virtual slot, introduced by nw::lyt::Layout
    virtual void vf_0x30(); // 0x004BAA38 slot 0x30 | virtual slot, introduced by nw::lyt::Layout
    virtual void CalculateMtx(const nw::lyt::DrawInfo&); // 0x004BA1C0 slot 0x34 | slot vf_0x34 of nw::lyt::Layout
    virtual void vf_0x38(); // 0x004BA730 slot 0x38 | virtual slot, introduced by nw::lyt::Layout
    virtual void vf_0x3C(); // 0x004B7118 slot 0x3C | virtual slot, introduced by nw::lyt::Layout
    virtual void BuildPaneObj(int, const void*, const nw::lyt::ResBlockSet&); // 0x004BA034 slot 0x40 | nintendogs:callseq
    Layout(); // 0x001322C8 | nintendogs:bytes [tier A]
    void FreeMemory(void*); // 0x0013BB1C | libgarden [tier A]
    void AllocMemory(unsigned int, unsigned char); // 0x004BA010 | libgarden [tier A]
    void AllocDeviceMemory(unsigned, unsigned char); // 0x004BA278 | nintendogs:bytes [tier A]
};
} // namespace lyt
} // namespace nw
