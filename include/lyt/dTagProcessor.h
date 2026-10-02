#pragma once

#include "decomp.h"
#include "nw/font/font_TagProcessorBase.h"

namespace lyt {
// RTTI N3lyt12TagProcessorE @ 0x008D0F10
// vtable 0x009042B4 (vptr 0x009042BC), offset_to_top 0, 6 entries
class TagProcessor : public ::nw::font::TagProcessorBase<wchar_t>
{
public:
    TagProcessor(); // ctor address unknown
    virtual ~TagProcessor(); // 0x00501EF4 slot 0x00 | slot vf_0x00 of nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x04(); // 0x00501EE4 slot 0x04 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x08(); // 0x00501D9C slot 0x08 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x0C(); // 0x00501E8C slot 0x0C | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x10(); // 0x00501C94 slot 0x10 | virtual slot, introduced by lyt::TagProcessor
    virtual void vf_0x14(); // 0x005019FC slot 0x14 | virtual slot, introduced by lyt::TagProcessor
};
} // namespace lyt
