#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_UpdateMatchmakeSessionParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex27UpdateMatchmakeSessionParamE @ 0x008CF054
// vtable 0x008FEFBC (vptr 0x008FEFC4), offset_to_top 0, 2 entries
class UpdateMatchmakeSessionParam : public ::nn::nex::_DDL_UpdateMatchmakeSessionParam
{
public:
    // (inline in pia::inet::NexMatchmakeSession; not decompiled yet)
    UpdateMatchmakeSessionParam();
    // the values of a new parameter / the application buffer (names are ours)
    void Reset(); // 0x003BD81C
    void SetApplicationBuffer(const qVector<u8>& buffer); // 0x003BD65C
    virtual void vf_0x00(); // 0x003BD94C slot 0x00 | virtual slot, introduced by nn::nex::_DDL_UpdateMatchmakeSessionParam
    virtual void vf_0x04(); // 0x003BD908 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_UpdateMatchmakeSessionParam
};
} // namespace nex
} // namespace nn
