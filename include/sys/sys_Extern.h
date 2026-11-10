#pragma once

#include "decomp.h"
#include "sys/dSwkbd.h"

namespace nw {
namespace lyt {
class Pane;
}
} // namespace nw
namespace lyt {
class Object;
}

// Game code and globals that sys (the text input of the software keyboard) uses and that are not decompiled yet. Names
// are ours.
namespace sys {
// a variant of a kana (the plain kana and its forms with dakuten, handakuten and the small form; 0: none)
struct KanaVariant
{
    u16 plain;
    u16 dakuten;
    u16 handakuten;
    u16 small;
};

// the romaji typed so far (inLength characters) as kana in out (outSize characters at most); the number of kana that
// are complete (the rest of out is romaji that is still being composed)
s32 ConvertRomajiToKana(wchar_t* out, s32 outSize, const wchar_t* in, s32 inLength, bool isHiragana); // 0x004DAD4C
// the variants of the kana (NULL: it has none)
const KanaVariant* FindKanaVariant(u16 c); // 0x004DB4E0

// the variants of the hiragana (from U+3041) and the katakana (from U+30A1)
const s32 KANA_VARIANT_NUM = 46;
extern const KanaVariant s_HiraganaVariants[KANA_VARIANT_NUM];
extern const KanaVariant s_KatakanaVariants[KANA_VARIANT_NUM];

// (inline; names are ours) the variants of the kana have a form of the kind other than the kana itself
inline bool HasKanaVariantForm(u16 c, u16 KanaVariant::*form)
{
    const KanaVariant* variant = FindKanaVariant(c);
    u16 formChar = variant != NULL ? variant->*form : 0;
    if (formChar == 0) {
        formChar = c;
    }
    return formChar != c;
}
// ... the kana is the form of the kind of a variant
inline bool IsKanaForm(u16 c, u16 KanaVariant::*form)
{
    if (static_cast<u32>(c - 0x3041) <= 82) {
        for (s32 i = 0; i < KANA_VARIANT_NUM; i++) {
            if (s_HiraganaVariants[i].*form == c) {
                return true;
            }
        }
    } else if (static_cast<u32>(c - 0x30A1) <= 85) {
        for (s32 i = 0; i < KANA_VARIANT_NUM; i++) {
            if (s_KatakanaVariants[i].*form == c) {
                return true;
            }
        }
    }
    return false;
}
// ... the kana has a form of the kind (dakuten, handakuten or small) or is one
inline bool HasKanaForm(u16 c, u16 KanaVariant::*form)
{
    return HasKanaVariantForm(c, form) || IsKanaForm(c, form);
}
// the pan of a sound at the pane (its horizontal center on the screen)
f32 CalcPaneSoundPan(const nw::lyt::Pane* pane); // 0x004DB298
// the touch position is in a bounding pane of the group of the layout: the 1-based index of that pane among the
// bounding panes of the group (the layout gets the sound pan of the pane); 0: none
s32 TouchGroup(lyt::Object* layout, const char* groupName); // 0x004DB3A8
// the touch position (g_TouchPosition) is in the rectangle of the pane
bool IsTouchInPane(const nw::lyt::Pane* pane); // 0x004DB300
} // namespace sys
