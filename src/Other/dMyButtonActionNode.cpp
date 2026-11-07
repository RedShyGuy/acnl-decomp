#include "Other/dMyButtonActionNode.h"

// 0x002E5E84 (name after the RTTI)
MyButtonActionNode::MyButtonActionNode()
{
    // empty (in the original too)
}

// 0x002B74C0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
// 0x002E5E9C (deleting dtor)
MyButtonActionNode::~MyButtonActionNode()
{
    // empty (in the original too)
}

// 0x002E5E6C slot 0x0C (name after libgarden)
void MyButtonActionNode::ResetSelect()
{
    ButtonActionNode::ResetSelect();
    m_IsPressed = false;
}
