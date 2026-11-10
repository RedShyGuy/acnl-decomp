#include "gui/dKanaKeySet.h"

#include <cstring>
#include <new>

#include "Sound/dSoundLabel.h"
#include "gui/dInputButton.h"
#include "gui/dNrmKey.h"
#include "gui/dRadioButtonEx.h"
#include "gui/dRepeatInputButton.h"
#include "gui/gui_KeySetUtil.h"
#include "libms/lms_Api.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "sys/dSwkbd.h"
#include "sys/dTextManager.h"
#include "sys/sys_Extern.h"

namespace gui {
namespace {
// the keys of the layout
// 0x00897318
const KeySet::KeyEntry s_KanaKeys[KeySet::KEY_MAX] = {
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
    {KeySet::KEY_CHAR, "G_key_44", NULL}, {KeySet::KEY_CHAR, "G_key_45", NULL},
    {KeySet::KEY_CHAR, "G_key_46", NULL}, {KeySet::KEY_CHAR, "G_key_47", NULL},
    {KeySet::KEY_CHAR, "G_key_48", NULL}, {KeySet::KEY_CHAR, "G_key_49", NULL},
    {KeySet::KEY_CHAR, "G_key_bar", "kana50_bar"}, {KeySet::KEY_SPACE, "G_key_Spc", "kana50_space"},
    {KeySet::KEY_BACKSPACE, "G_key_Bsp", "kana50_backspace"}, {KeySet::KEY_ENTER, "G_key_Ent", NULL},
    {KeySet::KEY_5, "G_key_dakuten", "kana50_dakuten"}, {KeySet::KEY_5, "G_key_handakuten", "kana50_handakuten"},
    {KeySet::KEY_5, "G_key_sutegana", "kana50_sutegana"}, {KeySet::KEY_SWITCH, "G_key_hira", "kana50_hiragana"},
    {KeySet::KEY_SWITCH, "G_key_kata", "kana50_katakana"}, {KeySet::KEY_END, NULL, NULL},};
// the pane animations of the keys: n0s1 with the press, i0 with the disabling
// 0x00896CF8
const s32 s_KanaSubAnimSets[2] = {0, 5};
// the radio keys: n0s1 with the press, s1t0 with the decision
// 0x00896CF0
const s32 s_KanaRadioSubAnimSets[2] = {0, 2};
// (inline) the text of the label in swkbd.msbt
const wchar_t* GetText(const char* label)
{
    return static_cast<const wchar_t*>(LMS_GetTextByLabel(sys::g_pMessage, label));
}
} // namespace

// 0x0095F160
const char* KanaKeySet::s_LayoutName = "Keytop_50on";
// 0x0095F164
const char* KanaKeySet::s_AnimN0S1 = "n0s1";
// 0x0095F168
const char* KanaKeySet::s_AnimS1T0 = "s1t0";
// 0x0095F16C
const char* KanaKeySet::s_AnimI0 = "i0";
// the kana of the key sets
// 0x0095F1D0
const char* KanaKeySet::s_CharLabels[KANA_LABEL_NUM] = {"kana50_keytop_h", "kana50_keytop_k"};

// 0x004F56FC slot 0x14
void KanaKeySet::SetVisible(bool visible)
{
    for (s32 i = 0; i < m_KeyNum; i++) {
        if (visible) {
            const bool isKeyVisible = m_IsKeyVisible[i][m_Label];
            m_pLayout->SetGroupVisible(s_KanaKeys[i].groupName, isKeyVisible);
            m_pButtons[i]->SetActive(isKeyVisible);
        } else {
            m_pButtons[i]->SetActive(false);
        }
    }
    m_pLayout->SetEnabled(visible);
}

// 0x004F57CC
void KanaKeySet::UpdateConvertKeys(bool isDakuten, bool isHandakuten, bool isSmall)
{
    bool isChanged = false;
    if (m_pButtons[m_DakutenKey]->IsEnabled() != isDakuten) {
        m_pButtons[m_DakutenKey]->SetEnabled(isDakuten, true, false);
        isChanged = true;
    }
    if (m_pButtons[m_HandakutenKey]->IsEnabled() != isHandakuten) {
        m_pButtons[m_HandakutenKey]->SetEnabled(isHandakuten, true, false);
        isChanged = true;
    }
    if (m_pButtons[m_SmallKey]->IsEnabled() != isSmall) {
        m_pButtons[m_SmallKey]->SetEnabled(isSmall, true, false);
        isChanged = true;
    }
    if (isChanged) {
        m_pLayout->m_State = lyt::Object::STATE_CHANGED;
    }
}

// 0x004F58D4
void KanaKeySet::UpdateConvertKeysForCursor()
{
    bool isDakuten = false;
    bool isHandakuten = false;
    bool isSmall = false;
    sys::TextManager* textManager = sys::g_pTextManager;
    if (textManager->m_Cursor > 0) {
        const u16 c = textManager->m_pBuffer[textManager->m_Cursor - 1];
        isDakuten = sys::HasKanaForm(c, &sys::KanaVariant::dakuten);
        isHandakuten = sys::HasKanaForm(c, &sys::KanaVariant::handakuten);
        isSmall = sys::HasKanaForm(c, &sys::KanaVariant::small);
    }
    UpdateConvertKeys(isDakuten, isHandakuten, isSmall);
}

// 0x004F5B08 slot 0x08
bool KanaKeySet::Initialize(s32 label)
{
    // the animations of the key panes ("" none)
    // 0x0095F2D0 (guard 0x0095F194)
    static const char* s_KanaAnimNames[6] = {s_AnimN0S1, "", s_AnimS1T0, "", "", s_AnimI0};
    u32 paneAnimNums[6] = {};
    s32 charIndices[KANA_LABEL_NUM] = {};
    sead::Heap* heap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    lyt::AnmObject* paneAnims[6][detail::PANE_ANIM_MAX];
    char name[64];
    nw::ut::FormatString(name, sizeof(name), "%s.bclyt", s_LayoutName);
    m_pLayout = new (heap, 4) lyt::ObjectEx(0, 500, NULL);
    m_pLayout->Build(m_pArcReader, name, 0xE800);
    nw::lyt::TextBox* textBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(m_pLayout->FindPane("T_key_Tra"));
    if (textBox != NULL) {
        m_pLayout->SetMessage("kana50_conv", textBox, NULL, false, 0);
    }
    m_pLayout->m_Layout.SetLayer(151);
    for (s32 i = 0; i < 6; i++) {
        if (s_KanaAnimNames[i][0] != '\0') {
            paneAnimNums[i] = detail::CreatePaneAnimations(m_pLayout, s_LayoutName, s_KanaAnimNames[i], paneAnims[i]);
        }
    }
    for (s32 key = 0; key < m_KeyNum; key++) {
        const s32 kind = detail::GetKeyKind<KEY_7 + 1, KEY_6>(s_KanaKeys, m_KeyNum, key);
        const KeyEntry* entry = &s_KanaKeys[key];
        if (kind == KEY_SWITCH) {
            RadioButtonEx::Description desc;
            desc.pLayout = m_pLayout;
            desc.groupName = entry->groupName;
            desc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_KEY;
            desc.sounds[ButtonBase::SOUND_DECIDE] = SE_SYS_SWK_TOGGLE_ON;
            desc.group = 'KKS0';
            lyt::AnmObject** const dsts[2] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DECIDE]};
            detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_KanaRadioSubAnimSets, paneAnims, paneAnimNums);
            m_pButtons[key] = new (heap, 4) RadioButtonEx(desc);
        } else {
            ButtonEx::Description desc;
            desc.pLayout = m_pLayout;
            desc.groupName = entry->groupName;
            lyt::AnmObject** const dsts[2] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DISABLE]};
            detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_KanaSubAnimSets, paneAnims, paneAnimNums);
            switch (kind) {
            case KEY_CHAR:
            case KEY_ENTER:
            case KEY_5:
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
            case KEY_6:
                m_pButtons[key] = new (heap, 4) ButtonEx(desc);
                break;
            }
        }
        // the labels and characters of the key sets
        NrmKey::Description keyDesc;
        keyDesc.labelNum = KANA_LABEL_NUM;
        keyDesc.pButton = static_cast<ButtonEx*>(m_pButtons[key]);
        wchar_t charTexts[KANA_LABEL_NUM][2];
        for (s32 i = 0; i < KANA_LABEL_NUM; i++) {
            if (kind == KEY_CHAR && entry->labelName == NULL) {
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
                if (kind == KEY_CHAR) {
                    keyDesc.chars[i] = text[0];
                } else if (kind == KEY_SPACE) {
                    // the ideographic space
                    keyDesc.chars[i] = 0x3000;
                }
            } else if (kind == KEY_ENTER) {
                keyDesc.chars[i] = '\n';
                if (!sys::g_KeyboardParam.isNewlineAllowed) {
                    m_IsKeyEnabled[key][i] = false;
                }
            }
        }
        const char* textBoxName = detail::FindLabelTextBox(m_pLayout, entry->groupName);
        if (textBoxName != NULL) {
            keyDesc.textBoxName = textBoxName;
        }
        m_pKeys[key] = new (heap, 4) NrmKey(keyDesc);
        if (kind == KEY_SWITCH) {
            if (std::strcmp(entry->groupName, "G_key_hira") == 0) {
                m_HiraganaKey = key;
            } else if (std::strcmp(entry->groupName, "G_key_kata") == 0) {
                m_KatakanaKey = key;
            }
        } else if (kind == KEY_5) {
            if (std::strcmp(entry->groupName, "G_key_dakuten") == 0) {
                m_DakutenKey = key;
            } else if (std::strcmp(entry->groupName, "G_key_handakuten") == 0) {
                m_HandakutenKey = key;
            } else if (std::strcmp(entry->groupName, "G_key_sutegana") == 0) {
                m_SmallKey = key;
            }
        } else if (kind == KEY_ENTER) {
            m_pEnterButton = static_cast<TchOnButton*>(m_pButtons[key]);
        }
    }
    SetLabel(label);
    UpdateConvertKeys(false, false, false);
    m_pLayout->m_State = lyt::Object::STATE_CHANGED;
    return true;
}

