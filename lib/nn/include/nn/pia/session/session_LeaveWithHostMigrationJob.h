#pragma once

#include "decomp.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"
#include <string.h>

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session25LeaveWithHostMigrationJobE @ 0x008D0104
// vtable 0x00901C50 (vptr 0x00901C58), offset_to_top 0, 8 entries
//
// The host leaves the mesh and lets the stations migrate: it tells them the next host (or sends
// the ranking of the candidates) and waits for their responses, then disconnects. The network
// specific jobs decide the next host. The steps are from the strings of the binary; the layout is
// from the constructor, the member names and the names marked so are ours.
class LeaveWithHostMigrationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LeaveWithHostMigrationJob(); // 0x00443570 | fefates:bytes [tier B]
    virtual ~LeaveWithHostMigrationJob(); // 0x004435DC slot 0x00
    // 0x004435C8 slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x00734174 slot 0x14
    // the next host (name is ours)
    virtual nn::pia::StationIndex DecideNextHost() = 0; // slot 0x18
    virtual void CleanupStatus(); // 0x00442DF4 slot 0x1C | fefates:bytes

    // false if it runs already (name is ours)
    bool Startup(nn::pia::common::CallContext* pCallContext); // 0x00443468
    void Cleanup(); // 0x00443420 | fefates:bytes [tier B]
    void ReceiveMigrationResponse(nn::pia::StationIndex stationIndex); // 0x00442EE8 | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult CheckHostMigrationProcess(); // 0x0044302C
    common::ExecuteResult WaitHostMigrationProcess(); // 0x00442F00
    common::ExecuteResult SendStartMigrationMessage(); // 0x004431D8
    common::ExecuteResult SendStartMultiMigrationMessage(); // 0x00443334
    common::ExecuteResult WaitMigrationResponse(); // 0x00442E20
    common::ExecuteResult CleanupMesh(); // 0x00442D40

    common::CallContext* m_pCallContext;   // 0x40, of the caller
    common::Time m_Deadline;               // 0x48
    s32 m_TimeoutMSec;                     // 0x50 (5000)
    bool m_IsRunning;                      // 0x54
    StationIndex m_NewHostStationIndex;    // 0x55 (253)
    bool m_IsWaitingResponses[STATION_INDEX_MAX + 1]; // 0x56, per station
    bool m_IsWaitingResponse;              // 0x62, the migration responses are awaited
};
ASSERT_OFFSET(LeaveWithHostMigrationJob, m_TimeoutMSec, 0x50);
ASSERT_OFFSET(LeaveWithHostMigrationJob, m_IsWaitingResponses, 0x56);
ASSERT_OFFSET(LeaveWithHostMigrationJob, m_IsWaitingResponse, 0x62);
ASSERT_SIZE(LeaveWithHostMigrationJob, 0x68);
} // namespace session
} // namespace pia
} // namespace nn
