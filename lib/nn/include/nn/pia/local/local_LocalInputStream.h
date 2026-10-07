#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/local/local_LocalStreamBase.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local16LocalInputStreamE @ 0x008CFB10
// vtable 0x00900930 (vptr 0x00900938), offset_to_top 0, 4 entries
// vtable 0x00900948 (vptr 0x00900950), offset_to_top -1472, 3 entries
//
// Reads the packets of the stations from the local network.
class LocalInputStream : public ::nn::pia::local::LocalStreamBase, public ::nn::pia::common::IPacketInput
{
public:
    LocalInputStream(); // 0x00416B68 | fefates:bytes [tier B]
    // (a nop that falls into the destructor of LocalStreamBase)
    virtual ~LocalInputStream(); // 0x00416A40 slot 0x00
    // 0x00416B88 slot 0x04 (deleting dtor)
    virtual void vf_0x08(); // 0x00730248 slot 0x08
    // RESULT_NO_DATA when nothing arrived
    virtual nn::Result Read(nn::pia::common::Packet* pPacket); // 0x00416A50 slot 0x0C | fefates:bytes-fuzzy
};
ASSERT_SIZE(LocalInputStream, 0x5C4);
} // namespace local
} // namespace pia
} // namespace nn