// 0x004F67DC slot 0x0C
void KanaKeySet::Update()
{
    for (s32 i = 0; i < KEY_MAX; i++) {
        if (m_pKeys[i] != NULL) {
            m_pKeys[i]->Update();
        }
    }
    UpdateConvertKeysForCursor();
    sys::TextManager* textManager = sys::g_pTextManager;
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
}

// 0x004F6914 slot 0x1C
void KanaKeySet::SetLabel(s32 label)
{
    sys::g_KeyboardSetting.kanaKeySet = label;
    KeySet::SetLabel(label);
    if (IsVisible()) {
        SetVisible(true);
    }
    static_cast<RadioButtonEx*>(m_pButtons[label == 0 ? m_HiraganaKey : m_KatakanaKey])->Select(false);
    for (s32 i = 0; i < m_KeyNum; i++) {
        const bool isEnabled = m_IsKeyEnabled[i][label];
        if (!isEnabled || m_pButtons[i]->IsEnabled() != isEnabled) {
            m_pButtons[i]->SetEnabled(isEnabled, true, false);
        }
    }
}

// 0x004F6A24 slot 0x20
lyt::ObjectEx* KanaKeySet::GetLayout()
{
    return m_pLayout;
}

// 0x004F6A2C slot 0x18
bool KanaKeySet::IsVisible()
{
    return m_pLayout->m_IsEnabled;
}

