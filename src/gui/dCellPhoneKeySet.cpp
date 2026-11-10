#include "gui/dCellPhoneKeySet.h"

#include <cstring>
#include <new>

#include "Other/dRegion.h"
#include "Sound/dSoundLabel.h"
#include "gui/dInputButton.h"
#include "gui/dMulKey.h"
#include "gui/dNrmKey.h"
#include "gui/dRadioButtonEx.h"
#include "gui/dRepeatInputButton.h"
#include "gui/dTglButton.h"
#include "gui/dTglKey.h"
#include "gui/gui_KeySetUtil.h"
#include "libms/lms_Api.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "sys/dSwkbd.h"
#include "sys/dTextManager.h"

namespace gui {
namespace {
// the keys of the layout
// 0x00896D18
const KeySet::KeyEntry s_CellPhoneKeys[KeySet::KEY_MAX] = {
    {KeySet::KEY_CHAR, "G_key_00", NULL}, {KeySet::KEY_CHAR, "G_key_01", NULL},
    {KeySet::KEY_CHAR, "G_key_02", NULL}, {KeySet::KEY_CHAR, "G_key_03", NULL},
    {KeySet::KEY_CHAR, "G_key_04", NULL}, {KeySet::KEY_CHAR, "G_key_05", NULL},
    {KeySet::KEY_CHAR, "G_key_06", NULL}, {KeySet::KEY_CHAR, "G_key_07", NULL},
    {KeySet::KEY_CHAR, "G_key_08", NULL}, {KeySet::KEY_CHAR, "G_key_09", NULL},
    {KeySet::KEY_CHAR, "G_key_10", NULL}, {KeySet::KEY_CHAR, "G_key_11", NULL},
    {KeySet::KEY_SPACE, "G_key_Spc", "cell_space"}, {KeySet::KEY_ENTER, "G_key_Ent", NULL},
    {KeySet::KEY_ENTER, "G_key_EntUSEU", NULL}, {KeySet::KEY_BACKSPACE, "G_key_Bsp", "cell_backspace"},
    {KeySet::KEY_SWITCH, "G_key_Cps", NULL}, {KeySet::KEY_5, "G_key_L01", "cell_l01"},
    {KeySet::KEY_5, "G_key_L02", "cell_l02"}, {KeySet::KEY_5, "G_key_L03", "cell_l03"},
    {KeySet::KEY_5, "G_key_L04", "cell_l04"}, {KeySet::KEY_5, "G_keyUSEU_L01", "cell_l03"},
    {KeySet::KEY_5, "G_keyUSEU_L02", "cell_l04"}, {KeySet::KEY_6, "G_key_JPDaku", NULL},
    {KeySet::KEY_7 + 1, "G_key_Ret", NULL}, {KeySet::KEY_END, NULL, NULL},
};
// the pane animations of the multi-tap keys: n0s1 with the press, i0 with the disabling
// 0x00896CD8
const s32 s_MulSubAnimSets[2] = {0, 5};
// ... of the other keys
// 0x00896CE0
const s32 s_CellPhoneSubAnimSets[2] = {0, 5};
// the radio keys: n0s1 with the press, s1t0 with the decision, i0 with the disabling
// 0x00898484
const s32 s_CellPhoneRadioSubAnimSets[3] = {0, 2, 5};
// the caps toggle: n0s1 with the press, i0 with the disabling, t0s1 while it is on
// 0x00898490
const s32 s_CellPhoneToggleSubAnimSets[3] = {0, 5, 3};

// the key layout with a text that is never shown (the text boxes get a space)
const wchar_t* const SPACE_TEXT = L" ";
// the most characters of a multi-tap key
const s32 MUL_CHAR_MAX = 16;

// (inline) the text of the label in swkbd.msbt
const wchar_t* GetText(const char* label)
{
    return static_cast<const wchar_t*>(LMS_GetTextByLabel(sys::g_pMessage, label));
}

// (inline) the line of the text (lines end with '\n')
const wchar_t* SkipLines(const wchar_t* text, s32 lineNum)
{
    for (s32 i = 0; i < lineNum; i++) {
        for (;;) {
            const u16 c = *text;
            if (c == '\n') {
                text++;
                break;
            }
            if (c == 0) {
                break;
            }
            text++;
        }
    }
    return text;
}

// (inline) the character is forbidden by the filters of the keyboard
bool IsForbiddenChar(u16 c)
{
    for (s32 i = 0; i < sys::g_ForbiddenCharNum; i++) {
        if (sys::g_ForbiddenChars[i] == c) {
            return true;
        }
    }
    return sys::IsDigitChar(c);
}

// (inline) the allowed characters of the line (at most MUL_CHAR_MAX); their number
s32 CollectChars(u16* dst, const wchar_t* text)
{
    s32 num = 0;
    do {
        const u16 c = *text;
        if (!IsForbiddenChar(c)) {
            dst[num++] = c;
        }
        text++;
    } while (*text != '\n' && *text != 0 && num < MUL_CHAR_MAX);
    return num;
}

// (inline) the name of the first text box of the group (NULL: none)
const char* FindFirstTextBox(lyt::ObjectEx* layout, const char* groupName)
{
    nw::lyt::Group::PaneLinkList& paneList = layout->m_Layout.GetLayout().GetGroupContainer()->FindGroupByName(groupName)->GetPaneList();
    for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
        nw::lyt::Pane* pane = nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget;
        nw::lyt::TextBox* textBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(pane);
        if (textBox != NULL) {
            return textBox->m_Name;
        }
    }
    return NULL;
}

// (inline) the region is the Americas or Europe (GetRegionIndex 1 or 2)
bool IsRegionAmericaEurope()
{
    return GetRegionIndex() == 1 || GetRegionIndex() == 2;
}
} // namespace

