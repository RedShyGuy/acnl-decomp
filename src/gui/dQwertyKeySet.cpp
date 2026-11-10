#include "gui/dQwertyKeySet.h"

#include <cstring>
#include <new>

#include "Other/dRegion.h"
#include "Sound/dSoundLabel.h"
#include "event/dEventBase.h"
#include "gui/dInputButton.h"
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
// 0x00897618
const KeySet::KeyEntry s_QwertyKeys[KeySet::KEY_MAX] = {
    {KeySet::KEY_CHAR, "G_key_00", NULL}, {KeySet::KEY_CHAR, "G_key_01", NULL},
    {KeySet::KEY_CHAR, "G_key_02", NULL}, {KeySet::KEY_CHAR, "G_key_03", NULL},
    {KeySet::KEY_CHAR, "G_key_04", NULL}, {KeySet::KEY_CHAR, "G_key_05", NULL},
    {KeySet::KEY_CHAR, "G_key_06", NULL}, {KeySet::KEY_CHAR, "G_key_07", NULL},
    {KeySet::KEY_CHAR, "G_key_08", NULL}, {KeySet::KEY_CHAR, "G_key_09", NULL},
    {KeySet::KEY_CHAR, "G_key_10", NULL}, {KeySet::KEY_CHAR, "G_key_11", NULL},
    {KeySet::KEY_CHAR, "G_key_12", NULL}, {KeySet::KEY_CHAR, "G_key_13", NULL},
    {KeySet::KEY_CHAR, "G_key_14", NULL}, {KeySet::KEY_CHAR, "G_key_15", NULL},
    {KeySet::KEY_CHAR, "G_key_16", NULL}, {KeySet::KEY_CHAR, "G_key_17", NULL},
    {KeySet::KEY_CHAR, "G_key_18", NULL}, {KeySet::KEY_CHAR, "G_key_19", NULL},
    {KeySet::KEY_CHAR, "G_key_20", NULL}, {KeySet::KEY_CHAR, "G_key_21", NULL},
    {KeySet::KEY_CHAR, "G_key_22", NULL}, {KeySet::KEY_CHAR, "G_key_23", NULL},
    {KeySet::KEY_CHAR, "G_key_24", NULL}, {KeySet::KEY_CHAR, "G_key_25", NULL},
    {KeySet::KEY_CHAR, "G_key_26", NULL}, {KeySet::KEY_CHAR, "G_key_27", NULL},
    {KeySet::KEY_CHAR, "G_key_28", NULL}, {KeySet::KEY_CHAR, "G_key_29", NULL},
    {KeySet::KEY_CHAR, "G_key_30", NULL}, {KeySet::KEY_CHAR, "G_key_31", NULL},
    {KeySet::KEY_CHAR, "G_key_32", NULL}, {KeySet::KEY_CHAR, "G_key_33", NULL},
    {KeySet::KEY_CHAR, "G_key_34", NULL}, {KeySet::KEY_CHAR, "G_key_35", NULL},
    {KeySet::KEY_CHAR, "G_key_36", NULL}, {KeySet::KEY_CHAR, "G_key_37", NULL},
    {KeySet::KEY_CHAR, "G_key_38", NULL}, {KeySet::KEY_CHAR, "G_key_39", NULL},
    {KeySet::KEY_CHAR, "G_key_40", NULL}, {KeySet::KEY_CHAR, "G_key_41", NULL},
    {KeySet::KEY_CHAR, "G_key_42", NULL}, {KeySet::KEY_CHAR, "G_key_43", NULL},
    {KeySet::KEY_CHAR, "G_key_44", NULL}, {KeySet::KEY_SPACE, "G_key_Spc", "qwerty_space"},
    {KeySet::KEY_BACKSPACE, "G_key_Bsp", "qwerty_backspace"}, {KeySet::KEY_ENTER, "G_key_Ent", NULL},
    {KeySet::KEY_SWITCH, "G_key_Cps", "qwerty_caps"}, {KeySet::KEY_5, "G_key_Sft", "qwerty_shift"},
    {KeySet::KEY_6, "G_romanKey_00", "qwerty_eiji"}, {KeySet::KEY_6, "G_romanKey_01", "qwerty_hiragana"},
    {KeySet::KEY_6, "G_romanKey_02", "qwerty_katakana"}, {KeySet::KEY_END, NULL, NULL},};
