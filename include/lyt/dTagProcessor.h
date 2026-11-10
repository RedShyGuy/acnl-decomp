#pragma once

#include "decomp.h"
#include "nw/font/font_TagProcessorBase.h"
#include "nw/ut/ut_Color8.h"

namespace lyt {
// The tag processor of the texts of the layouts: besides tab and line feed it handles the tags of the message texts
// (0x0E group index size parameters, 0x0F group index for the end of a tag): the scale (group 0 index 2: the width in
// percent, 100 restores it) and the color (group 0 index 3: RGBA; black restores it) of the predefined group, and the
// scale tags of group 1 (index 0 both, 1 the width, 2 the height in percent; the end tag restores them). Only the class
// name is from the binary (RTTI); member and function names are ours.
// RTTI N3lyt12TagProcessorE @ 0x008D0F10
// vtable 0x009042B4 (vptr 0x009042BC), offset_to_top 0, 6 entries
class TagProcessor : public ::nw::font::TagProcessorBase<wchar_t>
{
public:
    typedef nw::font::PrintContext<wchar_t> PrintContext;

    // (inline)
    TagProcessor() : m_SavedScaleH(1.0f) {}
    virtual ~TagProcessor(); // 0x00501EF4 slot 0x00
    // 0x00501EE4 slot 0x04 (deleting dtor)
    Operation Process(u16 code, PrintContext* pContext) override; // 0x00501D9C slot 0x08
    Operation CalcRect(nw::ut::Rect* pRect, u16 code, PrintContext* pContext) override; // 0x00501E8C slot 0x0C
    // a tag (isBegin) or the end of one is applied to the writer
    virtual void ProcessTag(PrintContext* pContext, bool isBegin); // 0x00501C94 slot 0x10
    // the same for the rectangle of the text (only the scale of group 1 changes the writer)
    virtual void CalcRectTag(nw::ut::Rect* pRect, PrintContext* pContext, bool isBegin); // 0x005019FC slot 0x14

private:
    // the scale tags of group 1 (the scale before the tag is kept in a static of each function)
    DECOMP_NOINLINE void ProcessScaleTag(PrintContext* pContext, bool scaleH, bool scaleV, bool isBegin); // 0x00501C08
    DECOMP_NOINLINE void CalcRectScaleTag(nw::ut::Rect* pRect, PrintContext* pContext, u16 index, bool isBegin); // 0x00501A64
    // the color tag of group 0
    DECOMP_NOINLINE void ProcessColorTag(PrintContext* pContext); // 0x00501B20

public:
    nw::ut::Color8 m_SavedColor;  // 0x4 the text color before the color tag
    f32 m_SavedScaleH;            // 0x8 the width scale before the scale tag of group 0
};
ASSERT_SIZE(TagProcessor, 0xC);
} // namespace lyt
