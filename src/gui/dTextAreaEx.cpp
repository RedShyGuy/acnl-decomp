#include "gui/dTextAreaEx.h"

#include <cstdio>
#include <new>

#include "Bs/dBsMenuMgr.h"
#include "Sound/dSoundLabel.h"
#include "gui/dEditAreaEx.h"
#include "gui/dRepeatButtonEx2.h"
#include "gui/dTchOnButtonSp.h"
#include "nw/lyt/lyt_GroupContainer.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"

namespace gui {
// the group of the field
// 0x0095F188
const char* TextAreaEx::s_EditGroupName = "G_edit_00";
// the groups of the arrow keys in the key layout
// 0x0095F1B8
const char* TextAreaEx::s_ArrowGroupNames[BUTTON_NUM] = {"G_btn_T", "G_btn_B"};

// 0x004F6F20
void TextAreaEx::DeleteBottomTouch()
{
    if (m_pTouchButtons[BUTTON_BOTTOM] != NULL) {
        delete m_pTouchButtons[BUTTON_BOTTOM];
        m_pTouchButtons[BUTTON_BOTTOM] = NULL;
    }
}

// 0x004F6F4C
void TextAreaEx::ResetArrows()
{
    m_pArrowButtons[BUTTON_TOP]->Reset(false);
    m_pArrowButtons[BUTTON_BOTTOM]->Reset(false);
}

// 0x004F6F88
void TextAreaEx::SetArrowsActive(bool active)
{
    for (s32 i = 0; i < BUTTON_NUM; i++) {
        if (m_pArrowButtons[i] != NULL) {
            m_pArrowButtons[i]->SetActive(active);
        }
    }
}

// 0x004F6FC8 slot 0x14
void TextAreaEx::Initialize(lyt::TextAreaEx* textArea, lyt::Object* keyLayout)
{
    textArea->GetLayoutName(); // (not used)
    const char* arrowName = "Key_arrow";
    // (the initialization image is at 0x00896D10)
    const s32 lineSounds[BUTTON_NUM] = {SE_SYS_SWK_INPUT_LINE_UP, SE_SYS_SWK_INPUT_LINE_DOWN};
    m_pHeap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    m_pTextArea = textArea;
    m_pKeyLayout = keyLayout;

    {
        ButtonEx::Description desc;
        desc.pLayout = m_pTextArea;
        desc.pKeyLayout = m_pKeyLayout;
        desc.groupName = s_EditGroupName;
        desc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_KEY;
        desc.sounds[ButtonBase::SOUND_SLIDE_OUT] = -1;
        m_pEditAreaEx = new (m_pHeap, 4) EditAreaEx(desc, GetTextAreaEx(), m_pKeyLayout);
    }
    {
        ButtonEx::Description desc;
        desc.pLayout = m_pKeyLayout;
        desc.groupName = "G_btn_T";
        desc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_INPUT_LINE_UP;
        m_pTouchButtons[BUTTON_TOP] = new (m_pHeap, 4) TchOnButtonSp(false, desc, m_pTextArea);
        desc.groupName = "G_btn_B";
        desc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_INPUT_LINE_DOWN;
        m_pTouchButtons[BUTTON_BOTTOM] = new (m_pHeap, 4) TchOnButtonSp(true, desc, m_pTextArea);
    }

    for (s32 i = 0; i < BUTTON_NUM; i++) {
        if (m_pKeyLayout->m_Layout.GetLayout().GetGroupContainer()->FindGroupByName(s_ArrowGroupNames[i]) == NULL) {
            continue;
        }
        RepeatButtonEx::Description repeatDesc;
        repeatDesc.repeatDelay = 10;
        for (s32 j = 0; j < RepeatButtonEx::STAGE_NUM; j++) {
            repeatDesc.repeatIntervals[j] = 3;
        }
        repeatDesc.pLayout = m_pKeyLayout;
        repeatDesc.groupName = s_ArrowGroupNames[i];
        repeatDesc.sounds[ButtonBase::SOUND_PRESS] = SE_SYS_SWK_KEY;
        repeatDesc.sounds[ButtonBase::SOUND_DECIDE] = lineSounds[i];
        repeatDesc.repeatKeys = i == BUTTON_TOP ? BsMenuMgr::INPUT_DPAD_UP : BsMenuMgr::INPUT_DPAD_DOWN;
        char animName[64];
        std::sprintf(animName, "%s_select.bclan", arrowName);
        repeatDesc.anims[ButtonBase::ANIM_PRESS] = m_pKeyLayout->CreateAnmObject(animName, s_ArrowGroupNames[i]);
        std::sprintf(animName, "%s_select_ok.bclan", arrowName);
        repeatDesc.anims[ButtonBase::ANIM_SLIDE_OUT] = m_pKeyLayout->CreateAnmObject(animName, s_ArrowGroupNames[i]);
        std::sprintf(animName, "%s_touch.bclan", arrowName);
        repeatDesc.exAnims[ButtonEx::ANIM_EX_5 - ButtonBase::ANIM_NUM] = m_pKeyLayout->CreateAnmObject(animName, s_ArrowGroupNames[i]);
        std::sprintf(animName, "%s_touch_ok.bclan", arrowName);
        repeatDesc.exAnims[ButtonEx::ANIM_EX_6 - ButtonBase::ANIM_NUM] = m_pKeyLayout->CreateAnmObject(animName, s_ArrowGroupNames[i]);
        std::sprintf(animName, "%s_ng.bclan", arrowName);
        repeatDesc.anims[ButtonBase::ANIM_DISABLE] = m_pKeyLayout->CreateAnmObject(animName, s_ArrowGroupNames[i]);
        repeatDesc.isDecideOnPress = false;
        repeatDesc.repeatIntervals[1] = 2;
        repeatDesc.stageRepeats[0] = 20;
        RepeatButtonEx2* button = new (m_pHeap, 4) RepeatButtonEx2(repeatDesc);
        m_pArrowButtons[i] = button;
        button->SetEnabled(false, true, false);
        m_IsScrollEnabled[i] = false;
    }
}

// 0x004F73C0 slot 0x0C
void TextAreaEx::Update()
{
    s32 column;
    const s32 line = GetCursorLine(sys::g_pTextManager, sys::GetLineManager(), &column);
    const s32* page = &GetTextAreaEx()->m_Page;
    // the text has more lines than the area scrolls through
    const bool hasMore = sys::GetLineManager()->GetLineStartChecked(m_pTextArea->GetScrollLineNum()) >= 0;
    // (a button changes when the wanted state is the same in two frames)
    if (m_pArrowButtons[BUTTON_TOP] != NULL) {
        const bool isUp = (hasMore && line > 0) || *page > 0;
        if (m_IsScrollEnabled[BUTTON_TOP] == isUp && m_pArrowButtons[BUTTON_TOP]->IsEnabled() != isUp) {
            m_pArrowButtons[BUTTON_TOP]->SetEnabled(isUp, true, false);
            m_pKeyLayout->m_State = lyt::Object::STATE_CHANGED;
        }
        m_IsScrollEnabled[BUTTON_TOP] = isUp;
    }
    if (m_pArrowButtons[BUTTON_BOTTOM] != NULL) {
        const bool hasNextPage = GetPageNum() != 0 && GetPageNum() - 1 > static_cast<u32>(*page);
        const bool isDown = (hasMore && sys::GetLineManager()->GetLineStartChecked(line + 1) >= 0) || hasNextPage;
        if (m_IsScrollEnabled[BUTTON_BOTTOM] == isDown && m_pArrowButtons[BUTTON_BOTTOM]->IsEnabled() != isDown) {
            m_pArrowButtons[BUTTON_BOTTOM]->SetEnabled(isDown, true, false);
            m_pKeyLayout->m_State = lyt::Object::STATE_CHANGED;
        }
        m_IsScrollEnabled[BUTTON_BOTTOM] = isDown;
    }
    // the upper part is touched to go back to the first page, the lower part to leave it
    if (m_pTouchButtons[BUTTON_TOP] != NULL) {
        const bool isEnabled = *page != 0;
        if (m_pTouchButtons[BUTTON_TOP]->IsEnabled() != isEnabled) {
            m_pTouchButtons[BUTTON_TOP]->SetEnabled(isEnabled, true, false);
        }
    }
    if (m_pTouchButtons[BUTTON_BOTTOM] != NULL) {
        const bool isEnabled = *page == 0;
        if (m_pTouchButtons[BUTTON_BOTTOM]->IsEnabled() != isEnabled) {
            m_pTouchButtons[BUTTON_BOTTOM]->SetEnabled(isEnabled, true, false);
        }
    }
}

// 0x004F7650 slot 0x00
// 0x004F763C slot 0x04 (deleting dtor)
TextAreaEx::~TextAreaEx()
{
    for (s32 i = 0; i < BUTTON_NUM; i++) {
        if (m_pArrowButtons[i] != NULL) {
            delete m_pArrowButtons[i];
            m_pArrowButtons[i] = NULL;
        }
        if (m_pTouchButtons[i] != NULL) {
            delete m_pTouchButtons[i];
            m_pTouchButtons[i] = NULL;
        }
    }
    if (m_pEditAreaEx != NULL) {
        delete m_pEditAreaEx;
        m_pEditAreaEx = NULL;
    }
}
} // namespace gui
