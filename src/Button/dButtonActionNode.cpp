#include "Button/dButtonActionNode.h"
#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuMgr.h"
#include "Other/dTouchState.h"
#include "Sound/dSoundMgr.h"
#include "nn/math/math_ARMv6.h"
#include "nw/lyt/lyt_Api.h"
#include "nw/lyt/lyt_Pane.h"

namespace {
// the pan of a button sound: the x position of the pane (its horizontal centre) over the bottom screen, -1 to 1
inline f32 GetPan(const nw::lyt::Pane* pane)
{
    if (pane == NULL) {
        return 0.0f;
    }
    f32 x;
    u8 basePosition = pane->GetBasePositionH();
    if (basePosition == 0) {
        x = pane->m_GlbMtx.m[0][3] + pane->m_Size.width * 0.5f;
    } else {
        x = pane->m_GlbMtx.m[0][3];
        if (basePosition == 2) {
            x -= pane->m_Size.width * 0.5f;
        }
    }
    return x * (1.0f / 320.0f) * 2.0f;
}
} // namespace

// 0x002B6FCC | libgarden [tier A]
void ButtonActionNode::Initialize(ButtonActionNode::Info const& info)
{
    m_pPane = info.pane;
    m_pGroup = info.group;
    m_Position = info.pos;
    m_Input = info.input;
    m_Index = info.index;
    m_pCursorCallback = info.cursorCallback;
}

// 0x002B7040 slot 0x24 (name after libgarden)
void ButtonActionNode::BeginSelectOk()
{
    BindAnimation(m_TouchOk);
    BindAnimation(m_SelectOk);
    PlayOkSound();
}

// 0x002B7070 slot 0x2C (name after libgarden)
bool ButtonActionNode::IsOkOngoing() const
{
    if (m_TouchOk.GetAnimTransform() != NULL && !m_TouchOk.IsDone()) {
        return true;
    }
    if (m_SelectOk.GetAnimTransform() != NULL && !m_SelectOk.IsDone()) {
        return true;
    }
    return false;
}

// 0x002B70C4 (name is ours)
void ButtonActionNode::SetAnimationFrame(ssys::ma::lyt::Animation& anim, f32 frame)
{
    if (anim.GetAnimTransform() != NULL) {
        anim.SetFrame(frame);
        m_pLayout->m_IsAnimationChanged = 1;
    }
}

// 0x002B70F8 slot 0x28 (name after libgarden)
void ButtonActionNode::EndSelectOk()
{
    UnbindAnimation(m_TouchOk);
    UnbindAnimation(m_SelectOk);
}

// 0x002B7174 slot 0x30 (name after libgarden)
void ButtonActionNode::DoOk()
{
    if (m_TouchOk.GetAnimTransform() != NULL) {
        m_TouchOk.IncreaseFrame();
    }
    if (m_SelectOk.GetAnimTransform() != NULL) {
        m_SelectOk.IncreaseFrame();
    }
    m_pLayout->m_IsAnimationChanged = 1;
}

// 0x002B71AC slot 0x34 (name after libgarden)
void ButtonActionNode::Animate(ssys::ma::lyt::Animation& anim, bool set)
{
    BindAnimation(anim);
    if (set && anim.GetAnimTransform() != NULL) {
        anim.SetLastFrame();
        m_pLayout->m_IsAnimationChanged = 1;
    }
    m_pLayout->Animate();
    UnbindAnimation(anim);
}

// 0x002B7234 slot 0x20 (name after libgarden)
void ButtonActionNode::Select(bool select, bool sound)
{
    if (select && sound) {
        PlaySound();
    }
    m_IsPressed = select;
    Animate(m_Select, select);
}

// 0x002B7278 (name is ours)
void ButtonActionNode::BindAnimation(ssys::ma::lyt::Animation& anim)
{
    if (anim.GetAnimTransform() != NULL && m_pLayout != NULL) {
        if (m_pGroup != NULL) {
            m_pLayout->BindAnimation(&anim, m_pGroup, false);
        } else {
            m_pLayout->BindAnimation(&anim);
        }
        anim.SetFrame(0.0f);
        m_pLayout->m_IsAnimationChanged = 1;
    }
}

// 0x002B72E0 slot 0x10 (name after libgarden)
void ButtonActionNode::FUN_002b72e0()
{
    // empty (in the original too)
}

