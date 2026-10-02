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
    virtual ~PersistentGathering(); // 0x0039475C slot 0x00 | slot vf_0x00 of nn::nex::_DDL_Gathering
    // 0x0039474C slot 0x04 | slot vf_0x04 of nn::nex::_DDL_Gathering (deleting dtor)
    void Reset(); // 0x003945E0 | fefates:bytes [tier B]
    PersistentGathering(); // 0x003946AC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
