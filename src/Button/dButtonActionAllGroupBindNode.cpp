#include "Button/dButtonActionAllGroupBindNode.h"

// 0x00343530 slot 0x24 (name after libgarden)
void ButtonActionAllGroupBindNode::BeginSelectOk()
{
    if (m_IsOkAlt == 1) {
        BindAnimation(m_TouchOkAlt);
        BindAnimation(m_SelectOkAlt);
    } else {
        BindAnimation(m_TouchOk);
        BindAnimation(m_SelectOk);
    }
    PlayOkSound();
}

// 0x00343594 slot 0x0C (name after libgarden)
void ButtonActionAllGroupBindNode::ResetSelect()
{
    Select(false);
}

// 0x0034359C slot 0x2C (name after libgarden)
bool ButtonActionAllGroupBindNode::IsOkOngoing() const
{
    if (m_IsOkAlt == 1) {
        if (m_TouchOkAlt.GetAnimTransform() != NULL && !m_TouchOkAlt.IsDone()) {
            return true;
        }
        if (m_SelectOkAlt.GetAnimTransform() != NULL && !m_SelectOkAlt.IsDone()) {
            return true;
        }
        return false;
    }
    if (m_TouchOk.GetAnimTransform() != NULL && !m_TouchOk.IsDone()) {
        return true;
    }
    if (m_SelectOk.GetAnimTransform() != NULL && !m_SelectOk.IsDone()) {
        return true;
    }
    return false;
}

// 0x00343640 slot 0x28 (name after libgarden)
void ButtonActionAllGroupBindNode::EndSelectOk()
{
    if (m_IsOkAlt == 1) {
        UnbindAnimation(m_TouchOkAlt);
        UnbindAnimation(m_SelectOkAlt);
    } else {
        UnbindAnimation(m_TouchOk);
        UnbindAnimation(m_SelectOk);
    }
}

// 0x00343730 slot 0x30 (name after libgarden)
void ButtonActionAllGroupBindNode::DoOk()
{
    if (m_IsOkAlt == 1) {
        if (m_TouchOkAlt.GetAnimTransform() != NULL) {
            m_TouchOkAlt.IncreaseFrame();
        }
        if (m_SelectOkAlt.GetAnimTransform() != NULL) {
            m_SelectOkAlt.IncreaseFrame();
        }
    } else {
        if (m_TouchOk.GetAnimTransform() != NULL) {
            m_TouchOk.IncreaseFrame();
        }
        if (m_SelectOk.GetAnimTransform() != NULL) {
            m_SelectOk.IncreaseFrame();
        }
    }
    m_pLayout->m_IsAnimationChanged = 1;
}

// 0x003437A0 slot 0x1C (name after libgarden)
void ButtonActionAllGroupBindNode::TouchSelect(bool touch, bool sound)
{
    if (touch && sound) {
        PlaySound();
    }
    m_IsPressed = touch;
    Animate(m_Touch, touch);
    Animate(m_Select, touch);
}

// 0x003437FC (name after the RTTI)
ButtonActionAllGroupBindNode::ButtonActionAllGroupBindNode() : m_IsOkAlt(0)
{
}

// 0x003438D0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
// 0x0034382C (deleting dtor)
ButtonActionAllGroupBindNode::~ButtonActionAllGroupBindNode()
{
    m_TouchOkAlt.SetAnimTransform(NULL);
    m_SelectOkAlt.SetAnimTransform(NULL);
}
