#pragma once

#include "decomp.h"
#include "gui/dKeySet.h"
#include "sys/sys_Extern.h"

namespace gui {
class ButtonBase;
class TchOnButton;

// The cellphone key set ("Keytop_cellphone"): 12 multi-tap keys (MulKey), space, enter (two layouts: Japan and the
// Americas/Europe), backspace, the caps toggle, the radio keys of the key sets, the kana conversion key and the return
// key that ends a multi-tap; 7 key sets (hiragana, katakana, alphabet, alphabet caps, cyrillic, cyrillic caps,
// numbers). Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui15CellPhoneKeySetE @ 0x008D0E6C
// vtable 0x00903F9C (vptr 0x00903FA4), offset_to_top 0, 9 entries
class CellPhoneKeySet : public ::gui::KeySet
{
public:
    static const s32 CELL_LABEL_NUM = 7;
    // the key sets
    enum
    {
        LABEL_HIRAGANA,
        LABEL_KATAKANA,
        LABEL_ALPHABET,
        LABEL_ALPHABET_CAPS,
        LABEL_CYRILLIC,
        LABEL_CYRILLIC_CAPS,
        LABEL_NUMBER,
    };
    static const s32 RADIO_KEY_NUM = 4;

    explicit CellPhoneKeySet(ssys::ma::lyt::ArcResAccReader* arcReader); // 0x004FDAE0
    ~CellPhoneKeySet() override; // 0x004FDCA0 slot 0x00
    // 0x004FDBCC slot 0x04 (deleting dtor)
    bool Initialize(s32 label) override; // 0x004FB9A8 slot 0x08
    // (the enter key as in GridKeySet::Update; the return key while a multi-tap goes on; the conversion key after the
    // kana before the cursor)
    void Update() override; // 0x004FD2C8 slot 0x0C
    void OnEvent(Widget* sender, s32 event) override; // 0x004FD730 slot 0x10
    void SetVisible(bool visible) override; // 0x004FB8BC slot 0x14
    bool IsVisible() override; // 0x004FD720 slot 0x18
    // (the key sets of the other regions become the alphabet or the hiragana; kept in the keyboard setting; the radio key
    // and the caps toggle follow)
    void SetLabel(s32 label) override; // 0x004FD50C slot 0x1C
    lyt::ObjectEx* GetLayout() override; // 0x004FD718 slot 0x20

    // the key set without caps
    s32 GetLowerLabel(s32 label); // 0x004FB98C
    // the character of the conversion key for the kana (the next form: dakuten, handakuten, small; 0: none)
    DECOMP_EMIT_COPY u16 GetConvertChar(u16 c); // 0x0074759C

    static const char* s_LayoutName;
    static const char* s_AnimN0S1;
    static const char* s_AnimS1T0;
    static const char* s_AnimT0S1;
    static const char* s_AnimI0;
    static const char* s_KeytopLabels[CELL_LABEL_NUM];
    static const char* s_InputLabels[CELL_LABEL_NUM];

    lyt::ObjectEx* m_pLayout;                         // 0x110
    ButtonBase* m_pButtons[KEY_MAX];                  // 0x114
    bool m_IsKeyVisible[KEY_MAX][CELL_LABEL_NUM];     // 0x214
    bool m_IsKeyEnabled[KEY_MAX][CELL_LABEL_NUM];     // 0x3D4
    bool m_IsKeyUnknown[KEY_MAX][CELL_LABEL_NUM];     // 0x594 (set for some multi-tap keys in the Americas/Europe; never read)
    s32 m_KeyNum;                                     // 0x754
    s32 m_ToggleKey;                                  // 0x758
    s32 m_RadioKeys[RADIO_KEY_NUM];                   // 0x75C
    TchOnButton* m_pEnterButton;                      // 0x76C
    ButtonBase* m_pConvertButton;                     // 0x770 (Japan only)
    ButtonBase* m_pReturnButton;                      // 0x774
    ButtonBase* m_pUnknownButton;                     // 0x778 (the keys of kind 7; none in the table)
    s32 m_Unknown0x77C;                               // 0x77C (sys::g_KeyboardSetting._unknown0x00 at the construction)
};
ASSERT_SIZE(CellPhoneKeySet, 0x780);

inline u16 CellPhoneKeySet::GetConvertChar(u16 c)
{
    const bool hasDakuten = sys::HasKanaVariantForm(c, &sys::KanaVariant::dakuten);
    const bool hasHandakuten = sys::HasKanaVariantForm(c, &sys::KanaVariant::handakuten);
    const bool hasSmall = sys::HasKanaVariantForm(c, &sys::KanaVariant::small);
    if (sys::IsKanaForm(c, &sys::KanaVariant::plain)) {
        if (hasDakuten) {
            return g_KanaConvertChars[0];
        }
        if (hasHandakuten) {
            return g_KanaConvertChars[1];
        }
        if (hasSmall) {
            return g_KanaConvertChars[2];
        }
        return 0;
    }
    if (sys::IsKanaForm(c, &sys::KanaVariant::dakuten)) {
        if (hasHandakuten) {
            return g_KanaConvertChars[1];
        }
        if (hasSmall) {
            return g_KanaConvertChars[2];
        }
        return g_KanaConvertChars[0];
    }
    if (sys::IsKanaForm(c, &sys::KanaVariant::handakuten)) {
        if (hasSmall) {
            return g_KanaConvertChars[2];
        }
        return g_KanaConvertChars[1];
    }
    if (sys::IsKanaForm(c, &sys::KanaVariant::small)) {
        return g_KanaConvertChars[2];
    }
    return 0;
}
} // namespace gui
