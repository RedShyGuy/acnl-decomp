#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local15LocalStreamBaseE @ 0x008CFB04
// vtable 0x0090091C (vptr 0x00900924), offset_to_top 0, 3 entries
//
// Base of the streams of the local network (the packets go through LocalNetworkManager). The
// buffer is not used by the functions here; the member name is ours.
class LocalStreamBase : public ::nn::pia::common::RootObject
{
public:
    static const u32 BUFFER_SIZE = 0x5BC;

    LocalStreamBase(); // 0x00416A2C
    virtual ~LocalStreamBase(); // 0x00416A44 slot 0x00
    // 0x00416A3C slot 0x04 (deleting dtor)
    virtual void vf_0x08(); // 0x00730244 slot 0x08

    u8 m_Buffer[BUFFER_SIZE]; // 0x004
};
ASSERT_SIZE(LocalStreamBase, 0x5C0);
} // namespace local
} // namespace pia
} // namespace nn
