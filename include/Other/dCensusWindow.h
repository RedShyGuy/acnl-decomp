#pragma once

#include "decomp.h"
#include "Other/dSoPaCaWindowBase.h"
#include "state/dMode.h"

// RTTI 12CensusWindow @ 0x008CB5C4
// vtable 0x008EE310 (vptr 0x008EE318), offset_to_top 0, 11 entries
// vtable 0x008EE344 (vptr 0x008EE34C), offset_to_top -36, 10 entries
class CensusWindow : public ::state::Mode<CensusWindow>, public ::SoPaCaWindowBase
{
public:
    CensusWindow(); // ctor candidate(s) 0x001FF560 (unverified)
    virtual void vf_0x00(); // 0x001FF6E0 slot 0x00 | virtual slot, introduced by state::Mode<CensusWindow>
    virtual void vf_0x04(); // 0x001FF68C slot 0x04 | virtual slot, introduced by state::Mode<CensusWindow>
    virtual void vf_0x0C(); // 0x001FF218 slot 0x0C | virtual slot, introduced by CensusWindow
    virtual void vf_0x10(); // 0x00712E74 slot 0x10 | virtual slot, introduced by CensusWindow
    virtual void vf_0x14(); // 0x001FF230 slot 0x14 | virtual slot, introduced by CensusWindow
    virtual void vf_0x18(); // 0x00712EC8 slot 0x18 | virtual slot, introduced by CensusWindow
    virtual void vf_0x1C(); // 0x001FF250 slot 0x1C | virtual slot, introduced by CensusWindow
    virtual void vf_0x20(); // 0x001FF430 slot 0x20 | virtual slot, introduced by CensusWindow
    virtual void vf_0x24(); // 0x001CBC30 slot 0x24 | virtual slot, introduced by CensusWindow
    virtual void vf_0x28(); // 0x001FF4B8 slot 0x28 | virtual slot, introduced by CensusWindow
};
