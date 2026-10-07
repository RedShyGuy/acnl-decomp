#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session21ProcessJoinRequestJobE @ 0x008D00A4
// vtable 0x00901AA4 (vptr 0x00901AAC), offset_to_top 0, 6 entries
//
// The join requests on the host: the job waits suspended in InitialStep until MeshProtocol hands it
// a request, decides on it and sends the join response (up to three parts) or the rejection. The
// steps are from the strings of the binary; the layout is from the constructor, the member names
// and the names marked so are ours.
class ProcessJoinRequestJob : public ::nn::pia::common::StepSequenceJob
{
public:
    // the join response parts
    static const u32 RESPONSE_PART_NUM = 3;

    ProcessJoinRequestJob(); // 0x00440520 | fefates:bytes-fuzzy [tier B]
    virtual ~ProcessJoinRequestJob(); // 0x004406B8 slot 0x00
    // 0x00440644 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x0073405C slot 0x14

    bool Startup(); // 0x00440488 | fefates:bytes [tier B]
    void Cleanup(); // 0x00440408 | fefates:bytes [tier B]
    // the joining station left
    void CancellationNotice(nn::pia::StationIndex stationIndex); // 0x00440320 | fefates:bytes [tier B]
    void SetJoiningStationData(nn::pia::StationIndex stationIndex, const nn::pia::common::StationAddress& address); // 0x00426F30

    // the steps
    common::ExecuteResult InitialStep(); // 0x0043F848 | fefates:bytes [tier B]
    common::ExecuteResult CheckApprovalJoin(); // 0x004400B8
    common::ExecuteResult SendJoinResponse(); // 0x0043FAC4
    common::ExecuteResult SendDenyingJoinResponse(); // 0x00440334 | fefates:bytes [tier B]
    common::ExecuteResult WaitResponseAck(); // 0x0043F96C | fefates:bytes [tier B]
    common::ExecuteResult JoinSucceeded(); // 0x0043F8DC

    common::Time m_Deadline;                        // 0x40, of the acknowledgements of the response
    s32 m_TimeoutMSec;                              // 0x48 (15000)
    u32 m_AckId;                                    // 0x4C, of the join request being processed
    u32 m_ResponseAckIds[RESPONSE_PART_NUM];        // 0x50
    u8* m_pResponseBuffers[RESPONSE_PART_NUM];      // 0x5C
    common::StationAddress m_JoiningStationAddress; // 0x68
    StationIndex m_JoiningStationIndex;             // 0x78 (253: none)
    bool m_IsProcessing;                            // 0x79, a request is being processed
    u8 m_RejectReason;                              // 0x7A (3)
    bool m_IsCanceled;                              // 0x7B, the joining station left
};
ASSERT_OFFSET(ProcessJoinRequestJob, m_AckId, 0x4C);
ASSERT_OFFSET(ProcessJoinRequestJob, m_JoiningStationAddress, 0x68);
ASSERT_OFFSET(ProcessJoinRequestJob, m_JoiningStationIndex, 0x78);
ASSERT_SIZE(ProcessJoinRequestJob, 0x80);
} // namespace session
} // namespace pia
} // namespace nn
