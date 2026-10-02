#pragma once

#include "decomp.h"
#include "nw/font/font_TagProcessorBase.h"

namespace script {
// RTTI N6script10RenderBaseE @ 0x008D372C
// vtable 0x00909960 (vptr 0x00909968), offset_to_top 0, 4 entries
class RenderBase : public ::nw::font::TagProcessorBase<wchar_t>
{
public:
    RenderBase(); // ctor candidate(s) 0x005D6C38 (unverified)
    virtual ~RenderBase(); // 0x005D6D30 slot 0x00 | slot vf_0x00 of nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x04(); // 0x005D6CF0 slot 0x04 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
};
} // namespace script
