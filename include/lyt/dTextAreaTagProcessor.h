#pragma once

#include "decomp.h"
#include "nw/font/font_TagProcessorBase.h"

namespace lyt {
// RTTI N3lyt20TextAreaTagProcessorE @ 0x008D0F58
// vtable 0x0090446C (vptr 0x00904474), offset_to_top 0, 4 entries
class TextAreaTagProcessor : public ::nw::font::TagProcessorBase<wchar_t>
{
public:
    TextAreaTagProcessor(); // ctor address unknown
    virtual ~TextAreaTagProcessor(); // 0x00507E64 slot 0x00 | slot vf_0x00 of nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x04(); // 0x00507E60 slot 0x04 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x08(); // 0x00507D30 slot 0x08 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x0C(); // 0x00507E38 slot 0x0C | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
};
} // namespace lyt