// the pane animations of the keys: n0s1 with the press, i0 with the disabling
// 0x00896D00
const s32 s_QwertySubAnimSets[2] = {0, 5};
// the caps and shift keys: n0s1 with the press, t0s1 while they are on
// 0x00896D08
const s32 s_QwertyToggleSubAnimSets[2] = {0, 3};
// the roman keys: n0s1 with the press, s1t0 with the decision, i0 with the disabling
// 0x008984A8
const s32 s_QwertyRadioSubAnimSets[3] = {0, 2, 5};

// the key layout with a text that is never shown (the text boxes get a space)
const wchar_t* const SPACE_TEXT = L" ";
// the keyboards with more forbidden characters (see sys::g_Unknown95F0EB)
const u8 KEYBOARD_KIND_1 = 17;
const u8 KEYBOARD_KIND_2 = 18;

// (inline) the text of the label in swkbd.msbt
const wchar_t* GetText(const char* label)
{
    return static_cast<const wchar_t*>(LMS_GetTextByLabel(sys::g_pMessage, label));
}

// (inline) the character is a dead key of the keyboard of the region (an accent waiting for its letter)
bool IsDeadKey(u16 c)
{
    switch (GetRegion()) {
    case event::REGION_AMERICA_SPANISH:
    case event::REGION_EUROPE_SPANISH:
        return c == 0xB4 || c == 0xA8;
    case event::REGION_AMERICA_FRENCH:
    case event::REGION_EUROPE_FRENCH:
        return c == 0xB4 || c == '`' || c == '^' || c == 0xA8;
    case event::REGION_EUROPE_GERMAN:
        return c == 0xB4 || c == '`';
    default:
        return false;
    }
}
} // namespace

// 0x0095F170
const char* QwertyKeySet::s_LayoutName = "Keytop_qwerty";
// 0x0095F174
const char* QwertyKeySet::s_AnimN0S1 = "n0s1";
// 0x0095F178
const char* QwertyKeySet::s_AnimS1T0 = "s1t0";
// 0x0095F17C
const char* QwertyKeySet::s_AnimT0S1 = "t0s1";
// 0x0095F180
const char* QwertyKeySet::s_AnimI0 = "i0";
// the characters of the key sets
// 0x0095F300
const char* QwertyKeySet::s_CharLabels[QWERTY_LABEL_NUM] = {
    "qwerty_keytop",      "qwerty_keytop_c",  "qwerty_keytop_s",    "qwerty_keytop_jp",
    "qwerty_keytop_jp_c", "qwerty_keytop_ru", "qwerty_keytop_ru_c", "qwerty_keytop_ru_s",
};

// 0x004F8004 slot 0x14
void QwertyKeySet::SetVisible(bool visible)
{
    for (s32 i = 0; i < m_KeyNum; i++) {
        if (visible) {
            const bool isKeyVisible = m_IsKeyVisible[i][m_Label];
            m_pLayout->SetGroupVisible(s_QwertyKeys[i].groupName, isKeyVisible);
            m_pButtons[i]->SetActive(isKeyVisible);
        } else {
            m_pButtons[i]->SetActive(false);
        }
    }
    m_pLayout->SetEnabled(visible);
}

// 0x004F80D4 slot 0x24
s32 QwertyKeySet::GetFirstLabel(bool isJapanese)
{
    return isJapanese ? LABEL_JP_LOWER : LABEL_LOWER;
}

// 0x004F80E4
void QwertyKeySet::SetRomajiMode(s32 mode)
{
    if (mode == 0) {
        sys::TextManager* textManager = sys::g_pTextManager;
        if (textManager->m_CompositionLength > 0) {
            textManager->EndCellPhone(true);
            textManager->Commit(sys::g_Unknown95F0E8);
            textManager->m_IsSelecting = false;
            textManager->m_SelectionAnchor = textManager->m_Cursor;
            textManager->m_Unknown0x90 = -1;
        }
        sys::g_KeyboardSetting.kanaMode = mode;
        static_cast<RadioButtonEx*>(m_pButtons[m_RomanKeys[0]])->Select(false);
        if (m_Label == LABEL_JP_LOWER) {
            SetLabel(LABEL_LOWER);
        } else if (m_Label == LABEL_JP_CAPS) {
            SetLabel(LABEL_CAPS);
        }
        return;
    }
    sys::g_KeyboardSetting.kanaMode = mode;
    if (mode == 1) {
        static_cast<RadioButtonEx*>(m_pButtons[m_RomanKeys[1]])->Select(false);
    } else if (mode == 2) {
        static_cast<RadioButtonEx*>(m_pButtons[m_RomanKeys[2]])->Select(false);
    }
    if (m_Label == LABEL_LOWER) {
        SetLabel(LABEL_JP_LOWER);
    } else if (m_Label == LABEL_CAPS) {
        SetLabel(LABEL_JP_CAPS);
    }
}

