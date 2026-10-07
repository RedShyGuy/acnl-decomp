#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21LocalHostMigrationJobE @ 0x008CFBEC
// vtable 0x00900DF8 (vptr 0x00900E00), offset_to_top 0, 6 entries
//
// The host migration after the host left (LocalMigrationManager::StartHostMigration): every
// station leaves the old network; the next host creates the new network and waits for the others,
// the others scan for it (10 seconds at most) and connect. The member names are ours.
class LocalHostMigrationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    static const u32 SCAN_NETWORK_TIMEOUT_MSEC = 10000;
    static const s32 CONNECTION_TIMEOUT_MSEC = 7000;

    LocalHostMigrationJob(); // 0x0041C284 | fefates:bytes [tier B]
    virtual ~LocalHostMigrationJob(); // 0x0041C324 slot 0x00
    // 0x0041C2E0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007311E0 slot 0x14

    common::ExecuteResult ScanNetwork(); // 0x0041B000 | fefates:bytes [tier B]
    common::ExecuteResult CreateNetwork(); // 0x0041B1EC | fefates:bytes [tier B]
    common::ExecuteResult WaitForCancel(); // 0x0041B310 | fefates:bytes [tier B]
    common::ExecuteResult ConnectNetwork(); // 0x0041B360 | fefates:bytes [tier B]
    common::ExecuteResult WaitScanNetwork(); // 0x0041B498
    common::ExecuteResult DisconnectNetwork(); // 0x0041B608
    common::ExecuteResult WaitAllClientsAck(); // 0x0041B760 | fefates:bytes [tier B]
    common::ExecuteResult WaitCreateNetwork(); // 0x0041B83C | fefates:bytes [tier B]
    common::ExecuteResult WaitConnectNetwork(); // 0x0041B9B0 | fefates:bytes [tier B]
    common::ExecuteResult SearchNewHostNetwork(); // 0x0041BAF8 | fefates:bytes [tier B]
    common::ExecuteResult WaitDisconnectNetwork(); // 0x0041BCA0 | fefates:bytes [tier B]
    // the migration failed: the nodes that are gone are cleared
    void HostMigrationFailureProcess(); // 0x0041BE44 | fefates:bytes [tier B]
    common::ExecuteResult WaitUntilAllClientsConnection(); // 0x0041BEB8
    void Cleanup(); // 0x0041C18C | fefates:bytes [tier B]
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, bool isNextHost); // 0x0041C1E0 | fefates:bytes [tier B]

    // the cancel of the caller (name is ours)
    inline void StartCancel();

    common::CallContext* m_pCallContext;           // 0x40, of LocalMigrationManager
    common::CallContext* m_pNetworkCallContext;    // 0x44, of the requests to LocalNetwork
    bool m_IsNextHost;                             // 0x48
    u32 m_NetworkIndex;                            // 0x4C, of the network of the new host
    common::Time m_ConnectionStartTime;            // 0x50
    common::Time m_ScanStartTime;                  // 0x58
};
ASSERT_OFFSET(LocalHostMigrationJob, m_ConnectionStartTime, 0x50);
ASSERT_SIZE(LocalHostMigrationJob, 0x60);
} // namespace local
} // namespace pia
} // namespace nn
