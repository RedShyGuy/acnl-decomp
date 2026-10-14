#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_PersistentGathering.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19PersistentGatheringE @ 0x008CE7F0
// vtable 0x008FD6D8 (vptr 0x008FD6E0), offset_to_top 0, 9 entries
class PersistentGathering : public ::nn::nex::_DDL_PersistentGathering
{
public:
    // the values of a reset (the names are ours)
    static const u32 RESET_UNKNOWN0X40_SIZE = 6;
    static const u32 FLAG_PERSISTENT = 1;
    static const u32 PARTICIPATION_POLICY = 97;

    PersistentGathering(); // 0x003946AC | fefates:bytes [tier B]
    virtual ~PersistentGathering(); // 0x0039475C slot 0x00
    // 0x0039474C slot 0x04 (deleting dtor)
    void Reset(); // 0x003945E0 | fefates:bytes [tier B]
};
ASSERT_SIZE(PersistentGathering, 0x70);
} // namespace nex
} // namespace nn
