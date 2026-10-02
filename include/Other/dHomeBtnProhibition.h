#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 18HomeBtnProhibition @ 0x008CC850
// vtable 0x008F4730 (vptr 0x008F4738), offset_to_top 0, 3 entries
class HomeBtnProhibition : public ::state::Mode<HomeBtnProhibition>
{
public:
    class SingletonDisposer_;
    HomeBtnProhibition(); // ctor address unknown
    virtual void vf_0x00(); // 0x002E38B4 slot 0x00 | virtual slot, introduced by HomeBtnProhibition
    virtual void vf_0x04(); // 0x002E385C slot 0x04 | virtual slot, introduced by HomeBtnProhibition
    virtual void vf_0x08(); // 0x0082C2A0 slot 0x08 | virtual slot, introduced by HomeBtnProhibition
    static HomeBtnProhibition* s_pInstance; // 0x00953CD0
};