// 0x004F8238 slot 0x08
bool QwertyKeySet::Initialize(s32 label)
{
    // the animations of the key panes ("" none)
    // 0x0095F2E8 (guard 0x0095F198)
    static const char* s_QwertyAnimNames[6] = {s_AnimN0S1, "", s_AnimS1T0, s_AnimT0S1, "", s_AnimI0};
    s32 romanKeyNum = 0;
    s32 charIndices[QWERTY_LABEL_NUM] = {};
    u32 paneAnimNums[6] = {};
    sead::Heap* heap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    lyt::AnmObject* paneAnims[6][detail::PANE_ANIM_MAX];
    char name[64];
    nw::ut::FormatString(name, sizeof(name), "%s.bclyt", s_LayoutName);
    m_pLayout = new (heap, 4) lyt::ObjectEx(0, 500, NULL);
    m_pLayout->Build(m_pArcReader, name, 0xDC00);
    m_pLayout->SetText(SPACE_TEXT, "T_key_Tra", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_Spc", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_BspJP", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_romanKey_00", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_romanKey_01", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_romanKey_02", false, 0, 0);
    for (s32 i = 0; i < 6; i++) {
        if (s_QwertyAnimNames[i][0] != '\0') {
            paneAnimNums[i] = detail::CreatePaneAnimations(m_pLayout, s_LayoutName, s_QwertyAnimNames[i], paneAnims[i]);
        }
    }
    for (s32 key = 0; key < m_KeyNum; key++) {
        const s32 kind = detail::GetKeyKind<KEY_7 + 2, KEY_7 + 1>(s_QwertyKeys, m_KeyNum, key);
        const KeyEntry* entry = &s_QwertyKeys[key];
        switch (kind) {
        case KEY_CHAR:
        case KEY_SPACE:
        case KEY_ENTER:
        case KEY_BACKSPACE:
        case KEY_6:
        case KEY_7:
        case KEY_7 + 1: {
            if (kind == KEY_6) {
                RadioButtonEx::Description desc;
                desc.pLayout = m_pLayout;
                desc.groupName = entry->groupName;
                desc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_KEY;
                desc.sounds[ButtonBase::SOUND_DECIDE] = SE_SYS_SWK_TOGGLE_ON;
                desc.group = 'QKS0';
                lyt::AnmObject** const dsts[3] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DECIDE],
                                                  desc.subAnims[ButtonBase::ANIM_DISABLE]};
                detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_QwertyRadioSubAnimSets, paneAnims, paneAnimNums);
                m_pButtons[key] = new (heap, 4) RadioButtonEx(desc);
            } else {
                ButtonEx::Description desc;
                desc.pLayout = m_pLayout;
                desc.groupName = entry->groupName;
                lyt::AnmObject** const dsts[2] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DISABLE]};
                detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_QwertySubAnimSets, paneAnims, paneAnimNums);
                switch (kind) {
                case KEY_CHAR:
                case KEY_ENTER:
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
                case KEY_7 + 1:
                    m_pButtons[key] = new (heap, 4) ButtonEx(desc);
                    break;
                }
            }
            // the labels and characters of the key sets
            NrmKey::Description keyDesc;
            keyDesc.labelNum = QWERTY_LABEL_NUM;
            keyDesc.pButton = static_cast<ButtonEx*>(m_pButtons[key]);
            wchar_t charTexts[QWERTY_LABEL_NUM][2];
            for (s32 i = 0; i < QWERTY_LABEL_NUM; i++) {
                if (kind == KEY_CHAR) {
                    const u16 c = GetText(s_CharLabels[i])[charIndices[i]];
                    if (c == 0) {
                        m_IsKeyEnabled[key][i] = false;
                        continue;
                    }
                    if (c == ' ') {
                        m_IsKeyEnabled[key][i] = false;
                        charIndices[i]++;
                        continue;
                    }
                    bool isForbidden = false;
                    for (s32 j = 0; j < sys::g_ForbiddenCharNum; j++) {
                        if (sys::g_ForbiddenChars[j] == c) {
                            isForbidden = true;
                            break;
                        }
                    }
                    if (isForbidden || sys::IsDigitChar(c)) {
                        m_IsKeyEnabled[key][i] = false;
                    }
                    charTexts[i][0] = c;
                    charTexts[i][1] = 0;
                    keyDesc.labels[i] = charTexts[i];
                    keyDesc.chars[i] = c;
                    charIndices[i]++;
                } else if (entry->labelName != NULL) {
                    const wchar_t* text = GetText(entry->labelName);
                    keyDesc.labels[i] = text;
                    if (kind == KEY_SPACE) {
                        // the ideographic space in the Japanese key sets
                        keyDesc.chars[i] = (i == LABEL_JP_LOWER || i == LABEL_JP_CAPS) ? 0x3000 : ' ';
                    } else if (kind == KEY_BACKSPACE) {
                        // the icon of the other language is hidden
                        if (text[0] != 0) {
                            m_pLayout->FindPane("P_Key_BspIcon")->m_Alpha = 0;
                        } else {
                            m_pLayout->FindPane("P_Key_BspIconJP")->m_Alpha = 0;
                        }
                    } else if (kind == KEY_6) {
                        // the roman keys only exist in Japan
                        if (GetRegionIndex() != 0) {
                            m_IsKeyVisible[key][i] = false;
                        } else if (sys::g_KeyboardParam._unknown0x000 == 1 || sys::g_KeyboardParam._unknown0x000 == 3) {
                            m_IsKeyEnabled[key][i] = false;
                            m_IsKeyVisible[key][i] = false;
                        }
                    }
                } else if (kind == KEY_ENTER) {
                    keyDesc.chars[i] = '\n';
                    if (!sys::g_KeyboardParam.isNewlineAllowed) {
                        m_IsKeyEnabled[key][i] = false;
                    }
                } else if (kind == KEY_7) {
                    if (GetRegionIndex() == 1 || GetRegionIndex() == 2) {
                        m_IsKeyEnabled[key][i] = false;
                    } else {
                        m_IsKeyVisible[key][i] = false;
                    }
                }
            }
            const char* textBoxName = detail::FindLabelTextBox(m_pLayout, entry->groupName);
            if (textBoxName != NULL) {
                keyDesc.textBoxName = textBoxName;
            }
            m_pKeys[key] = new (heap, 4) NrmKey(keyDesc);
            break;
        }
        case KEY_SWITCH:
        case KEY_5: {
            TglButton::Description desc;
            desc.pLayout = m_pLayout;
            desc.groupName = entry->groupName;
            lyt::AnmObject** const dsts[2] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.onSubAnims[ButtonBase::ANIM_PRESS]};
            detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_QwertyToggleSubAnimSets, paneAnims, paneAnimNums);
            m_pButtons[key] = new (heap, 4) TglButton(desc);
            TglKey::Description keyDesc;
            keyDesc.labelNum = QWERTY_LABEL_NUM;
            keyDesc.pButton = static_cast<ButtonEx*>(m_pButtons[key]);
            for (s32 i = 0; i < QWERTY_LABEL_NUM; i++) {
                if (entry->labelName != NULL) {
                    keyDesc.labels[i] = GetText(entry->labelName);
                }
            }
            // the icon of the key is hidden while it has a text
            if (kind == KEY_SWITCH) {
                if (keyDesc.labels[0][0] != 0) {
                    m_pLayout->FindPane("P_key_CpsIcon")->m_Alpha = 0;
                }
            } else if (keyDesc.labels[0][0] != 0) {
                m_pLayout->FindPane("P_key_SftIcon")->m_Alpha = 0;
            }
            const char* textBoxName = detail::FindLabelTextBox(m_pLayout, entry->groupName);
            if (textBoxName != NULL) {
                keyDesc.textBoxName = textBoxName;
            }
            m_pKeys[key] = new (heap, 4) TglKey(keyDesc);
            break;
        }
        }
        switch (kind) {
        case KEY_ENTER:
            m_pEnterButton = static_cast<TchOnButton*>(m_pButtons[key]);
            break;
        case KEY_SWITCH:
            m_CapsKey = key;
            break;
        case KEY_5:
            m_ShiftKey = key;
            break;
        case KEY_6:
            m_RomanKeys[romanKeyNum] = key;
            romanKeyNum++;
            break;
        }
    }
    // these keyboards have no space, caps and shift keys and show their kind
    const u8 keyboard = sys::g_Unknown95F0EB;
    if (keyboard == KEYBOARD_KIND_1 || keyboard == KEYBOARD_KIND_2) {
        for (s32 key = 0; key < m_KeyNum; key++) {
            const s32 kind = detail::GetKeyKind<KEY_7 + 2, KEY_7 + 1>(s_QwertyKeys, m_KeyNum, key);
            if (kind == KEY_SWITCH || kind == KEY_5 || kind == KEY_SPACE) {
                std::memset(m_IsKeyVisible[key], false, sizeof(m_IsKeyVisible[key]));
                std::memset(m_IsKeyEnabled[key], false, sizeof(m_IsKeyEnabled[key]));
            }
        }
        nw::ut::FormatString(name, sizeof(name), "%s_kind.bclan", s_LayoutName);
        m_pKindAnim = m_pLayout->CreateAnmObject(name, "G_kind_00");
        m_pKindAnim->ApplyFrame(m_pKindAnim->m_EndFrame);
    }
    SetLabel(label);
    SetRomajiMode(sys::g_KeyboardSetting.kanaMode);
    m_pLayout->m_State = lyt::Object::STATE_CHANGED;
    return true;
}

