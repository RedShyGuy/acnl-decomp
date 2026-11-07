#include "Button/dButtonActionNodeEx.h"

// 0x002E0158 slot 0x24 (name after libgarden)
void ButtonActionNodeEx::BeginSelectOk()
{
    if (m_IsOkBack == 1) {
        BindAnimation(m_TouchOkBack);
        BindAnimation(m_SelectOkBack);
    } else {
        BindAnimation(m_TouchOk);
        BindAnimation(m_SelectOk);
    }
    PlayOkSound();
}

// 0x002E01BC slot 0x0C | libgarden
void ButtonActionNodeEx::ResetSelect()
{
    TouchSelect(false, false);
}

// 0x002E01D0 slot 0x2C (name after libgarden)
bool ButtonActionNodeEx::IsOkOngoing() const
{
    if (m_IsOkBack == 1) {
        if (m_TouchOkBack.GetAnimTransform() != NULL && !m_TouchOkBack.IsDone()) {
            return true;
        }
        if (m_SelectOkBack.GetAnimTransform() != NULL && !m_SelectOkBack.IsDone()) {
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

// 0x002E0274 slot 0x28 (name after libgarden)
void ButtonActionNodeEx::EndSelectOk()
{
    if (m_IsOkBack == 1) {
        UnbindAnimation(m_TouchOkBack);
        UnbindAnimation(m_SelectOkBack);
    } else {
        UnbindAnimation(m_TouchOk);
        UnbindAnimation(m_SelectOk);
    }
}

// 0x002E0364 slot 0x30 (name after libgarden)
void ButtonActionNodeEx::DoOk()
{
    if (m_IsOkBack == 1) {
        if (m_TouchOkBack.GetAnimTransform() != NULL) {
            m_TouchOkBack.IncreaseFrame();
        }
        if (m_SelectOkBack.GetAnimTransform() != NULL) {
            m_SelectOkBack.IncreaseFrame();
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

// 0x002E03D4 (name is ours)
void ButtonActionNodeEx::SetAnimTransforms(nw::lyt::AnimTransform* touch, nw::lyt::AnimTransform* select,
                                           nw::lyt::AnimTransform* touchOk, nw::lyt::AnimTransform* selectOk,
                                           nw::lyt::AnimTransform* touchBack, nw::lyt::AnimTransform* selectBack,
                                           nw::lyt::AnimTransform* touchOkBack, nw::lyt::AnimTransform* selectOkBack,
                                           ssys::ma::lyt::Layout* layout)
{
    ButtonActionNode::SetAnimTransforms(touch, select, touchOk, selectOk, layout);
    m_TouchBack.SetAnimTransform(touchBack);
    m_SelectBack.SetAnimTransform(selectBack);
    m_TouchBack.SetFrame(0.0f);
    m_SelectBack.SetFrame(0.0f);
    m_TouchOkBack.SetAnimTransform(touchOkBack);
    m_SelectOkBack.SetAnimTransform(selectOkBack);
    m_TouchOkBack.SetFrame(0.0f);
    m_SelectOkBack.SetFrame(0.0f);
}

// 0x002E04AC slot 0x20 (name after libgarden)
void ButtonActionNodeEx::Select(bool select, bool sound)
{
    if (select && sound) {
        PlaySound();
    }
    m_IsPressed = select;
    Animate(m_IsBack == 1 ? m_SelectBack : m_Select, select);
}

// 0x002E050C slot 0x1C (name after libgarden)
void ButtonActionNodeEx::TouchSelect(bool touch, bool sound)
{
    if (touch && sound) {
        PlaySound();
    }
    m_IsPressed = touch;
    if (m_IsBack == 1) {
        Animate(m_TouchBack, touch);
        Animate(m_SelectBack, touch);
    } else {
        Animate(m_Touch, touch);
        Animate(m_Select, touch);
    }
}

// 0x002E059C | libgarden [tier A]
ButtonActionNodeEx::ButtonActionNodeEx() : m_IsBack(0), m_IsOkBack(0)
{
}

// 0x002E06AC slot 0x00 | libgarden
// 0x002E05E0 (deleting dtor)
ButtonActionNodeEx::~ButtonActionNodeEx()
{
    m_TouchBack.SetAnimTransform(NULL);
    m_SelectBack.SetAnimTransform(NULL);
    m_TouchOkBack.SetAnimTransform(NULL);
    m_SelectOkBack.SetAnimTransform(NULL);
}