// 0x002B72E4 slot 0x0C | libgarden
void ButtonActionNode::ResetSelect()
{
    Select(false);
}

// 0x002B72EC (libgarden)
void ButtonActionNode::Select(bool set)
{
    if (set) {
        m_IsPressed = true;
        Animate(m_Touch, true);
        Animate(m_Select, true);
    } else {
        TouchSelect(false, false);
    }
}

// 0x002B7350 slot 0x1C (name after libgarden)
void ButtonActionNode::TouchSelect(bool touch, bool sound)
{
    if (touch && sound) {
        PlaySound();
    }
    m_IsPressed = touch;
    Animate(m_Touch, touch);
    Animate(m_Select, touch);
}

// 0x002B73AC | libgarden [tier A]
ButtonActionNode::ButtonActionNode()
    : m_pPane(NULL), m_pGroup(NULL), m_Position(0.0f, 0.0f), m_pLayout(NULL), m_pCursorCallback(NULL), m_Input(0),
      m_Index(0), m_SeOk(static_cast<SeID>(0)), m_Se(SE_SYS_BTN_ACTIVE), m_SoundValue(-1), m_IsOkRequested(0),
      m_IsPressed(false), m_Unknown0xD8(0), m_IsActive(true), m_OkSoundEnabled(false), m_SoundEnabled(true),
      m_SoundMode(false), m_IsRepeat(false)
{
}

// 0x002B74C4 slot 0x00 | libgarden
// 0x002B7454 (deleting dtor)
ButtonActionNode::~ButtonActionNode()
{
    m_Touch.SetAnimTransform(NULL);
    m_Select.SetAnimTransform(NULL);
    m_TouchOk.SetAnimTransform(NULL);
    m_SelectOk.SetAnimTransform(NULL);
}

// 0x0071D944 (name is ours)
void ButtonActionNode::MoveHandCursor()
{
    nn::math::MTX34 mtx;
    nn::math::ARMv6::MTX34MultAsm(&mtx, &m_pLayout->m_InvViewMtx, &m_pPane->m_GlbMtx);
    nn::math::VEC3 pos;
    pos.x = mtx.m[0][3] + m_Position.x;
    pos.y = mtx.m[1][3] + m_Position.y;
    pos.z = mtx.m[2][3];
    if (m_pCursorCallback != NULL) {
        m_pCursorCallback(&pos, m_Index);
        return;
    }
    if (BsMenuMgr::Get()->m_pBsHandCursor->m_IsEnabled) {
        BsHandCursor* hand = BsMenuMgr::Get()->m_pBsHandCursor;
        hand->SetPosition(&pos);
        hand->m_Position.x = pos.x;
        hand->m_Position.y = pos.y;
    }
}

// 0x0071D9F8 slot 0x3C (name after libgarden)
void ButtonActionNode::PlaySound()
{
    if (!m_SoundEnabled) {
        return;
    }
    f32 pan = GetPan(m_pPane);
    if (m_SoundMode) {
        SoundMgr::SPlaySystemSound(m_Se, pan);
    } else {
        SoundMgr::PlaySound(m_Se, pan);
    }
}

// 0x0071DAE0 slot 0x14 (name after libgarden)
bool ButtonActionNode::IsTouch() const
{
    nn::math::VEC2 screen(g_TouchState.lastX, g_TouchState.lastY);
    nn::math::VEC2 pos;
    m_pLayout->ScreenToLayout(screen, &pos);
    return nw::lyt::IsContain(m_pPane, pos);
}

// 0x0071DB38 slot 0x18 (name after libgarden)
bool ButtonActionNode::IsCursorHover() const
{
    if (!BsMenuMgr::Get()->m_HandMode) {
        return false;
    }
    nn::math::VEC2 pos = BsMenuMgr::Get()->m_pBsHandCursor->m_Position;
    return nw::lyt::IsContain(m_pPane, pos);
}

// 0x0071DB7C slot 0x08 (name after libgarden)
bool ButtonActionNode::IsActivated() const
{
    return m_IsActive;
}

// 0x0071DB84 slot 0x38 (name after libgarden)
void ButtonActionNode::PlayOkSound()
{
    if (!m_OkSoundEnabled) {
        return;
    }
    if (m_SoundValue != -1) {
        SoundMgr::SetNextSoundValue(m_SoundValue);
    }
    SoundMgr::PlaySound(m_SeOk, GetPan(m_pPane));
}