// 0x0095F13C
const char* CellPhoneKeySet::s_LayoutName = "Keytop_cellphone";
// 0x0095F140
const char* CellPhoneKeySet::s_AnimN0S1 = "n0s1";
// 0x0095F144
const char* CellPhoneKeySet::s_AnimS1T0 = "s1t0";
// 0x0095F148
const char* CellPhoneKeySet::s_AnimT0S1 = "t0s1";
// 0x0095F14C
const char* CellPhoneKeySet::s_AnimI0 = "i0";
// the key tops of the multi-tap keys (a line per key)
// 0x0095F240
const char* CellPhoneKeySet::s_KeytopLabels[CELL_LABEL_NUM] = {
    "cell_keytop_hira",       "cell_keytop_kata",   "cell_keytop_alpha", "cell_keytop_alpha_c",
    "cell_keytop_cyrillic",   "cell_keytop_cyrillic_c", "cell_keytop_number",
};
// the characters of the multi-tap keys (a line per key)
// 0x0095F25C
const char* CellPhoneKeySet::s_InputLabels[CELL_LABEL_NUM] = {
    "cell_input_hira",     "cell_input_kata",       "cell_input_alpha", "cell_input_alpha_c",
    "cell_input_cyrillic", "cell_input_cyrillic_c", "cell_input_number",
};

// 0x004FB8BC slot 0x14
void CellPhoneKeySet::SetVisible(bool visible)
{
    for (s32 i = 0; i < m_KeyNum; i++) {
        if (visible) {
            const bool isKeyVisible = m_IsKeyVisible[i][m_Label];
            m_pLayout->SetGroupVisible(s_CellPhoneKeys[i].groupName, isKeyVisible);
            m_pButtons[i]->SetActive(isKeyVisible);
        } else {
            m_pButtons[i]->SetActive(false);
        }
    }
    m_pLayout->SetEnabled(visible);
}

// 0x004FB98C
s32 CellPhoneKeySet::GetLowerLabel(s32 label)
{
    if (label == LABEL_ALPHABET_CAPS) {
        return LABEL_ALPHABET;
    }
    if (label == LABEL_CYRILLIC_CAPS) {
        return LABEL_CYRILLIC;
    }
    return label;
}

