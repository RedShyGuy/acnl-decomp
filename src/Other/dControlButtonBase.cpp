#include "Other/dControlButtonBase.h"

#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuMgr.h"
#include "font/dMgr.h"

// the states with their names (debug; the first one means waiting); the machine starts idle
// 0x00858D48
const ControlButtonBase::StateInfo ControlButtonBase::s_StateInfos[4] = {
    {"待機", &ControlButtonBase::Enter_Idle, &ControlButtonBase::State_Idle},
    {"Button IN", &ControlButtonBase::Enter_In, &ControlButtonBase::State_In},
    {"button OUT", &ControlButtonBase::Enter_Out, &ControlButtonBase::State_Out},
    {"select", &ControlButtonBase::Enter_Select, &ControlButtonBase::State_Select},
};

// 0x002CD198
void ControlButtonBase::State_Idle()
{
    // empty (in the original too)
}

// 0x002CD19C
void ControlButtonBase::Enter_Idle()
{
    // empty (in the original too)
}

// 0x002CD1B4
void ControlButtonBase::SetButtonInactive(s32 index, bool inactive)
{
    m_pButtons[index].m_IsActive = !inactive;
}

// 0x002CD1D0
void ControlButtonBase::SetButtonsVisible(bool visible)
{
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetVisible(visible);
        m_pButtons[i].m_IsActive = visible;
    }
}

// 0x002CD224
void ControlButtonBase::Enter_Select()
{
    // empty (in the original too)
}

// 0x002CD23C
void ControlButtonBase::SetButtonDisabled(s32 index, bool disabled)
{
    m_pButtons[index].m_IsActive = !disabled;
    m_pButtons[index].SetDisabled(disabled);
}

// 0x002CD270
void ControlButtonBase::State_In()
{
    bool isBusy = false;
    for (s32 i = 0; i < m_ButtonNum; i++) {
        isBusy |= m_pButtons[i].IsBusy();
    }
    if (isBusy) {
        return;
    }
    SetFlags(DISABLED, false);
    Change(&ControlButtonBase::State_Select);
}

// 0x002CD2E0
void ControlButtonBase::Enter_In()
{
    for (s32 i = m_FirstVisible; i < m_ButtonNum; i++) {
        m_pButtons[i].StartIn();
    }
    Reset();
    m_pCurrent = NULL;
}

// 0x002CD338
void ControlButtonBase::State_Out()
{
    bool isBusy = false;
    for (s32 i = 0; i < m_ButtonNum; i++) {
        isBusy |= m_pButtons[i].IsBusy();
    }
    if (isBusy) {
        return;
    }
    Change(&ControlButtonBase::State_Idle);
}

// 0x002CD39C
void ControlButtonBase::Enter_Out()
{
    for (s32 i = m_FirstVisible; i < m_ButtonNum; i++) {
        m_pButtons[i].StartOut();
    }
    BsMenuMgr* mgr = BsMenuMgr::Get();
    if (mgr->m_HandMode && BsMenuMgr::s_pInstance->m_HandGrabTimer == 9) {
        mgr->m_pBsHandCursor->Disable();
        mgr->m_HandMode = 0;
    }
    SetFlags(DISABLED, true);
}

// 0x002CD428
bool ControlButtonBase::Initialize(const char* fontName, const nw::font::Font* font, const char* fontName2,
                                   const nw::font::Font* font2)
{
    if (m_IsInitialized) {
        return true;
    }
    if (m_ButtonNum > 0) {
        bool isOk = true;
        for (s32 i = 0; i < m_ButtonNum; i++) {
            isOk &= m_pButtons[i].Setup(fontName, font, fontName2, font2, true);
        }
        if (!isOk) {
            return false;
        }
    }
    for (s32 i = 0; i < m_ButtonNum; i++) {
        AddNode(&m_pButtons[i], false);
    }
    Start("ControlButtonBase", s_StateInfos, 4);
    m_CursorTimer = 9;
    SetFlags(ENABLE_BUTTON_SHORTCUT, true);
    m_Flags2 = ENABLE_TOUCH | ENABLE_CURSOR | ENABLE_NODE_SHORTCUT | CAN_RELEASE;
    m_IsInitialized = true;
    return true;
}

