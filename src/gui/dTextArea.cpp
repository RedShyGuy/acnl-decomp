#include "gui/dTextArea.h"

#include <cstdio>
#include <new>

#include "gui/dEditArea.h"
#include "gui/dRepeatButtonEx.h"
#include "nw/lyt/lyt_GroupContainer.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"

namespace gui {
// the group of the field
// 0x0095F184
const char* TextArea::s_GroupName = "G_textArea";
// the groups of the scroll buttons
// 0x0095F1B0
const char* TextArea::s_ScrollGroupNames[SCROLL_BUTTON_NUM] = {"G_scrollUp", "G_scrollDown"};

// 0x005007F8 slot 0x00
// 0x00500788 slot 0x04 (deleting dtor)
TextArea::~TextArea()
{
    for (s32 i = 0; i < SCROLL_BUTTON_NUM; i++) {
        if (m_pScrollButtons[i] != NULL) {
            delete m_pScrollButtons[i];
            m_pScrollButtons[i] = NULL;
        }
    }
    if (m_pEditArea != NULL) {
        delete m_pEditArea;
        m_pEditArea = NULL;
    }
}

// 0x005001C4 slot 0x08
void TextArea::Initialize(lyt::TextArea* textArea)
{
    const char* layoutName = textArea->GetLayoutName();
    m_pHeap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    m_pTextArea = textArea;

    ButtonEx::Description desc;
    desc.pLayout = m_pTextArea;
    desc.groupName = s_GroupName;
    desc.sounds[ButtonBase::SOUND_PRESS] = -1;
    desc.sounds[ButtonBase::SOUND_SLIDE_OUT] = -1;
    desc.sounds[ButtonBase::SOUND_DECIDE] = -1;
    m_pEditArea = new (m_pHeap, 4) EditArea(desc, m_pTextArea);

    for (s32 i = 0; i < SCROLL_BUTTON_NUM; i++) {
        if (m_pTextArea->m_Layout.GetLayout().GetGroupContainer()->FindGroupByName(s_ScrollGroupNames[i]) == NULL) {
            continue;
        }
        RepeatButtonEx::Description repeatDesc;
        repeatDesc.pLayout = m_pTextArea;
        repeatDesc.groupName = s_ScrollGroupNames[i];
        char animName[64];
        std::sprintf(animName, "%s_n0s1.bclan", layoutName);
        repeatDesc.anims[ButtonBase::ANIM_PRESS] = m_pTextArea->CreateAnmObject(animName, s_ScrollGroupNames[i]);
        std::sprintf(animName, "%s_i0.bclan", layoutName);
        repeatDesc.anims[ButtonBase::ANIM_DISABLE] = m_pTextArea->CreateAnmObject(animName, s_ScrollGroupNames[i]);
        repeatDesc.isDecideOnPress = false;
        repeatDesc.repeatIntervals[1] = 2;
        repeatDesc.stageRepeats[0] = 20;
        RepeatButtonEx* button = new (m_pHeap, 4) RepeatButtonEx(repeatDesc);
        m_pScrollButtons[i] = button;
        button->SetEnabled(false, true, false);
        m_IsScrollEnabled[i] = false;
    }
}

// 0x00500420 slot 0x0C
void TextArea::Update()
{
    s32 column;
    const s32 line = GetCursorLine(sys::g_pTextManager, sys::GetLineManager(), &column);
    // the text has more lines than the area scrolls through
    const bool hasMore = sys::GetLineManager()->GetLineStartChecked(m_pTextArea->GetScrollLineNum()) >= 0;
    // (a button changes when the wanted state is the same in two frames)
    if (m_pScrollButtons[0] != NULL) {
        const bool isUp = hasMore && line > 0;
        if (m_IsScrollEnabled[0] == isUp && m_pScrollButtons[0]->IsEnabled() != isUp) {
            m_pScrollButtons[0]->SetEnabled(isUp, true, false);
        }
        m_IsScrollEnabled[0] = isUp;
    }
    if (m_pScrollButtons[1] != NULL) {
        const bool isDown = hasMore && sys::GetLineManager()->GetLineStartChecked(line + 1) >= 0;
        if (m_IsScrollEnabled[1] == isDown && m_pScrollButtons[1]->IsEnabled() != isDown) {
            m_pScrollButtons[1]->SetEnabled(isDown, true, false);
        }
        m_IsScrollEnabled[1] = isDown;
    }
}

// 0x00500590 slot 0x10
void TextArea::OnEvent(Widget* sender, s32 event)
{
    if (event != EVENT_DECIDE) {
        return;
    }
    if (!(sender != NULL && sender == m_pScrollButtons[0]) && sender != m_pScrollButtons[1]) {
        return;
    }
    sys::TextManager* text = sys::g_pTextManager;
    sys::LineManager* lines = sys::GetLineManager();
    s32 column;
    const s32 line = GetCursorLine(text, lines, &column);
    lyt::TextArea* textArea = m_pTextArea;
    const s32 top = textArea->m_TopLine;
    s32 relLine = line - top;
    if (sender == m_pScrollButtons[0]) {
        if (top > 0) {
            textArea->m_TopLine = top - 1;
        } else {
            relLine = 0;
        }
    } else if (sender == m_pScrollButtons[1]) {
        if (lines->GetLineStartChecked(textArea->GetScrollLineNum() + top) >= 0) {
            m_pTextArea->m_TopLine = top + 1;
        } else {
            relLine = m_pTextArea->GetScrollLineNum() - 1;
        }
    }
    MoveCursorToLine(text, lines, relLine, column);
}
} // namespace gui
