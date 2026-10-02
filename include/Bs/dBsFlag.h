#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 6BsFlag @ 0x008CD2D8
// vtable 0x008F8BC8 (vptr 0x008F8BD0), offset_to_top 0, 22 entries
class BsFlag : public ::UtlBase<Base>
{
public:
    BsFlag(); // ctor address unknown
    virtual ~BsFlag(); // 0x005BFEC0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x005BFE68 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x005BFBEC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x005BFBD8 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x005BF89C slot 0x40 | virtual slot, introduced by BsFlag
    virtual void vf_0x44(); // 0x005BF7C8 slot 0x44 | virtual slot, introduced by BsFlag
    virtual void vf_0x48(); // 0x005BF508 slot 0x48 | virtual slot, introduced by BsFlag
    virtual void vf_0x4C(); // 0x005BF9FC slot 0x4C | virtual slot, introduced by BsFlag
    virtual void vf_0x50(); // 0x005BF9F4 slot 0x50 | virtual slot, introduced by BsFlag
    virtual void vf_0x54(); // 0x005BF9B0 slot 0x54 | virtual slot, introduced by BsFlag
};
