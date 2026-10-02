#pragma once

#include "decomp.h"
#include "Other/dInOutWindow.h"

// RTTI 17CensusContentList @ 0x008CC4E4
// vtable 0x008F36E0 (vptr 0x008F36E8), offset_to_top 0, 13 entries
class CensusContentList : public ::InOutWindow
{
public:
    CensusContentList(); // ctor address unknown
    virtual ~CensusContentList(); // 0x002CC0A4 slot 0x00 | slot vf_0x00 of InOutWindow
    // 0x002CC094 slot 0x04 | slot vf_0x04 of InOutWindow (deleting dtor)
    virtual void vf_0x18(); // 0x002CC014 slot 0x18 | virtual slot, introduced by InOutWindow
    virtual void vf_0x28(); // 0x002CB4E4 slot 0x28 | virtual slot, introduced by InOutWindow
};
