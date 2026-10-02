#pragma once

#include "decomp.h"
#include "Other/dExplainWindow.h"

// RTTI 18BadgeExplainWindow @ 0x008CC710
// vtable 0x008F3E5C (vptr 0x008F3E64), offset_to_top 0, 13 entries
class BadgeExplainWindow : public ::ExplainWindow
{
public:
    BadgeExplainWindow(); // ctor address unknown
    virtual ~BadgeExplainWindow(); // 0x002D6B88 slot 0x00 | slot vf_0x00 of InOutWindow
    // 0x002D6B54 slot 0x04 | slot vf_0x04 of InOutWindow (deleting dtor)
    virtual void vf_0x18(); // 0x002D6B2C slot 0x18 | virtual slot, introduced by InOutWindow
    virtual void vf_0x28(); // 0x002D6570 slot 0x28 | virtual slot, introduced by InOutWindow
};
