#pragma once

#include "decomp.h"
#include "script/dITalkRecept.h"

// RTTI 19PlayerSimpleMessage @ 0x008CCA38
// vtable 0x008F5110 (vptr 0x008F5118), offset_to_top 0, 63 entries
class PlayerSimpleMessage : public ::script::ITalkRecept
{
public:
    PlayerSimpleMessage(); // ctor address unknown
    virtual ~PlayerSimpleMessage(); // 0x002FCD28 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x002FCD04 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x08(); // 0x002FADE4 slot 0x08 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x1C(); // 0x002FBE34 slot 0x1C | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x20(); // 0x002FBF6C slot 0x20 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x48(); // 0x002FB2A0 slot 0x48 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x5C(); // 0x002FC04C slot 0x5C | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x98(); // 0x002FBFB0 slot 0x98 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xA0(); // 0x002FADD8 slot 0xA0 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xA4(); // 0x002FAE00 slot 0xA4 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xA8(); // 0x002FBE60 slot 0xA8 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xAC(); // 0x002FBF00 slot 0xAC | virtual slot, introduced by script::ITalkRecept
};
