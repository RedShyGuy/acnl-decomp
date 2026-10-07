#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet20NexConnectStationJobE @ 0x008CF960
// vtable 0x00900360 (vptr 0x00900368), offset_to_top 0, 8 entries
//
// The connection to a station of inet: the address to try (the public or the private location of
// the station) is tested, a NAT traversal is prepared and awaited if needed, then the connection
// request of the base is sent. The function names are from the fefates symbols and the step
// strings; the member names are ours.
class NexConnectStationJob : public ::nn::pia::transport::ConnectStationJob
{
public:
    // how long the traversal may take to start (ms)
    static const s32 NAT_TRAVERSAL_START_TIMEOUT_MSEC = 15000;
    // the step returns WAIT with this
    static const u16 WAIT_MSEC = 100;

    NexConnectStationJob(); // 0x004012E4 | fefates:bytes [tier B]
    virtual ~NexConnectStationJob(); // 0x00401380 slot 0x00
    // 0x00401338 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F174 slot 0x14
    virtual nn::Result StartupImpl(nn::pia::common::CallContext* pCallContext, nn::pia::transport::Station* pStation,
                                   const nn::pia::transport::StationConnectionInfo& info, bool isInverseConnection); // 0x004001F0 slot 0x18
    virtual void CleanupImpl(); // 0x00400140 slot 0x1C | fefates:callseq

    // the steps
    common::ExecuteResult tryCurrentAddress(); // 0x004004FC | fefates:bytes [tier B]
    common::ExecuteResult testCurrentAddress(); // 0x004005CC | fefates:bytes [tier B]
    common::ExecuteResult prepareNatTraversal(); // 0x0040072C
    common::ExecuteResult resolveCurrentAddress(); // 0x00400A24 | fefates:bytes [tier B]
    common::ExecuteResult waitForNatTraversalStarted(); // 0x00400AE8
    common::ExecuteResult waitForNatTraversalCompleted(); // 0x00400E80

    // the elapsed time since the start of the traversal to the location (inline)
    void SetNatTraversalTime();

    transport::StationLocation m_CurrentLocation;      // 0x5C, the address that is tried
    transport::StationConnectionInfo m_ConnectionInfo; // 0x84, of the station
    common::Time m_Deadline;                           // 0xD8, of the NAT traversal
    common::CallContext m_NatCallContext;              // 0xE0, of the NAT traversal
    s32 m_CompletedRetryNum;                           // 0xF4, waitForNatTraversalCompleted may start again
    s32 m_StartedRetryNum;                             // 0xF8, waitForNatTraversalStarted may wait again
};
ASSERT_OFFSET(NexConnectStationJob, m_CurrentLocation, 0x5C);
ASSERT_OFFSET(NexConnectStationJob, m_ConnectionInfo, 0x84);
ASSERT_OFFSET(NexConnectStationJob, m_NatCallContext, 0xE0);
ASSERT_SIZE(NexConnectStationJob, 0x100);
} // namespace inet
} // namespace pia
} // namespace nn
