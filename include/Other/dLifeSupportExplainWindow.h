#pragma once

#include "decomp.h"
#include "Other/dInOutWindow.h"

// RTTI 24LifeSupportExplainWindow @ 0x008CD00C
// vtable 0x008F78A8 (vptr 0x008F78B0), offset_to_top 0, 13 entries
class LifeSupportExplainWindow : public ::InOutWindow
{
public:
    LifeSupportExplainWindow(); // ctor address unknown
    virtual ~LifeSupportExplainWindow(); // 0x0033E5E4 slot 0x00 | slot vf_0x00 of InOutWindow
    // 0x0033E598 slot 0x04 | slot vf_0x04 of InOutWindow (deleting dtor)
    virtual void vf_0x18(); // 0x0033E55C slot 0x18 | virtual slot, introduced by InOutWindow
    virtual void vf_0x28(); // 0x0033DDDC slot 0x28 | virtual slot, introduced by InOutWindow
};