// 0x004FB9A8 slot 0x08
bool CellPhoneKeySet::Initialize(s32 label)
{
    // the animations of the key panes ("" none)
    // 0x0095F228 (guard 0x0095F18C)
    static const char* s_CellPhoneAnimNames[6] = {s_AnimN0S1, "", s_AnimS1T0, s_AnimT0S1, "", s_AnimI0};
    s32 mulKeyNum = 0;
    sead::Heap* heap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    u32 paneAnimNums[6] = {};
    lyt::AnmObject* paneAnims[6][detail::PANE_ANIM_MAX];
    char name[64];
    nw::ut::FormatString(name, sizeof(name), "%s.bclyt", s_LayoutName);
    m_pLayout = new (heap, 4) lyt::ObjectEx(0, 500, NULL);
    m_pLayout->Build(m_pArcReader, name, 0x5E00);
    m_pLayout->SetText(SPACE_TEXT, "T_key_Tra", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_Spc", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_BspJP", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_L01", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_L02", false, 0, 0);
    for (s32 i = 0; i < 6; i++) {
        if (s_CellPhoneAnimNames[i][0] != '\0') {
            paneAnimNums[i] = detail::CreatePaneAnimations(m_pLayout, s_LayoutName, s_CellPhoneAnimNames[i], paneAnims[i]);
        }
    }
    for (s32 key = 0; key < m_KeyNum; key++) {
        const s32 kind = detail::GetKeyKind<KEY_7 + 2, KEY_NONE>(s_CellPhoneKeys, m_KeyNum, key);
        const KeyEntry* entry = &s_CellPhoneKeys[key];
        switch (kind) {
        case KEY_CHAR: {
            // a multi-tap key
            ButtonEx::Description desc;
            desc.pLayout = m_pLayout;
            desc.groupName = entry->groupName;
            lyt::AnmObject** const dsts[2] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DISABLE]};
            detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_MulSubAnimSets, paneAnims, paneAnimNums);
            desc.sounds[ButtonBase::SOUND_DECIDE] = -1;
            desc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_KEY;
            m_pButtons[key] = new (heap, 4) InputButton(desc);
            MulKey::Description keyDesc;
            wchar_t labelTexts[CELL_LABEL_NUM][MUL_CHAR_MAX + 1];
            sys::TextManager::CellPhoneInput inputs[CELL_LABEL_NUM];
            keyDesc.labelNum = CELL_LABEL_NUM;
            keyDesc.pButton = static_cast<ButtonEx*>(m_pButtons[key]);
            for (s32 i = 0; i < CELL_LABEL_NUM; i++) {
                // the key top: the line of the key without the forbidden characters
                const wchar_t* text = SkipLines(GetText(s_KeytopLabels[i]), mulKeyNum);
                if (*text == '\n' || *text == 0) {
                    m_IsKeyEnabled[key][i] = false;
                } else {
                    const s32 num = CollectChars(reinterpret_cast<u16*>(labelTexts[i]), text);
                    if (num == 0) {
                        m_IsKeyEnabled[key][i] = false;
                    }
                    labelTexts[i][num] = 0;
                    keyDesc.labels[i] = labelTexts[i];
                }
                // the characters the presses cycle through
                text = SkipLines(GetText(s_InputLabels[i]), mulKeyNum);
                if (*text == '\n' || *text == 0) {
                    m_IsKeyEnabled[key][i] = false;
                } else {
                    u16 chars[MUL_CHAR_MAX];
                    s32 num = CollectChars(chars, text);
                    if (num == 0) {
                        m_IsKeyEnabled[key][i] = false;
                    }
                    if (num < 0) {
                        num = 0;
                    } else if (num > MUL_CHAR_MAX) {
                        num = MUL_CHAR_MAX;
                    }
                    for (s32 j = 0; j < num; j++) {
                        inputs[i].m_Chars[j] = chars[j];
                    }
                    inputs[i].m_Num = num;
                    inputs[i].m_Index = 0;
                    keyDesc.inputs[i] = &inputs[i];
                }
                if (IsRegionAmericaEurope()) {
                    if (std::strcmp(entry->groupName, "G_key_10") == 0) {
                        m_IsKeyUnknown[key][i] = true;
                    } else if (i == LABEL_NUMBER) {
                        m_IsKeyUnknown[key][LABEL_NUMBER] = true;
                    }
                }
            }
            const char* textBoxName = detail::FindLabelTextBox(m_pLayout, entry->groupName);
            if (textBoxName != NULL) {
                keyDesc.textBoxName = textBoxName;
            }
            m_pKeys[key] = new (heap, 4) MulKey(keyDesc);
            mulKeyNum++;
            break;
        }
        case KEY_SPACE:
        case KEY_ENTER:
        case KEY_BACKSPACE:
        case KEY_5:
        case KEY_6:
        case KEY_7:
        case KEY_7 + 1: {
            if (kind == KEY_5) {
                RadioButtonEx::Description desc;
                desc.pLayout = m_pLayout;
                desc.groupName = entry->groupName;
                desc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_KEY;
                desc.sounds[ButtonBase::SOUND_DECIDE] = SE_SYS_SWK_TOGGLE_ON;
                desc.group = 'CKS0';
                lyt::AnmObject** const dsts[3] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DECIDE],
                                                  desc.subAnims[ButtonBase::ANIM_DISABLE]};
                detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_CellPhoneRadioSubAnimSets, paneAnims, paneAnimNums);
                m_pButtons[key] = new (heap, 4) RadioButtonEx(desc);
            } else {
                ButtonEx::Description desc;
                desc.pLayout = m_pLayout;
                desc.groupName = entry->groupName;
                lyt::AnmObject** const dsts[2] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DISABLE]};
                detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_CellPhoneSubAnimSets, paneAnims, paneAnimNums);
                switch (kind) {
                case KEY_SPACE:
                case KEY_BACKSPACE:
                    desc.sounds[ButtonBase::SOUND_PRESS] = -1;
                    desc.sounds[ButtonBase::SOUND_DECIDE] = -1;
                    break;
                case KEY_ENTER:
                    desc.sounds[ButtonBase::SOUND_PRESS] = -1;
                    break;
                case KEY_6:
                case KEY_7 + 1:
                    desc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_ENTER;
                    desc.sounds[ButtonBase::SOUND_DECIDE] = -1;
                    break;
                }
                switch (kind) {
                case KEY_ENTER:
                case KEY_6:
                case KEY_7 + 1:
                    m_pButtons[key] = new (heap, 4) InputButton(desc);
                    break;
                case KEY_SPACE: {
                    RepeatButtonEx::Description repeatDesc;
                    static_cast<ButtonEx::Description&>(repeatDesc) = desc;
                    repeatDesc.repeatDelay = 20;
                    repeatDesc.repeatIntervals[0] = 8;
                    m_pButtons[key] = new (heap, 4) RepeatInputButton(repeatDesc);
                    break;
                }
                case KEY_BACKSPACE: {
                    RepeatButtonEx::Description repeatDesc;
                    static_cast<ButtonEx::Description&>(repeatDesc) = desc;
                    repeatDesc.repeatDelay = 20;
                    repeatDesc.repeatIntervals[0] = 3;
                    repeatDesc.repeatIntervals[1] = 2;
                    repeatDesc.stageRepeats[0] = 15;
                    m_pButtons[key] = new (heap, 4) RepeatInputButton(repeatDesc);
                    break;
                }
                case KEY_7:
                    m_pButtons[key] = new (heap, 4) ButtonEx(desc);
                    break;
                }
            }
            // the labels and characters of the key sets
            NrmKey::Description keyDesc;
            keyDesc.labelNum = CELL_LABEL_NUM;
            keyDesc.pButton = static_cast<ButtonEx*>(m_pButtons[key]);
            for (s32 i = 0; i < CELL_LABEL_NUM; i++) {
                if (entry->labelName != NULL) {
                    const wchar_t* text = GetText(entry->labelName);
                    keyDesc.labels[i] = text;
                    if (kind == KEY_SPACE) {
                        // the ideographic space with the kana; no space in the Americas and Europe
                        keyDesc.chars[i] = (i == LABEL_HIRAGANA || i == LABEL_KATAKANA) ? 0x3000 : ' ';
                        if (IsRegionAmericaEurope()) {
                            m_IsKeyVisible[key][i] = false;
                        }
                        continue;
                    }
                    if (kind == KEY_BACKSPACE) {
                        // the icon of the other language is hidden
                        if (text[0] != 0) {
                            m_pLayout->FindPane("P_key_Bspicon")->m_Alpha = 0;
                        } else {
                            m_pLayout->FindPane("P_Key_BspJPIcon")->m_Alpha = 0;
                        }
                        continue;
                    }
                }
                switch (kind) {
                case KEY_ENTER:
                    keyDesc.chars[i] = '\n';
                    if (!sys::g_KeyboardParam.isNewlineAllowed) {
                        m_IsKeyEnabled[key][i] = false;
                    }
                    // the enter key of the other layout is hidden
                    if (IsRegionAmericaEurope()) {
                        if (std::strcmp(entry->groupName, "G_key_Ent") == 0) {
                            m_IsKeyVisible[key][i] = false;
                        }
                    } else if (std::strcmp(entry->groupName, "G_key_EntUSEU") == 0) {
                        m_IsKeyVisible[key][i] = false;
                    }
                    break;
                case KEY_5:
                    // the radio keys of the other layout are hidden; the number key set needs digits
                    if (IsRegionAmericaEurope()) {
                        if (std::strcmp(entry->groupName, "G_key_L01") == 0 || std::strcmp(entry->groupName, "G_key_L02") == 0 ||
                            std::strcmp(entry->groupName, "G_key_L03") == 0 || std::strcmp(entry->groupName, "G_key_L04") == 0) {
                            m_IsKeyVisible[key][i] = false;
                        }
                        if (std::strcmp(entry->groupName, "G_keyUSEU_L02") == 0 && sys::IsDigitChar('1')) {
                            m_IsKeyEnabled[key][i] = false;
                            if (label == LABEL_NUMBER) {
                                label = LABEL_ALPHABET;
                            }
                        }
                    } else {
                        if (std::strcmp(entry->groupName, "G_keyUSEU_L01") == 0 || std::strcmp(entry->groupName, "G_keyUSEU_L02") == 0) {
                            m_IsKeyVisible[key][i] = false;
                        }
                        if (std::strcmp(entry->groupName, "G_key_L04") == 0 && sys::IsDigitChar('1')) {
                            m_IsKeyEnabled[key][i] = false;
                            if (label == LABEL_NUMBER) {
                                label = LABEL_HIRAGANA;
                            }
                        }
                    }
                    break;
                case KEY_6:
                    // the conversion key (only with the kana)
                    m_IsKeyEnabled[key][i] = false;
                    if (i >= LABEL_ALPHABET) {
                        m_IsKeyVisible[key][i] = false;
                    }
                    break;
                case KEY_7:
                    if (IsRegionAmericaEurope()) {
                        m_IsKeyEnabled[key][i] = false;
                    } else {
                        m_IsKeyVisible[key][i] = false;
                    }
                    break;
                case KEY_7 + 1:
                    m_IsKeyEnabled[key][i] = false;
                    break;
                }
            }
            const char* textBoxName = FindFirstTextBox(m_pLayout, entry->groupName);
            if (textBoxName != NULL) {
                keyDesc.textBoxName = textBoxName;
            }
            m_pKeys[key] = new (heap, 4) NrmKey(keyDesc);
            break;
        }
        case KEY_SWITCH: {
            // the caps toggle
            TglButton::Description desc;
            desc.pLayout = m_pLayout;
            desc.groupName = entry->groupName;
            lyt::AnmObject** const dsts[3] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DISABLE],
                                              desc.onSubAnims[ButtonBase::ANIM_PRESS]};
            detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_CellPhoneToggleSubAnimSets, paneAnims, paneAnimNums);
            m_pButtons[key] = new (heap, 4) TglButton(desc);
            TglKey::Description keyDesc;
            keyDesc.labelNum = CELL_LABEL_NUM;
            keyDesc.pButton = static_cast<ButtonEx*>(m_pButtons[key]);
            for (s32 i = 0; i < CELL_LABEL_NUM; i++) {
                if (entry->labelName != NULL) {
                    keyDesc.labels[i] = GetText(entry->labelName);
                }
                // only the alphabet and the cyrillic have caps
                if (i < LABEL_ALPHABET || i > LABEL_CYRILLIC_CAPS) {
                    m_IsKeyVisible[key][i] = false;
                }
            }
            const char* textBoxName = FindFirstTextBox(m_pLayout, entry->groupName);
            if (textBoxName != NULL) {
                keyDesc.textBoxName = textBoxName;
            }
            m_pKeys[key] = new (heap, 4) TglKey(keyDesc);
            break;
        }
        }
        switch (kind) {
        case KEY_SWITCH:
            m_ToggleKey = key;
            break;
        case KEY_5:
            if (IsRegionAmericaEurope()) {
                if (std::strcmp(entry->groupName, "G_keyUSEU_L01") == 0) {
                    m_RadioKeys[2] = key;
                } else if (std::strcmp(entry->groupName, "G_keyUSEU_L02") == 0) {
                    m_RadioKeys[3] = key;
                }
            } else if (std::strcmp(entry->groupName, "G_key_L01") == 0) {
                m_RadioKeys[0] = key;
            } else if (std::strcmp(entry->groupName, "G_key_L02") == 0) {
                m_RadioKeys[1] = key;
            } else if (std::strcmp(entry->groupName, "G_key_L03") == 0) {
                m_RadioKeys[2] = key;
            } else if (std::strcmp(entry->groupName, "G_key_L04") == 0) {
                m_RadioKeys[3] = key;
            }
            break;
        case KEY_ENTER:
            // (the enter key of the region)
            if (m_IsKeyVisible[key][0]) {
                m_pEnterButton = static_cast<TchOnButton*>(m_pButtons[key]);
            }
            break;
        case KEY_6:
            if (GetRegionIndex() == 0) {
                m_pConvertButton = m_pButtons[key];
            }
            break;
        case KEY_7 + 1:
            m_pReturnButton = m_pButtons[key];
            break;
        case KEY_7:
            m_pUnknownButton = m_pButtons[key];
            break;
        }
    }
    SetLabel(label);
    m_pLayout->m_State = lyt::Object::STATE_CHANGED;
    return true;
}

