#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"

namespace lyt {
// the one-line text area of the layout key16_birth (only the class name is from the RTTI)
// RTTI N3lyt20TextAreaOneLineBirthE @ 0x008D0F4C
// vtable 0x0090442C, offset_to_top 0, 14 entries
class TextAreaOneLineBirth : public ::lyt::TextAreaOneLine
{
public:
    // (inline)
    TextAreaOneLineBirth(u32 unknown, s32 priority, sead::Heap* heap) : TextAreaOneLine(unknown, priority, heap) {}
    virtual ~TextAreaOneLineBirth(); // 0x00507C78 slot 0x00
    // 0x00507C68 slot 0x04 (deleting dtor)
    const char* GetLayoutName() override { return "key16_birth"; } // 0x00747910 slot 0x1C
};
} // namespace lyt
