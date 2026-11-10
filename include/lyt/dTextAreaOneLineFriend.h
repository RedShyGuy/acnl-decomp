#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"

namespace lyt {
// the one-line text area of the layout key16_Friend (only the class name is from the RTTI)
// RTTI N3lyt21TextAreaOneLineFriendE @ 0x008D0F64
// vtable 0x00904484, offset_to_top 0, 14 entries
class TextAreaOneLineFriend : public ::lyt::TextAreaOneLine
{
public:
    // (inline)
    TextAreaOneLineFriend(u32 unknown, s32 priority, sead::Heap* heap) : TextAreaOneLine(unknown, priority, heap) {}
    virtual ~TextAreaOneLineFriend(); // 0x00507E78 slot 0x00
    // 0x00507E68 slot 0x04 (deleting dtor)
    const char* GetLayoutName() override { return "key16_Friend"; } // 0x00747924 slot 0x1C
};
} // namespace lyt