// 0x004FD2C8 slot 0x0C
void CellPhoneKeySet::Update()
{
    sys::TextManager* textManager = sys::g_pTextManager;
    for (s32 i = 0; i < KEY_MAX; i++) {
        if (m_pKeys[i] != NULL) {
            m_pKeys[i]->Update();
        }
    }
    if (sys::g_KeyboardParam.isNewlineAllowed || textManager->m_Length - textManager->m_CompositionLength < textManager->m_Length) {
        if (m_pEnterButton->IsTouchOn()) {
            m_pEnterButton->SetTouchOn(false);
        }
        if (!m_pEnterButton->IsEnabled()) {
            m_pEnterButton->SetEnabled(true, true, false);
        }
    } else {
        m_pEnterButton->SetTouchOn(true);
    }
    m_pLayout->m_State = lyt::Object::STATE_CHANGED;
    // the return key ends a multi-tap
    TchOnButton* returnButton = static_cast<TchOnButton*>(m_pReturnButton);
    const s32 cursor = textManager->m_pCellPhoneInput != NULL && textManager->m_Cursor > 0 ? textManager->m_Cursor - 1 : -1;
    if (cursor >= 0) {
        if (returnButton->IsTouchOn()) {
            returnButton->SetTouchOn(false);
        }
        if (!returnButton->IsEnabled()) {
            returnButton->SetEnabled(true, true, false);
        }
    } else {
        returnButton->SetTouchOn(true);
    }
    // the conversion key
    if (m_pConvertButton != NULL) {
        bool isEnabled = false;
        if (textManager->m_Cursor > 0 && GetConvertChar(textManager->m_pBuffer[textManager->m_Cursor - 1]) != 0) {
            isEnabled = true;
        }
        if (m_pConvertButton->IsEnabled() != isEnabled) {
            m_pConvertButton->SetEnabled(isEnabled, true, false);
            m_pLayout->m_State = lyt::Object::STATE_CHANGED;
        }
    }
}

