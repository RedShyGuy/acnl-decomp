#pragma once

#include "decomp.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/local/local_LocalStreamBase.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local16LocalInputStreamE @ 0x008CFB10
// vtable 0x00900930 (vptr 0x00900938), offset_to_top 0, 4 entries
// vtable 0x00900948 (vptr 0x00900950), offset_to_top -1472, 3 entries
class LocalInputStream : public ::nn::pia::local::LocalStreamBase, public ::nn::pia::common::IPacketInput
{
public:
    virtual void vf_0x00(); // 0x00416A40 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalStreamBase
    virtual void vf_0x04(); // 0x00416B88 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalStreamBase
    virtual void vf_0x08(); // 0x00730248 slot 0x08 | virtual slot, introduced by nn::pia::local::LocalStreamBase
    virtual nn::Result Read(nn::pia::common::Packet*); // 0x00416A50 slot 0x0C | fefates:bytes-fuzzy
    LocalInputStream(); // 0x00416B68 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
