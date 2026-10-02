#pragma once

#include "decomp.h"
#include "nw/font/font_TagProcessorBase.h"
#include "script/dChip.h"

// RTTI N6script4Chip16MeasureProcessorE @ 0x008D3A64
// vtable 0x0090A56C (vptr 0x0090A574), offset_to_top 0, 4 entries
class script::Chip::MeasureProcessor : public ::nw::font::TagProcessorBase<wchar_t>
{
public:
    MeasureProcessor(); // ctor candidate(s) 0x005EAB78 (unverified)
    virtual ~MeasureProcessor(); // 0x005EAAA8 slot 0x00 | slot vf_0x00 of nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x04(); // 0x005EAAA4 slot 0x04 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x0C(); // 0x005EA9F0 slot 0x0C | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
};
