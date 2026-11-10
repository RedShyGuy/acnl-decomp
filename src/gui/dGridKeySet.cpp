#include "gui/dGridKeySet.h"

#include <cstdio>
#include <cstring>
#include <cwchar>
#include <new>

#include "gui/dInputButton.h"
#include "gui/dNrmKey.h"
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
// 0x00897018
const KeySet::KeyEntry s_GridKeys[KeySet::KEY_MAX] = {
    {KeySet::KEY_CHAR, "G_key_00", NULL},          {KeySet::KEY_CHAR, "G_key_01", NULL},
    {KeySet::KEY_CHAR, "G_key_02", NULL},          {KeySet::KEY_CHAR, "G_key_03", NULL},
    {KeySet::KEY_CHAR, "G_key_04", NULL},          {KeySet::KEY_CHAR, "G_key_05", NULL},
    {KeySet::KEY_CHAR, "G_key_06", NULL},          {KeySet::KEY_CHAR, "G_key_07", NULL},
    {KeySet::KEY_CHAR, "G_key_08", NULL},          {KeySet::KEY_CHAR, "G_key_09", NULL},
    {KeySet::KEY_CHAR, "G_key_10", NULL},          {KeySet::KEY_CHAR, "G_key_11", NULL},
    {KeySet::KEY_CHAR, "G_key_12", NULL},          {KeySet::KEY_CHAR, "G_key_13", NULL},
    {KeySet::KEY_CHAR, "G_key_14", NULL},          {KeySet::KEY_CHAR, "G_key_15", NULL},
    {KeySet::KEY_CHAR, "G_key_16", NULL},          {KeySet::KEY_CHAR, "G_key_17", NULL},
    {KeySet::KEY_CHAR, "G_key_18", NULL},          {KeySet::KEY_CHAR, "G_key_19", NULL},
    {KeySet::KEY_CHAR, "G_key_20", NULL},          {KeySet::KEY_CHAR, "G_key_21", NULL},
    {KeySet::KEY_CHAR, "G_key_22", NULL},          {KeySet::KEY_CHAR, "G_key_23", NULL},
    {KeySet::KEY_CHAR, "G_key_24", NULL},          {KeySet::KEY_CHAR, "G_key_25", NULL},
    {KeySet::KEY_CHAR, "G_key_26", NULL},          {KeySet::KEY_CHAR, "G_key_27", NULL},
    {KeySet::KEY_CHAR, "G_key_28", NULL},          {KeySet::KEY_CHAR, "G_key_29", NULL},
    {KeySet::KEY_CHAR, "G_key_30", NULL},          {KeySet::KEY_CHAR, "G_key_31", NULL},
    {KeySet::KEY_CHAR, "G_key_32", NULL},          {KeySet::KEY_CHAR, "G_key_33", NULL},
    {KeySet::KEY_CHAR, "G_key_34", NULL},          {KeySet::KEY_CHAR, "G_key_35", NULL},
    {KeySet::KEY_SPACE, "G_key_Spc", "grid_space"}, {KeySet::KEY_BACKSPACE, "G_key_Bsp", "grid_backspace"},
    {KeySet::KEY_ENTER, "G_key_Ent", NULL},        {KeySet::KEY_SWITCH, "G_key_Cps", NULL},
    {KeySet::KEY_END, NULL, NULL},
};
// the pane animations of the keys: n0s1 with the press, i0 with the disabling
// 0x00896CE8
const s32 s_GridSubAnimSets[2] = {0, 5};
// the shift key: also t0s1 while it is on
// 0x0089849C
const s32 s_ShiftSubAnimSets[3] = {0, 5, 3};

// the key layout with a text that is never shown (the text boxes get a space)
const wchar_t* const SPACE_TEXT = L" ";

// (inline) the text of the label in swkbd.msbt
const wchar_t* GetText(const char* label)
{
    return static_cast<const wchar_t*>(LMS_GetTextByLabel(sys::g_pMessage, label));
}
} // namespace

// 0x0095F150
const char* GridKeySet::s_LayoutName = "Keytop_grid";
// 0x0095F154
const char* GridKeySet::s_AnimN0S1 = "n0s1";
// 0x0095F158
const char* GridKeySet::s_AnimT0S1 = "t0s1";
// 0x0095F15C
const char* GridKeySet::s_AnimI0 = "i0";
// the characters of the key sets
// 0x0095F290
const char* GridKeySet::s_CharLabels[LABEL_NUM] = {
    "sign_keytop_00",   "sign_keytop_01",   "sign_keytop_02",   "sign_keytop_03",   "euro_keytop_00",   "euro_keytop_01",
    "euro_keytop_02",   "euro_keytop_03",   "euro_keytop_04",   "euro_keytop_05",   "euro_keytop_00_c", "euro_keytop_01_c",
    "euro_keytop_02_c", "euro_keytop_03_c", "euro_keytop_04_c", "euro_keytop_05_c",
};

