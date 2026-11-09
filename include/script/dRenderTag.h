#pragma once

#include "decomp.h"
#include "nw/ut/ut_Color8.h"
#include "script/dMsgEngine.h"

namespace script {
// A tag in a buffer of the program, returned by value (the destructor of the original is empty; names are ours).
struct TagText
{
    // (inline)
    ~TagText() {}

    const char16* pText; // 0x0
};

// the tags around inserted words (group 0xFF, index 2..5; names are ours)
extern const char16 s_FrameTag2[5];
extern const char16 s_FrameTag3[5];
extern const char16 s_FrameTag4[5];
extern const char16 s_FrameTag5[5];
// the color tags of the project colors 0..6 / of the default colors (names are ours)
extern const char16 s_ColorTags[7][6];
extern const char16 s_DefaultColorTag[6];

// the tags of the cursor and of the font scale (the parameter is written before use; names are ours)
extern char16 s_CursorTagText[6];
extern char16 s_ScaleTagText[6];

// the tag of a ruby base / a ruby with its width (in 1/32 pixels) for RenderMain::ProcessTag (name is ours)
const Tag& GetStoreTag(f32 width); // 0x005D84B0 (name is ours)
// the color tag of the project color (an index of at least 7: the default colors; name is ours)
TagText GetColorTag(u32 index); // 0x005D848C (name is ours)
// the tag of a color (name is ours)
TagText GetRgbaTag(const nw::ut::Color8& color); // 0x005D851C (name is ours)
} // namespace script
