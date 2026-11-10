#include "gui/dTchOnButtonSp.h"

#include "Other/dController.h"
#include "Other/dTouchState.h"
#include "lyt/dTextArea.h"
#include "nw/lyt/lyt_Bounding.h"
#include "sys/dSwkbd.h"
#include "sys/dTextManager.h"

namespace gui {
// 0x004F7FAC slot 0x2C
void TchOnButtonSp::UpdatePressed()
{
    if (Controller::IsHold(Controller::TOUCH) && IsTouched()) {
        return;
    }
    PlayAnimation(ANIM_DECIDE);
    PlaySound(SOUND_DECIDE);
    m_State = STATE_DECIDED;
}

// 0x004FB000 slot 0x28
void TchOnButtonSp::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (!Controller::IsTrig(Controller::TOUCH) || !sys::IsTouchInPane(m_pBounding)) {
        return;
    }
    sys::TextManager* textManager = sys::g_pTextManager;
    textManager->EndCellPhone(true);
    textManager->Commit(false);
    s32 index;
    const bool isLineEnd = m_pTextArea->GetIndexAtPosition(&index, &g_TouchPosition);
    textManager->SetCursor(index, true, true);
    m_pTextArea->SetLineEnd(isLineEnd);
    if (m_Desc2.sounds[SOUND_PRESS] > 0) {
        SoundMgr::PlaySound(static_cast<SeID>(m_Desc2.sounds[SOUND_PRESS]), 0.0f);
    }
    m_IsHolding = true;
    g_IsWidgetHolding = true;
    m_State = STATE_DECIDED;
    SendEvent(this, EVENT_DECIDE, 0);
}

// 0x004FB0E4
TchOnButtonSp::TchOnButtonSp(bool isBottom, const Description& desc, lyt::TextArea* textArea)
    : TchOnButton(desc), m_pTextArea(textArea), m_Desc2(desc)
{
    if (isBottom) {
        m_pBounding = nw::lyt::DynamicCast<nw::lyt::Bounding>(desc.pLayout->FindPane("B_text_B"));
    } else {
        m_pBounding = nw::lyt::DynamicCast<nw::lyt::Bounding>(desc.pLayout->FindPane("B_text_T"));
    }
}

// 0x004FB214 slot 0x00
// 0x004FB1E4 (deleting dtor)
TchOnButtonSp::~TchOnButtonSp()
{
    // (only the base is destroyed)
}
} // namespace gui