// 0x004F40EC
bool GridKeySet::IsShiftedLabel(s32 label)
{
    return label >= 10 && label <= 15;
}

// 0x004F4100 slot 0x14
void GridKeySet::SetVisible(bool visible)
{
    for (s32 i = 0; i < m_KeyNum; i++) {
        if (visible) {
            const bool isKeyVisible = m_IsKeyVisible[i][m_Label];
            m_pLayout->SetGroupVisible(s_GridKeys[i].groupName, isKeyVisible);
            m_pButtons[i]->SetActive(isKeyVisible);
        } else {
            m_pButtons[i]->SetActive(false);
        }
    }
    m_pLayout->SetEnabled(visible);
}

// 0x004F41D0
s32 GridKeySet::GetShiftedLabel(s32 label)
{
    for (s32 i = 0; i < 6; i++) {
        if (s_ShiftLabels[i].label == label) {
            return s_ShiftLabels[i].shiftedLabel;
        }
    }
    return -1;
}

// 0x004F4224 slot 0x08
bool GridKeySet::Initialize(s32 label)
{
    // the animations of the key panes ("" none)
    // 0x0095F278 (guard 0x0095F190)
    static const char* s_GridAnimNames[6] = {s_AnimN0S1, "", "", s_AnimT0S1, "", s_AnimI0};
    sead::Heap* heap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    s32 charIndices[LABEL_NUM] = {};
    u32 paneAnimNums[6] = {};
    lyt::AnmObject* paneAnims[6][detail::PANE_ANIM_MAX];
    char name[64];
    nw::ut::FormatString(name, sizeof(name), "%s.bclyt", s_LayoutName);
    m_pLayout = new (heap, 4) lyt::ObjectEx(0, 500, NULL);
    m_pLayout->Build(m_pArcReader, name, 0xA700);
    m_pLayout->SetText(SPACE_TEXT, "T_key_Tra", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_Spc", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_BspJP", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_33", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_34", false, 0, 0);
    m_pLayout->SetText(SPACE_TEXT, "T_key_35", false, 0, 0);
    for (s32 i = 0; i < 6; i++) {
        if (s_GridAnimNames[i][0] != '\0') {
            paneAnimNums[i] = detail::CreatePaneAnimations(m_pLayout, s_LayoutName, s_GridAnimNames[i], paneAnims[i]);
        }
    }
    for (s32 key = 0; key < m_KeyNum; key++) {
        const s32 kind = detail::GetKeyKind<KEY_7, KEY_5>(s_GridKeys, m_KeyNum, key);
        const KeyEntry* entry = &s_GridKeys[key];
        switch (kind) {
        case KEY_CHAR:
        case KEY_SPACE:
        case KEY_ENTER:
        case KEY_BACKSPACE:
        case KEY_5: {
            ButtonEx::Description desc;
            desc.pLayout = m_pLayout;
            desc.groupName = entry->groupName;
            lyt::AnmObject** const dsts[2] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DISABLE]};
            detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_GridSubAnimSets, paneAnims, paneAnimNums);
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
            case KEY_5:
                m_pButtons[key] = new (heap, 4) ButtonEx(desc);
                break;
            }
            // the labels and characters of the key sets
            NrmKey::Description keyDesc;
            keyDesc.labelNum = LABEL_NUM;
            keyDesc.pButton = static_cast<ButtonEx*>(m_pButtons[key]);
            wchar_t charTexts[LABEL_NUM][2];
            for (s32 i = 0; i < LABEL_NUM; i++) {
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
                        keyDesc.chars[i] = ' ';
                    } else if (kind == KEY_BACKSPACE) {
                        // the icon of the other language is hidden
                        if (text[0] != 0) {
                            m_pLayout->FindPane("P_key_Bspicon")->m_Alpha = 0;
                        } else {
                            m_pLayout->FindPane("P_key_BspJPicon")->m_Alpha = 0;
                        }
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
            break;
        }
        case KEY_SWITCH: {
            TglButton::Description desc;
            desc.pLayout = m_pLayout;
            desc.groupName = entry->groupName;
            lyt::AnmObject** const dsts[3] = {desc.subAnims[ButtonBase::ANIM_PRESS], desc.subAnims[ButtonBase::ANIM_DISABLE],
                                              desc.onSubAnims[ButtonBase::ANIM_PRESS]};
            detail::FindSubAnimations(m_pLayout, entry->groupName, dsts, s_ShiftSubAnimSets, paneAnims, paneAnimNums);
            m_pButtons[key] = new (heap, 4) TglButton(desc);
            TglKey::Description keyDesc;
            keyDesc.labelNum = LABEL_NUM;
            keyDesc.pButton = static_cast<ButtonEx*>(m_pButtons[key]);
            for (s32 i = 0; i < LABEL_NUM; i++) {
                // a key set without a shifted one has no shift
                if (!IsShiftedLabel(i) && GetShiftedLabel(i) == -1) {
                    m_IsKeyEnabled[key][i] = false;
                }
                if (entry->labelName != NULL) {
                    keyDesc.labels[i] = GetText(entry->labelName);
                }
            }
            const char* textBoxName = detail::FindLabelTextBox(m_pLayout, entry->groupName);
            if (textBoxName != NULL) {
                keyDesc.textBoxName = textBoxName;
            }
            m_pKeys[key] = new (heap, 4) TglKey(keyDesc);
            break;
        }
        }
        if (kind == KEY_SWITCH) {
            m_ShiftKey = key;
        } else if (kind == KEY_ENTER) {
            m_pEnterButton = static_cast<TchOnButton*>(m_pButtons[key]);
        }
    }
    SetLabel(label);
    m_pLayout->m_State = lyt::Object::STATE_CHANGED;
    return true;
}

