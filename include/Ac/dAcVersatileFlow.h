#pragma once

#include "decomp.h"
#include "Ac/dAcSimpleTalk.h"
#include "Other/dVersatileFlowTalkRecept.h"

// RTTI 15AcVersatileFlow @ 0x008CBEA8
// vtable 0x008F1140 (vptr 0x008F1148), offset_to_top 0, 37 entries
class AcVersatileFlow : public ::AcSimpleTalk<VersatileFlowTalkRecept>
{
public:
    AcVersatileFlow(); // ctor candidate(s) 0x007EF55C (unverified)
    virtual ~AcVersatileFlow(); // 0x00283728 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002836F0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
};
