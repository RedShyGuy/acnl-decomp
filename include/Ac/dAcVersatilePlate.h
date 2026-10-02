#pragma once

#include "decomp.h"
#include "Ac/dAcSimpleTalk.h"
#include "Other/dVersatilePlateTalkRecept.h"

// RTTI 16AcVersatilePlate @ 0x008CC1A8
// vtable 0x008F22B4 (vptr 0x008F22BC), offset_to_top 0, 37 entries
class AcVersatilePlate : public ::AcSimpleTalk<VersatilePlateTalkRecept>
{
public:
    AcVersatilePlate(); // ctor candidate(s) 0x007F0068 (unverified)
    virtual ~AcVersatilePlate(); // 0x002AA304 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002AA2CC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x90(); // 0x0071D820 slot 0x90 | virtual slot, introduced by AcSimpleTalkBase
};
