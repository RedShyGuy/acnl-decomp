#pragma once

#include "decomp.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local31LocalProcessHostMigrationJobNewE @ 0x008CFDA8
// vtable 0x009013E8 (vptr 0x009013F0), offset_to_top 0, 20 entries
//
// The host migration of the local network: the new host is the next candidate of the
// LocalMigrationManager (by transport id); it waits for LocalHostMigrationJob (the network side)
// and greets the stations with the steps of session::ProcessHostMigrationJob. The step names
// are from the strings; the member names are ours.
class LocalProcessHostMigrationJobNew : public ::nn::pia::session::ProcessHostMigrationJob
{
public:
    static const s32 NETWORK_MIGRATION_TIMEOUT_MSEC = 10000;

    LocalProcessHostMigrationJobNew(); // 0x00425518
    // (a nop that falls into the destructor of session::ProcessHostMigrationJob)
    virtual ~LocalProcessHostMigrationJobNew(); // 0x00442AEC slot 0x00
    // 0x0042553C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007317F4 slot 0x14
    virtual bool IsValidHostMigrationSetting(); // 0x004250A0 slot 0x24
    virtual bool StartupImpl(bool isFromMessage, nn::pia::StationIndex stationIndex); // 0x004249DC slot 0x2C
    virtual void CleanupStatus(); // 0x00424AB4 slot 0x34

    // the steps
    common::ExecuteResult LocalDecideNextHost(); // 0x00424AD0
    common::ExecuteResult LocalCleanupOldHostInfo(); // 0x00424E08
    common::ExecuteResult LocalPrepareForBecomingHost(); // 0x004250C0
    common::ExecuteResult WaitLocalHostMigrationClient(); // 0x00425218
    common::ExecuteResult WaitLocalHostMigrationNewHost(); // 0x0042538C

    u8 m_OldHostTransportId; // 0xA8 (255)
    u8 m_NewHostTransportId; // 0xA9 (255)
};
ASSERT_OFFSET(LocalProcessHostMigrationJobNew, m_OldHostTransportId, 0xA8);
} // namespace local
} // namespace pia
} // namespace nn
