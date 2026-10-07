#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local30LocalStartHostMigrationMessageE @ 0x008CFD60
// vtable 0x00901340 (vptr 0x00901348), offset_to_top 0, 4 entries
//
// The host leaves and the clients start the host migration
// (LocalNetworkManager::SendStartHostMigrationMessage); the header has 4 bytes more (0).
class LocalStartHostMigrationMessage : public ::nn::pia::local::LocalMessage
{
public:
    static const u16 START_HOST_MIGRATION_HEADER_SIZE = 16;

    LocalStartHostMigrationMessage(u8* pBuffer, u32 bufferSize); // 0x0042394C
    virtual ~LocalStartHostMigrationMessage(); // 0x00423980 slot 0x00
    // 0x0042397C slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x00423900 slot 0x08
    virtual bool ParseMessageHeader(); // 0x004238C0 slot 0x0C
};
ASSERT_SIZE(LocalStartHostMigrationMessage, 0x14);
} // namespace local
} // namespace pia
} // namespace nn
