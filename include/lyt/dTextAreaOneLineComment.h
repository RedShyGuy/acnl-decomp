#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"

namespace lyt {
// the one-line text area of the layout key16_cmnt (only the class name is from the RTTI)
// RTTI N3lyt22TextAreaOneLineCommentE @ 0x008D0F70
// vtable 0x009044C4, offset_to_top 0, 14 entries
class TextAreaOneLineComment : public ::lyt::TextAreaOneLine
{
public:
    // (inline)
    TextAreaOneLineComment(u32 unknown, s32 priority, sead::Heap* heap) : TextAreaOneLine(unknown, priority, heap) {}
    virtual ~TextAreaOneLineComment(); // 0x00507E8C slot 0x00
    // 0x00507E7C slot 0x04 (deleting dtor)
    const char* GetLayoutName() override { return "key16_cmnt"; } // 0x0074793C slot 0x1C
};
} // namespace lyt
