#pragma once

#include "decomp.h"
#include "script/dIFlowRecept.h"
#include "script/dITalkRecept.h"

// RTTI 14NfcErrorRecept @ 0x008CBD00
// vtable 0x008F03E8 (vptr 0x008F03F0), offset_to_top 0, 67 entries
// vtable 0x008F04FC (vptr 0x008F0504), offset_to_top -124, 14 entries
class NfcErrorRecept : public ::script::ITalkRecept, public ::script::IFlowRecept
{
public:
    NfcErrorRecept(); // ctor candidate(s) 0x00270A28 (unverified)
    virtual ~NfcErrorRecept(); // 0x00270AA0 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x00270A84 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0xFC(); // 0x0027054C slot 0xFC | virtual slot, introduced by NfcErrorRecept
    virtual void vf_0x100(); // 0x00270810 slot 0x100 | virtual slot, introduced by NfcErrorRecept
    virtual void vf_0x104(); // 0x002709FC slot 0x104 | virtual slot, introduced by NfcErrorRecept
    virtual void vf_0x108(); // 0x005DA264 slot 0x108 | virtual slot, introduced by NfcErrorRecept
};
