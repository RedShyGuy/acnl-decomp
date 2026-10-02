#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18StationContactInfoE @ 0x008CE748
// vtable 0x008FD568 (vptr 0x008FD570), offset_to_top 0, 2 entries
class StationContactInfo : public ::nn::nex::RootObject
{
public:
    StationContactInfo(); // ctor candidate(s) 0x00390314 (unverified)
    virtual ~StationContactInfo(); // 0x003903E0 slot 0x00 | fefates:bytes
    // 0x003903B0 slot 0x04 | slot vf_0x04 of nn::nex::StationContactInfo (deleting dtor)
    StationContactInfo(const nn::nex::qList<nn::nex::StationURL>&); // 0x00390314 | fefates:bytes [tier B]
    void SortAndFilterTarget(nn::nex::StationContactInfo&) const; // 0x0072C2B8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
