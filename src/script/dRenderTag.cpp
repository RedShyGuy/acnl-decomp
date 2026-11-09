#include "script/dRenderTag.h"

namespace script {
// the tags around inserted words (group 0xFF, index 2..5, no parameters)
// 0x00891E3C
extern const char16 s_FrameTag2[5] = {Tag::TAG_BEGIN, 0xFF, 2, 0, 0};
// 0x00891E46
extern const char16 s_FrameTag3[5] = {Tag::TAG_BEGIN, 0xFF, 3, 0, 0};
// 0x00891E50
extern const char16 s_FrameTag4[5] = {Tag::TAG_BEGIN, 0xFF, 4, 0, 0};
// 0x00891E5A
extern const char16 s_FrameTag5[5] = {Tag::TAG_BEGIN, 0xFF, 5, 0, 0};
// the color tags of the project colors 0..6 and the default color (0xFFFF)
// 0x00891E64
extern const char16 s_ColorTags[7][6] = {
    {Tag::TAG_BEGIN, 0, 3, 2, 0, 0}, {Tag::TAG_BEGIN, 0, 3, 2, 1, 0}, {Tag::TAG_BEGIN, 0, 3, 2, 2, 0},
    {Tag::TAG_BEGIN, 0, 3, 2, 3, 0}, {Tag::TAG_BEGIN, 0, 3, 2, 4, 0}, {Tag::TAG_BEGIN, 0, 3, 2, 5, 0},
    {Tag::TAG_BEGIN, 0, 3, 2, 6, 0},
};
// 0x00891EB8
extern const char16 s_DefaultColorTag[6] = {Tag::TAG_BEGIN, 0, 3, 2, 0xFFFF, 0};

namespace {
// the tag of a ruby base / ruby (group 0xFF, index 0; the parameter is the width)
// 0x0095EF3C
char16 s_StoreTagText[6] = {Tag::TAG_BEGIN, 0xFF, 0, 2, 0, 0};
} // namespace

// the tag of the cursor (group 0xFF, index 1; the parameter is the x in 1/32 pixels)
// 0x0095EF48
char16 s_CursorTagText[6] = {Tag::TAG_BEGIN, 0xFF, 1, 2, 0, 0};

namespace {
// the tag of a color (group 0xFF, index 6; RGBA in two parameters)
// 0x0095EF54
char16 s_RgbaTagText[7] = {Tag::TAG_BEGIN, 0xFF, 6, 4, 0, 0, 0};
} // namespace

// the tag of the font scale (group 0, index 2; the parameter is the scale in percent)
// 0x0095EF62
char16 s_ScaleTagText[6] = {Tag::TAG_BEGIN, 0, 2, 2, 0, 0};

// 0x005D848C (name is ours)
TagText GetColorTag(u32 index)
{
    TagText tag;
    tag.pText = index < 7 ? s_ColorTags[index] : s_DefaultColorTag;
    return tag;
}

// 0x005D84B0 (name is ours)
const Tag& GetStoreTag(f32 width)
{
    s_StoreTagText[4] = static_cast<s32>(width * 32.0f);
    const char16* text = s_StoreTagText;
    // 0x00AD6718 (guard 0x0095EF14)
    static Tag s_StoreTag = Tag::GetTag(&text);
    return s_StoreTag;
}

// 0x005D851C (name is ours)
TagText GetRgbaTag(const nw::ut::Color8& color)
{
    u32 rgba = color.ToU32();
    s_RgbaTagText[4] = rgba >> 16;
    s_RgbaTagText[5] = rgba;
    TagText tag;
    tag.pText = s_RgbaTagText;
    return tag;
}
} // namespace script
