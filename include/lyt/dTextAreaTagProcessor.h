#pragma once

#include "decomp.h"
#include "nw/font/font_TagProcessorBase.h"

namespace lyt {
// The tag processor of the text box of a TextArea: the character 12 followed by two characters sets the text color
// (see TextArea::WriteColorTag); tab and line feed as in the base (only the class name is from the RTTI).
// RTTI N3lyt20TextAreaTagProcessorE @ 0x008D0F58
// vtable 0x0090446C (vptr 0x00904474), offset_to_top 0, 4 entries
class TextAreaTagProcessor : public ::nw::font::TagProcessorBase<wchar_t>
{
public:
    TextAreaTagProcessor() {}
    virtual ~TextAreaTagProcessor(); // 0x00507E64 slot 0x00
    // 0x00507E60 slot 0x04 (deleting dtor)
    Operation Process(u16 code, nw::font::PrintContext<wchar_t>* pContext) override; // 0x00507D30 slot 0x08
    Operation CalcRect(nw::ut::Rect* pRect, u16 code, nw::font::PrintContext<wchar_t>* pContext) override; // 0x00507E38 slot 0x0C
};
} // namespace lyt
