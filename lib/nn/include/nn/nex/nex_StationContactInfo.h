#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// The URLs of a station (of one RVCID) to choose the ones to contact. The names of the members are
// ours.
// RTTI N2nn3nex18StationContactInfoE @ 0x008CE748
// vtable 0x008FD568 (vptr 0x008FD570), offset_to_top 0, 2 entries
class StationContactInfo : public ::nn::nex::RootObject
{
public:
    // the bit of the parameter "type" of a public URL (name is ours)
    static const u32 URL_TYPE_PUBLIC = 2;

    // the URLs without an RVCID and the ones of the RVCID of the first URL that has one
    StationContactInfo(const nn::nex::qList<nn::nex::StationURL>& urls); // 0x00390314 | fefates:bytes [tier B]
    virtual ~StationContactInfo(); // 0x003903E0 slot 0x00 | fefates:bytes
    // 0x003903B0 slot 0x04 (deleting dtor)
    // the URLs of the target to contact from this station: the public ones, and first the private ones
    // too if the target has a public address of this station (the same NAT); unchanged if the target
    // has no public URL
    void SortAndFilterTarget(nn::nex::StationContactInfo& target) const; // 0x0072C2B8 | fefates:bytes [tier B]

    u32 m_Rvcid;              // 0x04
    qList<StationURL> m_URLs; // 0x08
};
ASSERT_SIZE(StationContactInfo, 0x20);
} // namespace nex
} // namespace nn
