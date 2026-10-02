#pragma once

#include "decomp.h"
#include "Bs/dBsSvMgr.h"
#include "Sv/dSvStep.h"

// RTTI N7BsSvMgr8SaveStepE @ 0x008D3ED0
// vtable 0x0090BC58 (vptr 0x0090BC60), offset_to_top 0, 5 entries
class BsSvMgr::SaveStep : public ::SvStep<BsSvMgr, BsSvMgr::SaveStep>
{
public:
    SaveStep(); // ctor address unknown
    virtual void vf_0x00(); // 0x0060B8C4 slot 0x00 | virtual slot, introduced by state::Step<BsSvMgr, BsSvMgr::SaveStep>
    virtual void vf_0x04(); // 0x0060B8AC slot 0x04 | virtual slot, introduced by state::Step<BsSvMgr, BsSvMgr::SaveStep>
    virtual void vf_0x0C(); // 0x0060B7E4 slot 0x0C | virtual slot, introduced by BsSvMgr::SaveStep
    virtual void vf_0x10(); // 0x0060B700 slot 0x10 | virtual slot, introduced by BsSvMgr::SaveStep
};
