#pragma once

#include "decomp.h"
#include "Other/dInOutWindow.h"

// RTTI 13ExplainWindow @ 0x008CB9FC
// vtable 0x008EF290 (vptr 0x008EF298), offset_to_top 0, 13 entries
class ExplainWindow : public ::InOutWindow
{
public:
    ExplainWindow(); // ctor candidate(s) 0x0022CD84 (unverified)
    virtual ~ExplainWindow(); // 0x0022CE38 slot 0x00 | slot vf_0x00 of InOutWindow
    // 0x0022CDEC slot 0x04 | slot vf_0x04 of InOutWindow (deleting dtor)
    virtual void vf_0x10(); // 0x00569170 slot 0x10 | virtual slot, introduced by InOutWindow
    virtual void vf_0x28(); // 0x0022CBE8 slot 0x28 | virtual slot, introduced by InOutWindow
};
