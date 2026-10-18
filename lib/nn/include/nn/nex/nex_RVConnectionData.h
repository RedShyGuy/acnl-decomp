#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_RVConnectionData.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16RVConnectionDataE @ 0x008CE608
// vtable 0x008FD224 (vptr 0x008FD22C), offset_to_top 0, 2 entries
class RVConnectionData : public ::nn::nex::_DDL_RVConnectionData
{
public:
    // (inline)
    DECOMP_EMIT_COPY RVConnectionData() {} // 0x003838F8
    virtual ~RVConnectionData(); // 0x003839B8 slot 0x00
    // 0x003839A8 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(RVConnectionData, 0xF8);
} // namespace nex
} // namespace nn