// 0x004FD50C slot 0x1C
void CellPhoneKeySet::SetLabel(s32 label)
{
    // the key sets of the other regions
    if (GetRegionIndex() != 0 && (label == LABEL_HIRAGANA || label == LABEL_KATAKANA)) {
        label = LABEL_ALPHABET;
    } else if (label == LABEL_CYRILLIC || label == LABEL_CYRILLIC_CAPS) {
        label = GetRegionIndex() == 0 ? LABEL_HIRAGANA : LABEL_ALPHABET;
    }
    sys::g_KeyboardSetting.cellPhoneKeySet = label;
    KeySet::SetLabel(label);
    if (IsVisible()) {
        SetVisible(true);
    }
    s32 radio = 0;
    if (IsRegionAmericaEurope()) {
        if (label == LABEL_ALPHABET || label == LABEL_ALPHABET_CAPS) {
            radio = 2;
        } else if (label == LABEL_NUMBER) {
            radio = 3;
        }
    } else {
        switch (label) {
        case LABEL_KATAKANA:
            radio = 1;
            break;
        case LABEL_ALPHABET:
        case LABEL_ALPHABET_CAPS:
            radio = 2;
            break;
        case LABEL_NUMBER:
            radio = 3;
            break;
        }
    }
    static_cast<RadioButtonEx*>(m_pButtons[m_RadioKeys[radio]])->Select(false);
    switch (label) {
    case LABEL_ALPHABET:
    case LABEL_CYRILLIC:
        static_cast<TglButton*>(m_pButtons[m_ToggleKey])->SetOn(false);
        break;
    case LABEL_ALPHABET_CAPS:
    case LABEL_CYRILLIC_CAPS:
        static_cast<TglButton*>(m_pButtons[m_ToggleKey])->SetOn(true);
        break;
    }
    for (s32 i = 0; i < m_KeyNum; i++) {
        const bool isEnabled = m_IsKeyEnabled[i][label];
        if (!isEnabled || m_pButtons[i]->IsEnabled() != isEnabled) {
            m_pButtons[i]->SetEnabled(isEnabled, true, false);
        }
    }
}

