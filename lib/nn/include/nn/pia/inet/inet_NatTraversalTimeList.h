#pragma once

#include "decomp.h"
#include "nn/pia/common/common_FixedObjList.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet20NatTraversalTimeListE @ 0x008CF948
// vtable 0x00900350 (vptr 0x00900358), offset_to_top 0, 2 entries
class NatTraversalTimeList : public ::nn::pia::common::FixedObjList<nn::pia::inet::NatTraversalTime, 12u>
{
public:
    NatTraversalTimeList(); // ctor address unknown
    virtual void vf_0x00(); // 0x004000D4 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatTraversalTimeList
    virtual void vf_0x04(); // 0x00400064 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatTraversalTimeList
    void Add(const nn::pia::inet::NatTraversalTime&); // 0x003FFE2C | fefates:bytes-fuzzy [tier B]
    void Find(const nn::pia::transport::StationLocation&); // 0x003FFF40 | fefates:bytes [tier B]
    void Remove(const nn::pia::transport::StationLocation&); // 0x003FFFB0 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
