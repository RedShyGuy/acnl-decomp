#include "Button/dButtonActionCstmNode.h"

// 0x0031CE64 (name after the RTTI)
ButtonActionCstmNode::ButtonActionCstmNode()
{
    // empty (in the original too)
}

// 0x0031CF18 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
// 0x0031CE7C (deleting dtor)
ButtonActionCstmNode::~ButtonActionCstmNode()
{
    m_Touch.Finalize();
    m_Select.Finalize();
    m_TouchOk.Finalize();
    m_SelectOk.Finalize();
}
