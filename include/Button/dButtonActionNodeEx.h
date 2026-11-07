#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"

// A button with a second set of animations ("back"), used instead of the normal ones while m_IsBack / m_IsOkBack is
// set. Class and member names after libgarden.
// RTTI 18ButtonActionNodeEx @ 0x008CC814
// vtable 0x008F463C (vptr 0x008F4644), offset_to_top 0, 16 entries
class ButtonActionNodeEx : public ::ButtonActionNode
{
public:
    ButtonActionNodeEx(); // 0x002E059C | libgarden [tier A]
    virtual ~ButtonActionNodeEx(); // 0x002E06AC slot 0x00 | libgarden
    // 0x002E05E0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void ResetSelect(); // 0x002E01BC slot 0x0C | libgarden
    virtual void TouchSelect(bool touch, bool sound); // 0x002E050C slot 0x1C (name after libgarden)
    virtual void Select(bool select, bool sound); // 0x002E04AC slot 0x20 (name after libgarden)
    virtual void BeginSelectOk(); // 0x002E0158 slot 0x24 (name after libgarden)
    virtual void EndSelectOk(); // 0x002E0274 slot 0x28 (name after libgarden)
    virtual bool IsOkOngoing() const; // 0x002E01D0 slot 0x2C (name after libgarden)
    virtual void DoOk(); // 0x002E0364 slot 0x30 (name after libgarden)

    // the animations of the control (name is ours)
    void SetAnimTransforms(nw::lyt::AnimTransform* touch, nw::lyt::AnimTransform* select,
                           nw::lyt::AnimTransform* touchOk, nw::lyt::AnimTransform* selectOk,
                           nw::lyt::AnimTransform* touchBack, nw::lyt::AnimTransform* selectBack,
                           nw::lyt::AnimTransform* touchOkBack, nw::lyt::AnimTransform* selectOkBack,
                           ssys::ma::lyt::Layout* layout); // 0x002E03D4

    using ButtonActionNode::Select;

    ssys::ma::lyt::Animation m_TouchBack;     // 0x0E0
    ssys::ma::lyt::Animation m_SelectBack;    // 0x108
    ssys::ma::lyt::Animation m_TouchOkBack;   // 0x130
    ssys::ma::lyt::Animation m_SelectOkBack;  // 0x158
    u8 m_IsBack;                              // 0x180
    u8 m_IsOkBack;                            // 0x181
};
ASSERT_OFFSET(ButtonActionNodeEx, m_IsBack, 0x180);
ASSERT_SIZE(ButtonActionNodeEx, 0x184);
