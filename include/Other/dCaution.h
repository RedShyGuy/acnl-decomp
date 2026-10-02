#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 7Caution @ 0x008CD3A8
// vtable 0x008F90DC (vptr 0x008F90E4), offset_to_top 0, 3 entries
class Caution : public ::state::Mode<Caution>
{
public:
    class SingletonDisposer_;
    Caution(); // ctor address unknown
    virtual void vf_0x00(); // 0x0060CFF0 slot 0x00 | virtual slot, introduced by Caution
    virtual void vf_0x04(); // 0x0060CFB8 slot 0x04 | virtual slot, introduced by Caution
    virtual void vf_0x08(); // 0x0082D128 slot 0x08 | virtual slot, introduced by Caution
    static Caution* s_pInstance; // 0x009524CC
};
