#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"

namespace nn {
namespace nex {
class NotificationEventHandler;
class ProtocolCallContext;
class StationURL;
template <typename T0> class qList;
} // namespace nex
namespace pia {
namespace common {
// a new object on the pia heap (the template name is from the symbols; ARMCC emits it out of line)
template <typename T> T* NewObj();
template <> nn::nex::qList<nn::nex::StationURL>* NewObj<nn::nex::qList<nn::nex::StationURL> >(); // 0x007E5078
} // namespace common
namespace inet {
// RTTI N2nn3pia4inet26NexProcessHostMigrationJobE @ 0x008CFA2C
// vtable 0x00900650 (vptr 0x00900658), offset_to_top 0, 20 entries
//
// The host migration of inet: the server (the matchmaking client) is asked who the host of the
// session is and is told about the new host. With host migration mode 2 (multi migration) the
// candidates send rank decisions to each other and a notification handler watches for the
// change of the host on the server. The step names are from the strings; the member names are
// ours.
class NexProcessHostMigrationJob : public ::nn::pia::session::ProcessHostMigrationJob
{
public:
    static const s32 RANK_DECISION_WAIT_MSEC_MIN = 5000;
    static const s32 RESELECT_WAIT_MSEC = 60000;
    static const s32 MULTI_TIMEOUT_MSEC = 15000;
    static const s32 OLD_HOST_CHECK_INTERVAL_MSEC = 6000;
    static const s32 OLD_HOST_DISCONNECTION_TIMEOUT_MSEC = 30000;
    static const s32 RELAY_TIMEOUT_MSEC = 30000;

    NexProcessHostMigrationJob(); // 0x0040F8C8
    virtual ~NexProcessHostMigrationJob(); // 0x0040F9FC slot 0x00
    // 0x0040F9EC slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F85C slot 0x14
    virtual bool IsFatalErrorOccur(); // 0x0040D240 slot 0x1C
    virtual void vf_0x20(); // 0x0040DEF8 slot 0x20
    virtual bool IsValidHostMigrationSetting(); // 0x0040DE98 slot 0x24
    virtual bool StartupImpl(bool isFromMessage, nn::pia::StationIndex stationIndex); // 0x0040D024 slot 0x2C
    virtual void CleanupImpl(); // 0x0040CFF0 slot 0x30
    virtual void CleanupStatus(); // 0x0040D0EC slot 0x34
    virtual bool CallUpdateSessionHost(unsigned int gatheringId); // 0x0040DA4C slot 0x38 | fefates:bytes-fuzzy
    virtual bool IsCompletedUpdateSessionHost(); // 0x0040E1B4 slot 0x3C
    virtual bool StartupMultiImpl(bool isFromMessage, unsigned short stationNumMax); // 0x0040D138 slot 0x44 | fefates:bytes-fuzzy
    virtual bool CheckWhetherReselectNewHost(); // 0x0040DD88 slot 0x48
    virtual bool CheckWhetherSendMigrationFinish(); // 0x0040EB9C slot 0x4C

    // the steps
    common::ExecuteResult InetDecideNextHost(); // 0x0040D26C
    common::ExecuteResult InetSendRankDecision(); // 0x0040D56C
    common::ExecuteResult InetWaitRankDecision(); // 0x0040D868
    common::ExecuteResult InetCleanupOldHostInfo(); // 0x0040DABC | fefates:bytes [tier B]
    common::ExecuteResult InetPrepareForBecomingHost(); // 0x0040DC24
    common::ExecuteResult InetGetMatchMakingClientHost(); // 0x0040DF20
    common::ExecuteResult InetMakeHostCandidateRanking(); // 0x0040E0B0
    common::ExecuteResult InetCheckOldHostDisconnection(); // 0x0040E1CC
    common::ExecuteResult InetWaitMatchMakingClientHost(); // 0x0040E504
    common::ExecuteResult InetCheckMatchMakingClientHost(); // 0x0040E958
    common::ExecuteResult InetPrepareForBecomingHostMulti(); // 0x0040EC08
    common::ExecuteResult WaitAfterPrepareForBecomingHost(); // 0x0040ED0C | fefates:bytes [tier B]
    common::ExecuteResult InetWaitCheckOldHostDisconnection(); // 0x0040EDAC
    common::ExecuteResult InetCleanupOldHostInfoOnMultiCandidate(); // 0x0040F1D4 | fefates:bytes [tier B]
    common::ExecuteResult InetWaitMatchMakingClientHostIsUpdated(); // 0x0040F2B8
    common::ExecuteResult InetCheckMatchMakingClientHostIsUpdated(); // 0x0040F474
    common::ExecuteResult InetGetMatchMakingClientHostLastConfirmation(); // 0x0040F568
    common::ExecuteResult InetWaitMatchMakingClientHostLastConfirmation(); // 0x0040F65C

    // (inline; names are ours) the call of the host URLs; the principal of the first one
    bool GetSessionURLs();
    u32 GetHostPrincipalId() const;
    void StopResendingAll();

    nn::nex::ProtocolCallContext* m_pNexCallContext;   // 0xA8, of the matchmaking client
    u32 m_Unknown0xAC;                                 // 0xAC
    common::Time m_RankDecisionDeadline;               // 0xB0
    s32 m_RankDecisionWaitMSec;                        // 0xB8
    common::Time m_ReselectDeadline;                   // 0xC0, the host is selected again before it
    s32 m_ReselectWaitMSec;                            // 0xC8 (60000)
    nn::nex::qList<nn::nex::StationURL>* m_pHostUrls;  // 0xCC, of the host on the server
    u32 m_ResendIds[STATION_INDEX_MAX + 1];            // 0xD0, of the rank decisions
    bool m_IsFromMessage;                              // 0x100, of StartupMulti
    bool m_IsSessionHostChanged;                       // 0x101, by the notification handler
    nn::nex::NotificationEventHandler* m_pNotificationEventHandler; // 0x104
    common::Time m_OldHostCheckTime;                   // 0x108
    common::Time m_OldHostDisconnectionDeadline;       // 0x110
};
ASSERT_OFFSET(NexProcessHostMigrationJob, m_pNexCallContext, 0xA8);
ASSERT_OFFSET(NexProcessHostMigrationJob, m_pHostUrls, 0xCC);
ASSERT_OFFSET(NexProcessHostMigrationJob, m_IsFromMessage, 0x100);
ASSERT_OFFSET(NexProcessHostMigrationJob, m_OldHostCheckTime, 0x108);
ASSERT_SIZE(NexProcessHostMigrationJob, 0x118);
} // namespace inet
} // namespace pia
} // namespace nn