// 0x004F93D4 slot 0x0C
void QwertyKeySet::Update()
{
    for (s32 i = 0; i < KEY_MAX; i++) {
        if (m_pKeys[i] != NULL) {
            m_pKeys[i]->Update();
        }
    }
    sys::TextManager* textManager = sys::g_pTextManager;
    bool isEnterEnabled;
    if (sys::g_KeyboardParam.isNewlineAllowed) {
        std::memset(m_IsKeyEnabled[ENTER_KEY], true, sizeof(m_IsKeyEnabled[ENTER_KEY]));
        isEnterEnabled = true;
    } else {
        std::memset(m_IsKeyEnabled[ENTER_KEY], false, sizeof(m_IsKeyEnabled[ENTER_KEY]));
        isEnterEnabled = textManager->m_Length - textManager->m_CompositionLength < textManager->m_Length;
    }
    if (isEnterEnabled) {
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
}

// 0x004F9520 slot 0x1C
void QwertyKeySet::SetLabel(s32 label)
{
    KeySet::SetLabel(label);
    if (IsVisible()) {
        SetVisible(true);
    }
    bool isCaps = false;
    bool isShift = false;
    switch (label) {
    case LABEL_LOWER:
        if (sys::g_KeyboardSetting.kanaMode != 0) {
            SetRomajiMode(0);
        }
        break;
    case LABEL_CAPS:
        isCaps = true;
        if (sys::g_KeyboardSetting.kanaMode != 0) {
            SetRomajiMode(0);
        }
        break;
    case LABEL_SHIFT:
    case LABEL_RU_SHIFT:
        isShift = true;
        break;
    case LABEL_JP_LOWER:
        if (sys::g_KeyboardSetting.kanaMode == 0) {
            SetRomajiMode(1);
        }
        break;
    case LABEL_JP_CAPS:
        isCaps = true;
        if (sys::g_KeyboardSetting.kanaMode == 0) {
            SetRomajiMode(1);
        }
        break;
    case LABEL_RU_LOWER:
        break;
    case LABEL_RU_CAPS:
        isCaps = true;
        break;
    }
    static_cast<TglButton*>(m_pButtons[m_CapsKey])->SetOn(isCaps);
    static_cast<TglButton*>(m_pButtons[m_ShiftKey])->SetOn(isShift);
    for (s32 i = 0; i < m_KeyNum; i++) {
        const bool isEnabled = m_IsKeyEnabled[i][label];
        if (!isEnabled || m_pButtons[i]->IsEnabled() != isEnabled) {
            m_pButtons[i]->SetEnabled(isEnabled, true, false);
        }
    }
}

// 0x004F96E8 slot 0x20
lyt::ObjectEx* QwertyKeySet::GetLayout()
{
    return m_pLayout;
}

// 0x004F96F0 slot 0x18
bool QwertyKeySet::IsVisible()
{
    return m_pLayout->m_IsEnabled;
}

// 0x004F9700 slot 0x10
void QwertyKeySet::OnEvent(Widget* sender, s32 event)
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
        const s32 kanaMode = sys::g_KeyboardSetting.kanaMode;
        switch (detail::GetKeyKind<KEY_7 + 2, KEY_7 + 1>(s_QwertyKeys, m_KeyNum, key)) {
        case KEY_CHAR:
        case KEY_SPACE:
        case KEY_ENTER: {
            u32 romajiMode = 0;
            if (kanaMode == 1) {
                romajiMode = 1;
            } else if (kanaMode == 2) {
                romajiMode = 2;
            }
            const u16 c = static_cast<NrmKey*>(m_pKeys[key])->GetChar();
            ButtonBase* button = m_pButtons[key];
            if (textManager->InputChar(c, romajiMode, IsDeadKey(c))) {
                m_IsHandled = true;
            } else if (textManager->m_IsFull == true) {
                // the text is full: the key goes back
                button->Reset(false);
            }
            break;
        }
        case KEY_BACKSPACE:
            if (textManager->Backspace(false)) {
                m_IsHandled = true;
            }
            break;
        case KEY_SWITCH:
            // caps
            switch (m_Label) {
            case LABEL_CAPS:
            case LABEL_JP_CAPS:
                SetLabel(kanaMode == 0 ? LABEL_LOWER : LABEL_JP_LOWER);
                return;
            case LABEL_RU_CAPS:
                SetLabel(LABEL_RU_LOWER);
                return;
            case LABEL_SHIFT:
            case LABEL_RU_SHIFT:
                m_pKeys[m_ShiftKey]->GetButton()->Reset(false);
                break;
            }
            if (m_Label == LABEL_RU_LOWER || m_Label == LABEL_RU_SHIFT) {
                SetLabel(LABEL_RU_CAPS);
            } else {
                SetLabel(sys::g_KeyboardSetting.kanaMode == 0 ? LABEL_CAPS : LABEL_JP_CAPS);
            }
            return;
        case KEY_5:
            // shift
            switch (m_Label) {
            case LABEL_SHIFT:
                SetLabel(kanaMode == 0 ? LABEL_LOWER : LABEL_JP_LOWER);
                return;
            case LABEL_RU_SHIFT:
                SetLabel(LABEL_RU_LOWER);
                return;
            case LABEL_CAPS:
            case LABEL_JP_CAPS:
            case LABEL_RU_CAPS:
                m_pKeys[m_CapsKey]->GetButton()->Reset(false);
                break;
            }
            if (m_Label == LABEL_RU_LOWER || m_Label == LABEL_RU_CAPS) {
                SetLabel(LABEL_RU_SHIFT);
            } else {
                SetLabel(LABEL_SHIFT);
            }
            return;
        case KEY_6:
            for (s32 i = 0; i < ROMAN_KEY_NUM; i++) {
                if (m_RomanKeys[i] == key) {
                    SetRomajiMode(i);
                    return;
                }
            }
            return;
        case KEY_7:
            m_pButtons[key]->SetActive(true);
            return;
        default:
            return;
        }
        // a shift ends with the character
        if (m_Label == LABEL_SHIFT) {
            m_pKeys[m_ShiftKey]->GetButton()->Reset(false);
            SetLabel(sys::g_KeyboardSetting.kanaMode == 0 ? LABEL_LOWER : LABEL_JP_LOWER);
        } else if (m_Label == LABEL_RU_SHIFT) {
            m_pKeys[m_ShiftKey]->GetButton()->Reset(false);
            SetLabel(LABEL_RU_LOWER);
        }
        return;
    }
}

// 0x004F9CD8
QwertyKeySet::QwertyKeySet(ssys::ma::lyt::ArcResAccReader* arcReader)
    : KeySet(arcReader), m_pLayout(NULL), m_KeyNum(0), m_CapsKey(-1), m_ShiftKey(-1), m_pEnterButton(NULL), m_pKindAnim(NULL)
{
    std::memset(m_pButtons, 0, sizeof(m_pButtons));
    m_KeyNum = 53;
    for (s32 i = 0; i < m_KeyNum; i++) {
        m_pButtons[i] = NULL;
        std::memset(m_IsKeyVisible[i], true, sizeof(m_IsKeyVisible[i]));
        std::memset(m_IsKeyEnabled[i], true, sizeof(m_IsKeyEnabled[i]));
    }
    for (s32 i = 0; i < ROMAN_KEY_NUM; i++) {
        m_RomanKeys[i] = -1;
    }
}

// 0x004F9E5C slot 0x00
// 0x004F9D9C (deleting dtor)
QwertyKeySet::~QwertyKeySet()
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
