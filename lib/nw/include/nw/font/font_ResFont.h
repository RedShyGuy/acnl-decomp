#pragma once

#include "decomp.h"
#include "nw/font/font_ResFontBase.h"

namespace nw {
namespace font {
// RTTI N2nw4font7ResFontE @ 0x008D0C18
// vtable 0x009037C8 (vptr 0x009037D0), offset_to_top 0, 27 entries
class ResFont : public ::nw::font::ResFontBase
{
public:
    ResFont(); // ctor candidate(s) 0x004D95D8 (unverified)
    virtual void vf_0x00(); // 0x001386B0 slot 0x00 | virtual slot, introduced by nw::font::Font
    virtual void vf_0x04(); // 0x004D95F0 slot 0x04 | virtual slot, introduced by nw::font::Font
    void SetDrawBuffer(void*); // 0x0013C52C | nintendogs:bytes [tier A]
    void GetDrawBufferSize(const void*); // 0x004D9540 | nintendogs:bytes [tier A]
};
} // namespace font
} // namespace nw
