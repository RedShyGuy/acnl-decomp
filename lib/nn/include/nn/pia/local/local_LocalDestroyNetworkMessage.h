#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local26LocalDestroyNetworkMessageE @ 0x008CFCD0
// vtable 0x00901188 (vptr 0x00901190), offset_to_top 0, 4 entries
//
// The host ends the network (LocalNetworkManager::SendDestroyNetworkMessage); the header has 4
// bytes more (0).
class LocalDestroyNetworkMessage : public ::nn::pia::local::LocalMessage
{
public:
    static const u16 DESTROY_NETWORK_HEADER_SIZE = 16;

    LocalDestroyNetworkMessage(u8* pBuffer, u32 bufferSize); // 0x00420BEC
    virtual ~LocalDestroyNetworkMessage(); // 0x00420C20 slot 0x00
    // 0x00420C1C slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x00420BA0 slot 0x08
    virtual bool ParseMessageHeader(); // 0x00420B60 slot 0x0C
};
ASSERT_SIZE(LocalDestroyNetworkMessage, 0x14);
} // namespace local
} // namespace pia
} // namespace nn