// 0x004F4F10 slot 0x0C
void GridKeySet::Update()
{
    for (s32 i = 0; i < KEY_MAX; i++) {
        if (m_pKeys[i] != NULL) {
            m_pKeys[i]->Update();
        }
    }
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

// 0x004F5040 slot 0x1C
void GridKeySet::SetLabel(s32 label)
{
    KeySet::SetLabel(label);
    if (IsVisible()) {
        SetVisible(true);
    }
    const bool isShifted = IsShiftedLabel(label);
    if (m_IsKeyEnabled[m_ShiftKey][label]) {
        static_cast<TglButton*>(m_pButtons[m_ShiftKey])->SetOn(isShifted);
    }
    for (s32 i = 0; i < m_KeyNum; i++) {
        const bool isEnabled = m_IsKeyEnabled[i][label];
        if (!isEnabled || m_pButtons[i]->IsEnabled() != isEnabled) {
            m_pButtons[i]->SetEnabled(isEnabled, true, false);
        }
    }
}

// 0x004F51B8
void GridKeySet::SetPage(s32 page, s32 pageNum)
{
    wchar_t text[6];
    if (pageNum >= 10) {
        std::swprintf(text, 6, L"%02d/%02d", (page + 1) % 100, pageNum % 100);
    } else {
        std::swprintf(text, 6, L"%d/%d", (page + 1) % 100, pageNum % 100);
    }
    m_pLayout->SetText(text, "T_page", false, 0, 0);
}

// 0x004F5280 slot 0x20
lyt::ObjectEx* GridKeySet::GetLayout()
{
    return m_pLayout;
}

// 0x004F5288 slot 0x18
bool GridKeySet::IsVisible()
{
    return m_pLayout->m_IsEnabled;
}

// 0x004F5298 slot 0x10
void GridKeySet::OnEvent(Widget* sender, s32 event)
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
        switch (detail::GetKeyKind<KEY_7, KEY_5>(s_GridKeys, m_KeyNum, key)) {
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
        case KEY_SWITCH: {
            s32 toggled = -1;
            if (IsShiftedLabel(m_Label)) {
                for (s32 i = 0; i < 6; i++) {
                    if (s_ShiftLabels[i].shiftedLabel == m_Label) {
                        toggled = s_ShiftLabels[i].label;
                        break;
                    }
                }
            } else {
                for (s32 i = 0; i < 6; i++) {
                    if (s_ShiftLabels[i].label == m_Label) {
                        toggled = s_ShiftLabels[i].shiftedLabel;
                        break;
                    }
                }
            }
            SetLabel(toggled);
            break;
        }
        default:
            return;
        }
        m_IsHandled = true;
        return;
    }
}

// 0x004F54E0
GridKeySet::GridKeySet(ssys::ma::lyt::ArcResAccReader* arcReader)
    : KeySet(arcReader), m_pLayout(NULL), m_KeyNum(0), m_ShiftKey(-1), m_pEnterButton(NULL)
{
    std::memset(m_pButtons, 0, sizeof(m_pButtons));
    m_KeyNum = 40;
    for (s32 i = 0; i < m_KeyNum; i++) {
        m_pButtons[i] = NULL;
        std::memset(m_IsKeyVisible[i], true, sizeof(m_IsKeyVisible[i]));
        std::memset(m_IsKeyEnabled[i], true, sizeof(m_IsKeyEnabled[i]));
    }
}

// 0x004F5650 slot 0x00
// 0x004F559C (deleting dtor)
GridKeySet::~GridKeySet()
{
    sead::Heap* heap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    for (s32 i = 0; i < m_KeyNum; i++) {
        ButtonBase* button = m_pButtons[i];
        if (button != NULL) {
            button->~ButtonBase();
            heap->free(button);
        }
    }
    lyt::ObjectEx* layout = m_pLayout;
    if (layout != NULL) {
        layout->~ObjectEx();
        heap->free(layout);
    }
}
} // namespace gui
