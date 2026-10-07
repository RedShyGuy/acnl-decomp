#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace common {
class StationAddress;
} // namespace common
namespace session {
// RTTI N2nn3pia7session16KickoutManageJobE @ 0x008CFFE4
// vtable 0x009018F4 (vptr 0x009018FC), offset_to_top 0, 8 entries
//
// Kickouts: the host notes the stations it kicked out (StartKickout sends the notice); a client
// that gets the notice leaves the mesh (ReceiveKickoutNotice starts the client steps). The steps
// are from the strings of the binary; the layout is from the constructor, the member names and the
// names marked so are ours.
class KickoutManageJob : public ::nn::pia::common::StepSequenceJob
{
public:
    // the type name is from the fefates symbols; the values are not known yet (MeshProtocol uses
    // 3 and more for the relay routes)
    enum KickoutReason : u8
    {
    };

    // a station the host kicked out
    struct Entry
    {
        StationIndex m_StationIndex; // 0x0 (253: free)
        KickoutReason m_Reason;      // 0x1
    };

    KickoutManageJob(); // 0x00438950
    virtual ~KickoutManageJob(); // 0x004389B4 slot 0x00
    // 0x0043898C slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x007338F8 slot 0x14
    // (empty here; names are ours)
    virtual void StartupImpl(); // 0x00438608 slot 0x18
    virtual void OnKickout(const nn::pia::common::StationAddress& address); // 0x0043860C slot 0x1C

    // the host: the notice to the station
    nn::Result StartKickout(nn::pia::StationIndex stationIndex, nn::pia::session::KickoutManageJob::KickoutReason reason); // 0x004382C4 | fefates:bytes [tier B]
    // the host: the station is gone (the name is from the fefates symbols)
    void SetLeaveEventStationIndex(nn::pia::StationIndex stationIndex); // 0x004387F0 | fefates:bytes [tier B]
    // the client: the kickout notice of the host arrived; false if a kickout runs (name is ours)
    bool ReceiveKickoutNotice(KickoutReason reason); // 0x00438374
    // the call context of the application for the kickout; false if there is one
    bool AssociateKickoutWith(nn::pia::common::CallContext* pCallContext); // 0x00438690 | fefates:bytes [tier B]
    // (names are ours)
    void Cleanup(); // 0x00438890
    bool ClearEntries(); // 0x00438904

    // the steps of the client
    common::ExecuteResult ClientStartLeaveMesh(); // 0x004386C8
    common::ExecuteResult ClientWaitLeaveMesh(); // 0x00438610 | fefates:bytes [tier B]
    common::ExecuteResult ClientFinalize(); // 0x00438528

    // (inline; name is ours)
    void ClearStationIndices()
    {
        for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
            m_Entries[i].m_StationIndex = STATION_INDEX_UNIDENTIFIED;
        }
    }

    Entry m_Entries[STATION_INDEX_MAX + 1];  // 0x40
    KickoutReason m_Reason;                  // 0x58, of the notice
    common::CallContext m_CallContext;       // 0x5C, of the leaving
    common::CallContext* m_pCallContext;     // 0x70, of the application (AssociateKickoutWith)
    bool m_IsJoinCanceled;                   // 0x74, the notice came while joining
    nn::Result m_Result;                     // 0x78, of the canceled join (RESULT_NOT_SET)
};
ASSERT_OFFSET(KickoutManageJob, m_Reason, 0x58);
ASSERT_OFFSET(KickoutManageJob, m_CallContext, 0x5C);
ASSERT_OFFSET(KickoutManageJob, m_Result, 0x78);
ASSERT_SIZE(KickoutManageJob, 0x80);
} // namespace session
} // namespace pia
} // namespace nn
