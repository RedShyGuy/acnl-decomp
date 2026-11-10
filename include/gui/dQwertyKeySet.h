#pragma once

#include "decomp.h"
#include "gui/dKeySet.h"

namespace lyt {
class AnmObject;
}

namespace gui {
class ButtonBase;
class TchOnButton;

// The QWERTY key set ("Keytop_qwerty"): 45 character keys, space, backspace, enter, caps, shift and three roman keys
// (alphabet, romaji to hiragana, romaji to katakana); 8 key sets (lower, caps, shift symbols, Japanese, Japanese caps,
// Russian, Russian caps, Russian shift). Only the class name is from the binary (RTTI); member and function names are
// ours.
// RTTI N3gui12QwertyKeySetE @ 0x008D0E18
// vtable 0x00903E0C (vptr 0x00903E14), offset_to_top 0, 10 entries
class QwertyKeySet : public ::gui::KeySet
{
public:
    static const s32 QWERTY_LABEL_NUM = 8;
    // the key sets
    enum
    {
        LABEL_LOWER,
        LABEL_CAPS,
        LABEL_SHIFT,
        LABEL_JP_LOWER,
        LABEL_JP_CAPS,
        LABEL_RU_LOWER,
        LABEL_RU_CAPS,
        LABEL_RU_SHIFT,
    };
    // the roman keys (sys::KeyboardSetting::kanaMode)
    static const s32 ROMAN_KEY_NUM = 3;
    // the index of the enter key in the table
    static const s32 ENTER_KEY = 47;

    explicit QwertyKeySet(ssys::ma::lyt::ArcResAccReader* arcReader); // 0x004F9CD8
    ~QwertyKeySet() override; // 0x004F9E5C slot 0x00
    // 0x004F9D9C slot 0x04 (deleting dtor)
    bool Initialize(s32 label) override; // 0x004F8238 slot 0x08
    void Update() override; // 0x004F93D4 slot 0x0C
    void OnEvent(Widget* sender, s32 event) override; // 0x004F9700 slot 0x10
    void SetVisible(bool visible) override; // 0x004F8004 slot 0x14
    bool IsVisible() override; // 0x004F96F0 slot 0x18
    // (the caps and shift keys follow; the romaji mode follows the Japanese key sets)
    void SetLabel(s32 label) override; // 0x004F9520 slot 0x1C
    lyt::ObjectEx* GetLayout() override; // 0x004F96E8 slot 0x20
    // the first key set of a language (true: Japanese)
    virtual s32 GetFirstLabel(bool isJapanese); // 0x004F80D4 slot 0x24

    // the roman key of the mode is selected (0 alphabet: the composition ends; 1/2: romaji to hiragana/katakana) and the
    // key set follows (Japanese for the romaji)
    void SetRomajiMode(s32 mode); // 0x004F80E4

    static const char* s_LayoutName;
    static const char* s_AnimN0S1;
    static const char* s_AnimS1T0;
    static const char* s_AnimT0S1;
    static const char* s_AnimI0;
    static const char* s_CharLabels[QWERTY_LABEL_NUM];

    lyt::ObjectEx* m_pLayout;                           // 0x110
    ButtonBase* m_pButtons[KEY_MAX];                    // 0x114
    bool m_IsKeyVisible[KEY_MAX][QWERTY_LABEL_NUM];     // 0x214
    bool m_IsKeyEnabled[KEY_MAX][QWERTY_LABEL_NUM];     // 0x414
    s32 m_KeyNum;                                       // 0x614
    s32 m_CapsKey;                                      // 0x618
    s32 m_ShiftKey;                                     // 0x61C
    s32 m_RomanKeys[ROMAN_KEY_NUM];                     // 0x620
    TchOnButton* m_pEnterButton;                        // 0x62C
    lyt::AnmObject* m_pKindAnim;                        // 0x630 (keyboards 17 and 18)
};
ASSERT_SIZE(QwertyKeySet, 0x634);
} // namespace gui