// 0x002CD5A0
bool ControlButtonBase::Initialize()
{
    const nw::font::Font* ruby = font::Mgr::s_pInstance->GetFont(font::GARDEN_RUBY);
    const char* rubyName = font::Mgr::s_pInstance->GetFontName(font::GARDEN_RUBY)->getCString();
    const nw::font::Font* message = font::Mgr::s_pInstance->GetFont(font::GARDEN_MSG_16);
    const char* messageName = font::Mgr::s_pInstance->GetFontName(font::GARDEN_MSG_16)->getCString();
    return Initialize(messageName, message, rubyName, ruby);
}

// 0x002CD630
void ControlButtonBase::SetIn()
{
    for (s32 i = m_FirstVisible; i < m_ButtonNum; i++) {
        m_pButtons[i].SetIn();
    }
    Change(&ControlButtonBase::State_Select);
}

// 0x002CD6AC
void ControlButtonBase::Finalize()
{
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].Finalize();
    }
}

// 0x002CD6EC
void ControlButtonBase::SetOut()
{
    for (s32 i = m_FirstVisible; i < m_ButtonNum; i++) {
        m_pButtons[i].SetOut();
    }
    Change(&ControlButtonBase::State_Idle);
}

// 0x002CD748 slot 0x08 | slot vf_0x08 of ButtonActionControl
void ControlButtonBase::Update()
{
    for (s32 i = m_FirstVisible; i < m_ButtonNum; i++) {
        m_pButtons[i].FUN_002b72e0();
    }
    Call();
}

// 0x002CD810 (name is ours)
void ControlButtonBase::AnimateButtons()
{
    for (s32 i = m_FirstVisible; i < m_ButtonNum; i++) {
        m_pButtons[i].AnimateLayout();
    }
}

// 0x002CD88C
bool ControlButtonBase::StartIn()
{
    Change(&ControlButtonBase::State_In);
    return true;
}

// 0x002CD8AC (name is ours)
void ControlButtonBase::SetButtonOkSound(SeID se, s32 index)
{
    m_pButtons[index].m_SeOk = se;
    m_pButtons[index].m_OkSoundEnabled = true;
}

// 0x002CD8D8
bool ControlButtonBase::StartOut()
{
    Change(&ControlButtonBase::State_Out);
    return true;
}

// 0x002CD944 (name is ours)
void ControlButtonBase::SetDisabled(bool disabled)
{
    SetFlags(DISABLED, disabled);
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetDisabled(disabled);
    }
}

// 0x002CD99C (name is ours)
void ControlButtonBase::SetVisibleNum(s32 num)
{
    m_FirstVisible = m_ButtonNum - num;
    for (s32 i = 0; i < m_ButtonNum; i++) {
        m_pButtons[i].SetVisible(m_FirstVisible <= i);
        m_pButtons[i].m_IsActive = m_FirstVisible <= i;
    }
}

// 0x002CDA10 (name after the RTTI)
ControlButtonBase::ControlButtonBase(CommonButtonSingle* buttons, s32 buttonNum)
    : m_pButtons(buttons), m_ButtonNum(buttonNum), m_FirstVisible(0), m_IsInitialized(false)
{
}

// 0x002F8184 slot 0x00 | slot vf_0x00 of ssys::st::List
// 0x002CDA78 (deleting dtor)
// 0x007760A4 (thunk)
// 0x00776090 (deleting thunk)
ControlButtonBase::~ControlButtonBase()
{
    // empty (in the original too)
}

// 0x002F7BA4
void ControlButtonBase::State_Select()
{
    ButtonActionControl::Update();
}
