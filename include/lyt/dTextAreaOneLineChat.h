#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"

namespace lyt {
// the one-line text area of the layout key16_chat (only the class name is from the RTTI)
// RTTI N3lyt19TextAreaOneLineChatE @ 0x008D0F40
// vtable 0x009043EC, offset_to_top 0, 14 entries
class TextAreaOneLineChat : public ::lyt::TextAreaOneLine
{
public:
    // (inline)
    TextAreaOneLineChat(u32 unknown, s32 priority, sead::Heap* heap) : TextAreaOneLine(unknown, priority, heap) {}
    virtual ~TextAreaOneLineChat(); // 0x00507C50 slot 0x00 (falls into ~TextAreaOneLine)
    // 0x00507C58 slot 0x04 (deleting dtor)
    const char* GetLayoutName() override { return "key16_chat"; } // 0x007478FC slot 0x1C
};
} // namespace lyt
