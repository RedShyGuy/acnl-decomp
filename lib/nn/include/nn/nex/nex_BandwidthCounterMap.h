#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19BandwidthCounterMapE @ 0x008CE784
// vtable 0x008FD5E8 (vptr 0x008FD5F0), offset_to_top 0, 2 entries
class BandwidthCounterMap : public ::nn::nex::RootObject
{
public:
    BandwidthCounterMap(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~BandwidthCounterMap(); // 0x0039164C slot 0x00 | fefates:bytes
    // 0x0039161C slot 0x04 | slot vf_0x04 of nn::nex::BandwidthCounterMap (deleting dtor)
    BandwidthCounterMap(const nn::nex::String&); // 0x00391564 | fefates:bytes [tier B]
    void operator[](unsigned int); // 0x003917F8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
