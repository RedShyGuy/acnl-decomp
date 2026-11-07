#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"

// A button with a second pair of ok animations, played instead of the normal ones while m_IsOkAlt is set (member names
// are ours).
// RTTI 28ButtonActionAllGroupBindNode @ 0x008CD100
// vtable 0x008F80D8 (vptr 0x008F80E0), offset_to_top 0, 16 entries
class ButtonActionAllGroupBindNode : public ::ButtonActionNode
{
public:
    ButtonActionAllGroupBindNode(); // 0x003437FC (name after the RTTI)
    virtual ~ButtonActionAllGroupBindNode(); // 0x003438D0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0034382C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void ResetSelect(); // 0x00343594 slot 0x0C (name after libgarden)
    virtual void TouchSelect(bool touch, bool sound); // 0x003437A0 slot 0x1C (name after libgarden)
    virtual void BeginSelectOk(); // 0x00343530 slot 0x24 (name after libgarden)
    virtual void EndSelectOk(); // 0x00343640 slot 0x28 (name after libgarden)
    virtual bool IsOkOngoing() const; // 0x0034359C slot 0x2C (name after libgarden)
    virtual void DoOk(); // 0x00343730 slot 0x30 (name after libgarden)

    ssys::ma::lyt::Animation m_TouchOkAlt;   // 0x0E0
    ssys::ma::lyt::Animation m_SelectOkAlt;  // 0x108
    u8 m_IsOkAlt;                            // 0x130
};
ASSERT_SIZE(ButtonActionAllGroupBindNode, 0x134);
