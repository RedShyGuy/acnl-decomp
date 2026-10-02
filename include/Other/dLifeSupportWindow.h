#pragma once

#include "decomp.h"
#include "Other/dSoPaCaWindowBase.h"
#include "state/dMode.h"

// RTTI 17LifeSupportWindow @ 0x008CC674
// vtable 0x008F3AD4 (vptr 0x008F3ADC), offset_to_top 0, 11 entries
// vtable 0x008F3B08 (vptr 0x008F3B10), offset_to_top -36, 10 entries
class LifeSupportWindow : public ::state::Mode<LifeSupportWindow>, public ::SoPaCaWindowBase
{
public:
    LifeSupportWindow(); // ctor address unknown
    virtual ~LifeSupportWindow(); // 0x002CF4AC slot 0x00 | slot vf_0x00 of state::Mode<LifeSupportWindow>
    // 0x002CF494 slot 0x04 | slot vf_0x04 of state::Mode<LifeSupportWindow> (deleting dtor)
    virtual void vf_0x0C(); // 0x002CF014 slot 0x0C | virtual slot, introduced by LifeSupportWindow
    virtual void vf_0x10(); // 0x0071FE5C slot 0x10 | virtual slot, introduced by LifeSupportWindow
    virtual void vf_0x14(); // 0x002CF02C slot 0x14 | virtual slot, introduced by LifeSupportWindow
    virtual void vf_0x18(); // 0x0071FEE0 slot 0x18 | virtual slot, introduced by LifeSupportWindow
    virtual void vf_0x1C(); // 0x002CF04C slot 0x1C | virtual slot, introduced by LifeSupportWindow
    virtual void vf_0x20(); // 0x002CE464 slot 0x20 | virtual slot, introduced by LifeSupportWindow
    virtual void vf_0x24(); // 0x002CEFF4 slot 0x24 | virtual slot, introduced by LifeSupportWindow
    virtual void vf_0x28(); // 0x002CF210 slot 0x28 | virtual slot, introduced by LifeSupportWindow
};