// 0x004F6A3C slot 0x10
void KanaKeySet::OnEvent(Widget* sender, s32 event)
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
        switch (detail::GetKeyKind<KEY_7 + 1, KEY_6>(s_KanaKeys, m_KeyNum, key)) {
        case KEY_CHAR:
        case KEY_SPACE:
        case KEY_ENTER: {
            ButtonBase* button = m_pButtons[key];
            if (textManager->InputChar(static_cast<NrmKey*>(m_pKeys[key])->GetChar(), 0, false)) {
                break;
            }
            // the text is full: the key goes back
            if (textManager->m_IsFull == true) {
                button->Reset(false);
            }
            return;
        }
        case KEY_BACKSPACE:
            if (!textManager->Backspace(false)) {
                return;
            }
            break;
        case KEY_SWITCH:
            if (m_HiraganaKey == key) {
                SetLabel(0);
            } else if (m_KatakanaKey == key) {
                SetLabel(1);
            }
            UpdateConvertKeys(false, false, false);
            break;
        case KEY_5: {
            u16 c = 0;
            if (m_DakutenKey == key) {
                c = g_KanaConvertChars[0];
            } else if (m_HandakutenKey == key) {
                c = g_KanaConvertChars[1];
            } else if (m_SmallKey == key) {
                c = g_KanaConvertChars[2];
            }
            InputButton* button = static_cast<InputButton*>(m_pButtons[key]);
            if (c != 0 && !textManager->InputChar(c, 0, false)) {
                // nothing to convert
                SoundMgr::PlaySound(SE_SYS_SWK_ERROR, button->m_Desc.pLayout->m_SoundPan);
                button->m_IsSilent = true;
                return;
            }
            if (button->m_Desc.sounds[ButtonBase::SOUND_PRESS] > 0) {
                SoundMgr::PlaySound(static_cast<SeID>(button->m_Desc.sounds[ButtonBase::SOUND_PRESS]), button->m_Desc.pLayout->m_SoundPan);
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

// 0x004F6CFC
KanaKeySet::KanaKeySet(ssys::ma::lyt::ArcResAccReader* arcReader)
    : KeySet(arcReader), m_pLayout(NULL), m_KeyNum(0), m_HiraganaKey(-1), m_KatakanaKey(-1), m_DakutenKey(-1), m_HandakutenKey(-1),
      m_SmallKey(-1), m_pEnterButton(NULL)
{
    std::memset(m_pButtons, 0, sizeof(m_pButtons));
    m_KeyNum = 59;
    for (s32 i = 0; i < m_KeyNum; i++) {
        m_pButtons[i] = NULL;
        for (s32 j = 0; j < KANA_LABEL_NUM; j++) {
            m_IsKeyVisible[i][j] = true;
            m_IsKeyEnabled[i][j] = true;
        }
    }
}

// 0x004F6E68 slot 0x00
// 0x004F6DA8 (deleting dtor)
KanaKeySet::~KanaKeySet()
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