// 0x004FD718 slot 0x20
lyt::ObjectEx* CellPhoneKeySet::GetLayout()
{
    return m_pLayout;
}

// 0x004FD720 slot 0x18
bool CellPhoneKeySet::IsVisible()
{
    return m_pLayout->m_IsEnabled;
}

// 0x004FD730 slot 0x10
void CellPhoneKeySet::OnEvent(Widget* sender, s32 event)
{
    sys::TextManager* textManager = sys::g_pTextManager;
    m_IsHandled = false;
    if (event != EVENT_DECIDE) {
        return;
    }
    for (s32 key = 0; key < m_KeyNum; key++) {
        if (m_pButtons[key] != sender) {
            continue;
        }
        ButtonBase* button = m_pButtons[key];
        switch (detail::GetKeyKind<KEY_7 + 2, KEY_NONE>(s_CellPhoneKeys, m_KeyNum, key)) {
        case KEY_CHAR:
            if (textManager->InputCellPhone(static_cast<MulKey*>(m_pKeys[key])->GetInput())) {
                break;
            }
            // the text is full: the key goes back
            if (textManager->m_IsFull == true) {
                button->Reset(false);
            }
            return;
        case KEY_SPACE:
        case KEY_ENTER:
            if (textManager->InputChar(static_cast<NrmKey*>(m_pKeys[key])->GetChar(), 0, false)) {
                break;
            }
            if (textManager->m_IsFull == true) {
                button->Reset(false);
            }
            return;
        case KEY_BACKSPACE:
            if (!textManager->Backspace(false)) {
                return;
            }
            break;
        case KEY_SWITCH:
            // caps
            if (static_cast<TglButton*>(m_pButtons[m_ToggleKey])->m_IsOn) {
                SetLabel(m_Label == LABEL_CYRILLIC ? LABEL_CYRILLIC_CAPS : LABEL_ALPHABET_CAPS);
            } else {
                SetLabel(m_Label == LABEL_CYRILLIC_CAPS ? LABEL_CYRILLIC : LABEL_ALPHABET);
            }
            break;
        case KEY_5:
            // the radio keys of the key sets
            if (m_RadioKeys[0] == key) {
                SetLabel(LABEL_HIRAGANA);
            } else if (m_RadioKeys[1] == key) {
                SetLabel(LABEL_KATAKANA);
            } else if (m_RadioKeys[2] == key) {
                SetLabel(static_cast<TglButton*>(m_pButtons[m_ToggleKey])->m_IsOn ? LABEL_ALPHABET_CAPS : LABEL_ALPHABET);
            } else if (m_RadioKeys[3] == key) {
                SetLabel(LABEL_NUMBER);
            }
            textManager->EndCellPhone(true);
            return;
        case KEY_6: {
            // the conversion key
            const u16 c = textManager->m_Cursor > 0 ? GetConvertChar(textManager->m_pBuffer[textManager->m_Cursor - 1]) : 0;
            InputButton* inputButton = static_cast<InputButton*>(m_pButtons[key]);
            if (c == 0) {
                textManager->EndCellPhone(true);
                break;
            }
            if (!textManager->InputChar(c, 0, false)) {
                // nothing to convert
                SoundMgr::PlaySound(SE_SYS_SWK_ERROR, inputButton->m_Desc.pLayout->m_SoundPan);
                inputButton->m_IsSilent = true;
                return;
            }
            break;
        }
        case KEY_7 + 1: {
            // the return key
            textManager->EndCellPhone(true);
            ButtonEx* buttonEx = static_cast<ButtonEx*>(button);
            if (buttonEx->m_Desc.sounds[ButtonBase::SOUND_PRESS] > 0) {
                SoundMgr::PlaySound(static_cast<SeID>(buttonEx->m_Desc.sounds[ButtonBase::SOUND_PRESS]), buttonEx->m_Desc.pLayout->m_SoundPan);
            }
            break;
        }
        default:
            return;
        }
        m_IsHandled = true;
        return;
    }
}

