#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport17NetworkRttManagerE @ 0x008D01E8
// vtable 0x00901E9C (vptr 0x00901EA4), offset_to_top 0, 1 entries
class NetworkRttManager : public ::nn::pia::common::RootObject
{
public:
    class MeasurementData;
    NetworkRttManager(); // ctor candidate(s) 0x00454490 (unverified)
    virtual void vf_0x00(); // 0x007356D8 slot 0x00 | virtual slot, introduced by nn::pia::transport::NetworkRttManager
    void GetAverage(const nn::pia::common::StationAddress&); // 0x0045421C | fefates:bytes [tier B]
    void WriteToPacket(nn::pia::common::Packet*); // 0x0045435C | fefates:bytes [tier B]
    void CreateInstance(); // 0x00454490 | fefates:bytes [tier B]
    void InitializeImpl(unsigned int, unsigned int); // 0x00454550 | fefates:bytes [tier B]
    void ReadFromPacket(const nn::pia::common::Packet*); // 0x004546D4 | fefates:bytes [tier B]
    void Finalize(); // 0x00454C20 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
