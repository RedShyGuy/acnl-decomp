#pragma once

#include "decomp.h"
#include "sys/dSwkbd.h"

namespace nw {
namespace lyt {
class Pane;
}
} // namespace nw

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
// the pan of a sound at the pane (its horizontal center on the screen)
f32 CalcPaneSoundPan(const nw::lyt::Pane* pane); // 0x004DB298
} // namespace sys
