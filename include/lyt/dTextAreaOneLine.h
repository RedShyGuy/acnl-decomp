#pragma once

#include "decomp.h"
#include "lyt/dTextArea.h"

namespace lyt {
// A TextArea of one line (no guide text and base lines; the text box is 16 wider than the line). The derived classes
// only name their layout (only the class name is from the RTTI).
// RTTI N3lyt15TextAreaOneLineE @ 0x008D0F70
// vtable 0x009043AC (vptr 0x009043B4), offset_to_top 0, 14 entries
class TextAreaOneLine : public ::lyt::TextArea
{
public:
    TextAreaOneLine(u32 unknown, s32 priority, sead::Heap* heap); // 0x00507C28
    virtual ~TextAreaOneLine(); // 0x00507C54 slot 0x00
    // 0x00507C40 slot 0x04 (deleting dtor)
    void Calc(bool isAnimate) override; // 0x00507030 slot 0x08
    void Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader) override; // 0x00506D50 slot 0x10
    bool GetIndexAtPosition(s32* index, const nn::math::VEC2* pos) override; // 0x00506C4C slot 0x14
    const char* GetLayoutName() override { return ""; } // 0x007478D0 slot 0x1C
    s32 GetDecorNum() override { return 1; } // 0x007478F4 slot 0x20
    s32 GetLineNum() override { return 1; } // 0x007478DC slot 0x24
    s32 GetScrollLineNum() override { return 1; } // 0x007478E4 slot 0x28
    s32 GetLineOffset() override { return 0; } // 0x007478EC slot 0x2C
};
} // namespace lyt
