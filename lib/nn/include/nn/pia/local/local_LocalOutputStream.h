#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_IPacketOutput.h"
#include "nn/pia/local/local_LocalStreamBase.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local17LocalOutputStreamE @ 0x008CFB3C
// vtable 0x009009AC (vptr 0x009009B4), offset_to_top 0, 7 entries
// vtable 0x009009D0 (vptr 0x009009D8), offset_to_top -1472, 6 entries
//
// Sends the packets to one station or to all of the local network.
class LocalOutputStream : public ::nn::pia::local::LocalStreamBase, public ::nn::pia::common::IPacketOutput
{
public:
    LocalOutputStream(); // 0x00416C98 | fefates:bytes [tier B]
    virtual ~LocalOutputStream(); // 0x00416CC8 slot 0x00
    // 0x00416CB8 slot 0x04 (deleting dtor)
    virtual void vf_0x08(); // 0x007302A8 slot 0x08
    virtual nn::Result OnStationConnectionEvent(); // 0x00416BE4 slot 0x0C
    virtual nn::Result Write(const nn::pia::common::Packet& packet); // 0x00416BF4 slot 0x10 | fefates:bytes
    virtual u32 GetDestinationNumMax(); // 0x007302A0 slot 0x14
    virtual bool IsBroadcast(); // 0x00730298 slot 0x18
};
ASSERT_SIZE(LocalOutputStream, 0x5C4);
} // namespace local
} // namespace pia
} // namespace nn
