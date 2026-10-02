#pragma once

#include "decomp.h"

namespace nw {
namespace font {
// RTTI N2nw4font10RectDrawerE @ 0x008D0BEC
// vtable 0x00903690 (vptr 0x00903698), offset_to_top 0, 7 entries
class RectDrawer
{
public:
    RectDrawer(); // ctor candidate(s) 0x004D8684 (unverified)
    virtual ~RectDrawer(); // 0x004D8738 slot 0x00 | mk7dlp:bytes
    virtual void vf_0x04(); // 0x004D8718 slot 0x04 | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x08(); // 0x004D8584 slot 0x08 | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x0C(); // 0x004D865C slot 0x0C | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x10(); // 0x004D85E4 slot 0x10 | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x14(); // 0x004D855C slot 0x14 | virtual slot, introduced by nw::font::RectDrawer
    virtual void vf_0x18(); // 0x004D8524 slot 0x18 | virtual slot, introduced by nw::font::RectDrawer
    void Initialize(void*, void*, const void*, unsigned); // 0x004D6F9C | nintendogs:bytes [tier A]
    void AddUniformMtx(unsigned long*); // 0x004D6FD0 | libgarden [tier A]
    void InitializeCMD(void*, void*, const void*, unsigned, bool); // 0x004D703C | nintendogs:callgraph [tier A]
    void BuildTextCommand(nw::font::CharWriter*); // 0x004D7890 | libgarden [tier A]
    void SetProjectionMtx(unsigned long*, nn::math::MTX44 const&); // 0x004D7FB0 | libgarden [tier A]
    void GetVertexBufferData(); // 0x004D8330 | mk7dlp:bytes [tier B]
    void GetDrawCommands() const; // 0x00744EB8 | nintendogs:callseq-callee [tier A]
    void GetVertexIndexAddressOffset(unsigned) const; // 0x00744EC4 | nintendogs:bytes [tier A]
};
} // namespace font
} // namespace nw
