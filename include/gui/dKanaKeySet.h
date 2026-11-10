#pragma once

#include "decomp.h"
#include "gui/dKeySet.h"

namespace gui {
class ButtonBase;
class TchOnButton;

// The key set of the kana ("Keytop_50on", the table of 50 sounds): 50 kana keys, the long vowel bar, space,
// backspace, enter, the dakuten, handakuten and small kana keys and the two radio keys hiragana/katakana (the two key
// sets). Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui10KanaKeySetE @ 0x008D0DDC
// vtable 0x00903D2C (vptr 0x00903D34), offset_to_top 0, 9 entries
class KanaKeySet : public ::gui::KeySet
{
public:
    static const s32 KANA_LABEL_NUM = 2;

    explicit KanaKeySet(ssys::ma::lyt::ArcResAccReader* arcReader); // 0x004F6CFC
    ~KanaKeySet() override; // 0x004F6E68 slot 0x00
    // 0x004F6DA8 slot 0x04 (deleting dtor)
    bool Initialize(s32 label) override; // 0x004F5B08 slot 0x08
    // (the conversion keys follow the kana before the cursor; the enter key as in GridKeySet::Update)
    void Update() override; // 0x004F67DC slot 0x0C
    void OnEvent(Widget* sender, s32 event) override; // 0x004F6A3C slot 0x10
    void SetVisible(bool visible) override; // 0x004F56FC slot 0x14
    bool IsVisible() override; // 0x004F6A2C slot 0x18
    // (the label is kept in the keyboard setting; the radio key of the key set is selected)
    void SetLabel(s32 label) override; // 0x004F6914 slot 0x1C
    lyt::ObjectEx* GetLayout() override; // 0x004F6A24 slot 0x20

    // the dakuten, handakuten and small kana keys are enabled or disabled
    void UpdateConvertKeys(bool isDakuten, bool isHandakuten, bool isSmall); // 0x004F57CC
    // ... after the kana before the cursor (all disabled without one)
    void UpdateConvertKeysForCursor(); // 0x004F58D4

    static const char* s_LayoutName;
    static const char* s_AnimN0S1;
    static const char* s_AnimS1T0;
    static const char* s_AnimI0;
    static const char* s_CharLabels[KANA_LABEL_NUM];

    lyt::ObjectEx* m_pLayout;                         // 0x110
    ButtonBase* m_pButtons[KEY_MAX];                  // 0x114
    bool m_IsKeyVisible[KEY_MAX][KANA_LABEL_NUM];     // 0x214
    bool m_IsKeyEnabled[KEY_MAX][KANA_LABEL_NUM];     // 0x294
    s32 m_KeyNum;                                     // 0x314
    s32 m_HiraganaKey;                                // 0x318
    s32 m_KatakanaKey;                                // 0x31C
    s32 m_DakutenKey;                                 // 0x320
    s32 m_HandakutenKey;                              // 0x324
    s32 m_SmallKey;                                   // 0x328
    TchOnButton* m_pEnterButton;                      // 0x32C
};
ASSERT_SIZE(KanaKeySet, 0x330);
} // namespace gui
