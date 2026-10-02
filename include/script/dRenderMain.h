#pragma once

#include "decomp.h"
#include "script/dRenderBase.h"

namespace script {
// RTTI N6script10RenderMainE @ 0x008D3738
// vtable 0x00909978 (vptr 0x00909980), offset_to_top 0, 4 entries
class RenderMain : public ::script::RenderBase
{
public:
    RenderMain(); // ctor candidate(s) 0x005D7338, 0x005D7B0C, 0x005E3C64 (unverified)
    virtual ~RenderMain(); // 0x005D7434 slot 0x00 | libgarden
    virtual void vf_0x04(); // 0x005D73D8 slot 0x04 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x08(); // 0x005D704C slot 0x08 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x0C(); // 0x005D71C8 slot 0x0C | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    RenderMain(unsigned int, unsigned int, unsigned int); // 0x005D7338 | libgarden [tier A]
};
} // namespace script
