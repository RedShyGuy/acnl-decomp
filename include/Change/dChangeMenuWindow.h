#pragma once

#include "decomp.h"
#include "Other/dInOutWindowInButton.h"

// RTTI 16ChangeMenuWindow @ 0x008CC2D8
// vtable 0x008F292C (vptr 0x008F2934), offset_to_top 0, 17 entries
class ChangeMenuWindow : public ::InOutWindowInButton<3>
{
public:
    ChangeMenuWindow(); // ctor candidate(s) 0x0025D048 (unverified)
    virtual ~ChangeMenuWindow(); // 0x002B79E0 slot 0x00 | slot vf_0x00 of InOutWindow
    // 0x002B7984 slot 0x04 | slot vf_0x04 of InOutWindow (deleting dtor)
    virtual void vf_0x1C(); // 0x002B7948 slot 0x1C | virtual slot, introduced by InOutWindow
    virtual void vf_0x20(); // 0x002B793C slot 0x20 | virtual slot, introduced by InOutWindow
    virtual void vf_0x2C(); // 0x002B75F4 slot 0x2C | virtual slot, introduced by InOutWindow
    virtual void vf_0x34(); // 0x0071DC50 slot 0x34 | virtual slot, introduced by InOutWindowInButton<3>
    virtual void vf_0x38(); // 0x0071DC40 slot 0x38 | virtual slot, introduced by InOutWindowInButton<3>
    virtual void vf_0x3C(); // 0x0071DC38 slot 0x3C | virtual slot, introduced by InOutWindowInButton<3>
    virtual void vf_0x40(); // 0x002B78EC slot 0x40 | virtual slot, introduced by InOutWindowInButton<3>
};