// 0x004FDAE0
CellPhoneKeySet::CellPhoneKeySet(ssys::ma::lyt::ArcResAccReader* arcReader)
    : KeySet(arcReader), m_pLayout(NULL), m_KeyNum(0), m_ToggleKey(-1), m_pEnterButton(NULL), m_pConvertButton(NULL),
      m_pReturnButton(NULL), m_pUnknownButton(NULL), m_Unknown0x77C(sys::g_KeyboardSetting._unknown0x00)
{
    std::memset(m_pButtons, 0, sizeof(m_pButtons));
    m_KeyNum = 25;
    for (s32 i = 0; i < m_KeyNum; i++) {
        m_pButtons[i] = NULL;
        std::memset(m_IsKeyVisible[i], true, sizeof(m_IsKeyVisible[i]));
        std::memset(m_IsKeyEnabled[i], true, sizeof(m_IsKeyEnabled[i]));
        std::memset(m_IsKeyUnknown[i], false, sizeof(m_IsKeyUnknown[i]));
    }
    for (s32 i = 0; i < RADIO_KEY_NUM; i++) {
        m_RadioKeys[i] = -1;
    }
}

// 0x004FDCA0 slot 0x00
// 0x004FDBCC (deleting dtor)
CellPhoneKeySet::~CellPhoneKeySet()
{
    sead::Heap* heap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    for (s32 i = 0; i < m_KeyNum; i++) {
        ButtonBase* button = m_pButtons[i];
        if (button != NULL) {
            button->~ButtonBase();
            heap->free(button);
            m_pButtons[i] = NULL;
        }
    }
    lyt::ObjectEx* layout = m_pLayout;
    if (layout != NULL) {
        layout->~ObjectEx();
        heap->free(layout);
        m_pLayout = NULL;
    }
}
} // namespace gui
