#include "Button/dButtonActionControlEx.h"

// 0x003281D8 | libgarden [tier A]
void ButtonActionControlEx::Initialize(ButtonActionControlEx::Description const& desc)
{
    m_CursorTimer = desc.cursorTimer;
    m_pLayout = desc.layout;
    m_pCheckInput = desc.checkInput;
    m_pBounding = desc.bounding;
    m_Flags = FLAG_4 | DISABLED;
    if (desc.touch != NULL) {
        m_Touch.Initialize(desc.touch, desc.resAcc);
    }
    if (desc.select != NULL) {
        m_Select.Initialize(desc.select, desc.resAcc);
    }
    if (desc.touchOk != NULL) {
        m_TouchOk.Initialize(desc.touchOk, desc.resAcc);
    }
    if (desc.selectOk != NULL) {
        m_SelectOk.Initialize(desc.selectOk, desc.resAcc);
    }
    if (desc.touchBack != NULL) {
        m_TouchBack.Initialize(desc.touchBack, desc.resAcc);
    }
    if (desc.selectBack != NULL) {
        m_SelectBack.Initialize(desc.selectBack, desc.resAcc);
    }
    if (desc.touchOkBack != NULL) {
        m_TouchOkBack.Initialize(desc.touchOkBack, desc.resAcc);
    }
    if (desc.selectOkBack != NULL) {
        m_SelectOkBack.Initialize(desc.selectOkBack, desc.resAcc);
    }
}

// 0x003282D4 | libgarden [tier A]
void ButtonActionControlEx::AddNode(ButtonActionNodeEx* node, bool updateAnimations)
{
    if (updateAnimations) {
        node->SetAnimTransforms(m_Touch.GetAnimTransform(), m_Select.GetAnimTransform(), m_TouchOk.GetAnimTransform(),
                                m_SelectOk.GetAnimTransform(), m_TouchBack.GetAnimTransform(),
                                m_SelectBack.GetAnimTransform(), m_TouchOkBack.GetAnimTransform(),
                                m_SelectOkBack.GetAnimTransform(), m_pLayout);
    }
    PushBack(node);
}

// 0x00328338 (name after libgarden)
ButtonActionControlEx::ButtonActionControlEx()
{
    // (only the members are constructed)
}

// 0x003283AC slot 0x00 (name after libgarden)
// 0x00328370 (deleting dtor)
ButtonActionControlEx::~ButtonActionControlEx()
{
    // (only the members are destroyed)
}
