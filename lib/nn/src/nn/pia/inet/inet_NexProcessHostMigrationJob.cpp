#include "nn/pia/inet/inet_NexProcessHostMigrationJob.h"
#include <new>
#include <string.h>
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_MatchMakingClient.h"
#include "nn/nex/nex_NotificationEvent.h"
#include "nn/nex/nex_NotificationEventHandler.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qResult.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_ResendingMessageManager.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_Transport.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace common {
// 0x007E5078
template <>
nn::nex::qList<nn::nex::StationURL>* nn::pia::common::NewObj<nn::nex::qList<nn::nex::StationURL> >()
{
    void* p = pead::AllocMemory(sizeof(nex::qList<nex::StationURL>), HeapManager::GetHeap());
    if (p == nullptr) {
        return nullptr;
    }
    return ::new (p) nex::qList<nex::StationURL>();
}
} // namespace common

namespace inet {
namespace {
// vtable 0x008B1518 (vptr 0x008B1520), offset_to_top 0, 3 entries
//
// Watches the notifications of the server for the change of the host of the session (type 110)
// during a multi migration. The class name is from the RTTI; the member name is ours.
class NexNotificationEventHandler4Pia : public nn::nex::NotificationEventHandler
{
public:
    explicit NexNotificationEventHandler4Pia(NexProcessHostMigrationJob* pJob) : m_pJob(pJob) {}
    virtual ~NexNotificationEventHandler4Pia(); // 0x0056EC8C slot 0x00
    // 0x0056EC88 slot 0x04 (deleting dtor)
    virtual void ProcessNotificationEvent(const nn::nex::NotificationEvent& event); // 0x0056EC44 slot 0x08

    NexProcessHostMigrationJob* m_pJob; // 0x4
};

// the nex notification of the change of the host
const u32 NOTIFICATION_TYPE_HOST_CHANGED = 110;

inline common::Time GetTimeAfter(s64 msec)
{
    return common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
}

inline bool IsDispatchTimeBefore(const common::Time& time)
{
    return common::Scheduler::s_pInstance->m_DispatchTime < time;
}
} // namespace

// 0x0056EC44
void NexNotificationEventHandler4Pia::ProcessNotificationEvent(const nn::nex::NotificationEvent& event)
{
    if (event.m_Type / 1000 == NOTIFICATION_TYPE_HOST_CHANGED && NexFacade::s_pInstance->m_Unknown0x10 == event.m_Param1) {
        m_pJob->m_IsSessionHostChanged = true;
    }
}

// 0x0056EC8C
// 0x0056EC88 (deleting dtor)
NexNotificationEventHandler4Pia::~NexNotificationEventHandler4Pia()
{
    // empty (in the original too)
}

// (inline; name is ours)
DECOMP_ALWAYS_INLINE bool nn::pia::inet::NexProcessHostMigrationJob::GetSessionURLs()
{
    u32 gatheringId = NexFacade::s_pInstance->m_Unknown0x10;
    if (gatheringId == 0) {
        return false;
    }
    if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
    }
    m_pNexCallContext->Reset();
    return NexFacade::s_pInstance->m_pMatchMakingClient->GetSessionURLs(m_pNexCallContext, gatheringId, m_pHostUrls);
}

// (inline; name is ours)
DECOMP_ALWAYS_INLINE u32 nn::pia::inet::NexProcessHostMigrationJob::GetHostPrincipalId() const
{
    return m_pHostUrls->front().GetPrincipalID();
}

// (inline; name is ours)
DECOMP_ALWAYS_INLINE void nn::pia::inet::NexProcessHostMigrationJob::StopResendingAll()
{
    for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
        if (m_ResendIds[i] != 0) {
            transport::ResendingMessageManager::s_pInstance->StopResending(m_ResendIds[i]);
            m_ResendIds[i] = 0;
        }
    }
}

// 0x0040CFF0
void nn::pia::inet::NexProcessHostMigrationJob::CleanupImpl()
{
    if (m_IsMultiMigration) {
        NexFacade::s_pInstance->UnregisterNexNotificationEventHandler4Pia(m_pNotificationEventHandler);
        m_IsSessionHostChanged = false;
    }
}

// 0x0040D024
bool nn::pia::inet::NexProcessHostMigrationJob::StartupImpl(bool isFromMessage, nn::pia::StationIndex stationIndex)
{
    if (isFromMessage) {
        m_OldHostStationIndex = session::Mesh::s_pInstance->m_HostStationIndex;
        m_NewHostStationIndex = stationIndex;
        SetStep(&NexProcessHostMigrationJob::InetCleanupOldHostInfo, "NexProcessHostMigrationJob::InetCleanupOldHostInfo");
    } else {
        SetStep(&NexProcessHostMigrationJob::InetDecideNextHost, "NexProcessHostMigrationJob::InetDecideNextHost");
    }
    return true;
}

// 0x0040D0EC
void nn::pia::inet::NexProcessHostMigrationJob::CleanupStatus()
{
    StopResendingAll();
    ProcessHostMigrationJob::CleanupStatus();
}

// 0x0040D138 | fefates:bytes-fuzzy
bool nn::pia::inet::NexProcessHostMigrationJob::StartupMultiImpl(bool isFromMessage, unsigned short)
{
    s32 waitMSec = m_TimeoutMSec - RANK_DECISION_WAIT_MSEC_MIN;
    if (waitMSec < RANK_DECISION_WAIT_MSEC_MIN) {
        waitMSec = RANK_DECISION_WAIT_MSEC_MIN;
    }
    m_RankDecisionWaitMSec = waitMSec;
    m_ReselectDeadline = GetTimeAfter(m_ReselectWaitMSec);
    m_OldHostStationIndex = session::Mesh::s_pInstance->m_HostStationIndex;
    if (isFromMessage) {
        SetStep(&NexProcessHostMigrationJob::InetCleanupOldHostInfoOnMultiCandidate, "NexProcessHostMigrationJob::InetCleanupOldHostInfoOnMultiCandidate");
    } else {
        SetStep(&NexProcessHostMigrationJob::InetMakeHostCandidateRanking, "NexProcessHostMigrationJob::InetMakeHostCandidateRanking");
    }
    m_IsFromMessage = isFromMessage;
    return true;
}

// 0x0040D240
bool nn::pia::inet::NexProcessHostMigrationJob::IsFatalErrorOccur()
{
    return m_IsSessionHostChanged && session::Session::s_pInstance == nullptr;
}

// 0x0040D26C
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetDecideNextHost()
{
    // the first other valid station is the candidate
    m_NewHostStationIndex = STATION_INDEX_UNIDENTIFIED;
    for (s32 i = 0; i <= STATION_INDEX_MAX; i++) {
        if (session::Mesh::s_pInstance->m_HostStationIndex != static_cast<StationIndex>(i) &&
            session::Mesh::s_pInstance->CheckStationIndexIsValid(static_cast<StationIndex>(i))) {
            m_NewHostStationIndex = static_cast<StationIndex>(i);
            break;
        }
    }
    u32 next = DecideNextHostCommonProc();
    bool isRelayed = false;
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (pRelayRouteManager != nullptr) {
        StationIndex relay;
        if (pRelayRouteManager->GetRelayRoute(m_OldHostStationIndex, m_NewHostStationIndex, &relay).IsFailure()) {
            SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_NewHostStationIndex != relay) {
            // the new host was reached through a relay: more time
            isRelayed = true;
            m_Deadline += common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * RELAY_TIMEOUT_MSEC);
        }
    }
    if (next == 1) {
        if (isRelayed) {
            SetStep(&NexProcessHostMigrationJob::InetGetMatchMakingClientHost, "NexProcessHostMigrationJob::InetGetMatchMakingClientHost");
            m_OldHostCheckTime = GetTimeAfter(OLD_HOST_CHECK_INTERVAL_MSEC);
            m_OldHostDisconnectionDeadline = GetTimeAfter(OLD_HOST_DISCONNECTION_TIMEOUT_MSEC);
        } else {
            SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHost, "NexProcessHostMigrationJob::InetPrepareForBecomingHost");
        }
    } else if (next == 2) {
        SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
    } else {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040D56C
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetSendRankDecision()
{
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    StationIndex localIndex = pMesh->m_LocalStationIndex;
    if (localIndex > STATION_INDEX_MAX) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_MeshVersion == 0) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // the rank of the local station and the number of candidates
    u32 rank = STATION_INDEX_MAX + 1;
    u32 candidateNum = 0;
    for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
        if (m_Ranking[i] == localIndex) {
            rank = i;
        }
        if (m_Ranking[i] != STATION_INDEX_UNIDENTIFIED) {
            candidateNum++;
        }
    }
    // the higher ranks decide first: the wait grows with the rank
    const common::Time& dispatchTime = common::Scheduler::s_pInstance->m_DispatchTime;
    s32 waitMSec;
    if (rank == 0) {
        waitMSec = 0;
    } else {
        u32 half = candidateNum >> 1;
        u32 unitMSec = static_cast<u32>(m_RankDecisionWaitMSec) / (half * (half - 1) + (candidateNum & 1) + half);
        if (rank < half) {
            u32 sum = 0;
            for (u32 j = 0; j < rank; j++) {
                sum += (half - j) * 2 - 2;
            }
            waitMSec = unitMSec * sum;
        } else {
            waitMSec = unitMSec * (rank - candidateNum + 1) + m_RankDecisionWaitMSec;
        }
    }
    m_RankDecisionDeadline = dispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * waitMSec);
    m_Deadline = GetTimeAfter(m_TimeoutMSec);
    for (s32 i = 0; i <= STATION_INDEX_MAX; i++) {
        StationIndex index = static_cast<StationIndex>(i);
        if (index != localIndex && index != m_OldHostStationIndex && pMesh->CheckStationIndexIsValid(index)) {
            u32 resendId = 0;
            if (pMesh->m_pMeshProtocol->SendMultiMigrationRankDecision(index, m_Deadline.m_Tick, &resendId)) {
                m_ResendIds[i] = resendId;
            }
        }
    }
    m_IsWaitingGreeting = true;
    m_GreetingStationIndex = STATION_INDEX_UNIDENTIFIED;
    SetStep(&NexProcessHostMigrationJob::InetWaitRankDecision, "NexProcessHostMigrationJob::InetWaitRankDecision");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040D868
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetWaitRankDecision()
{
    if (session::Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_GreetingStationIndex != STATION_INDEX_UNIDENTIFIED) {
        SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
        if (m_RankDecisions[i] == 1) {
            // a higher station decided to become the host
            SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    if (IsDispatchTimeBefore(m_RankDecisionDeadline)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&NexProcessHostMigrationJob::InetGetMatchMakingClientHost, "NexProcessHostMigrationJob::InetGetMatchMakingClientHost");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040DA4C | fefates:bytes-fuzzy
bool nn::pia::inet::NexProcessHostMigrationJob::CallUpdateSessionHost(unsigned int gatheringId)
{
    if (gatheringId == 0) {
        gatheringId = NexFacade::s_pInstance->m_Unknown0x10;
        if (gatheringId == 0) {
            return false;
        }
    }
    if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
    }
    m_pNexCallContext->Reset();
    return NexFacade::s_pInstance->m_pMatchMakingClient->UpdateSessionHost(m_pNexCallContext, gatheringId, true);
}

// 0x0040DABC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetCleanupOldHostInfo()
{
    if (!CleanupOldHostInfoCommonProc()) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    u32 next = DecideNextHostCommonProc();
    if (next == 1) {
        SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHost, "NexProcessHostMigrationJob::InetPrepareForBecomingHost");
    } else if (next == 2) {
        SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
    } else {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040DC24
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetPrepareForBecomingHost()
{
    if (!PrepareForBecomingHostCommonProc() || NexFacade::s_pInstance->m_Unknown0x10 == 0) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (!CallUpdateSessionHost(NexFacade::s_pInstance->m_Unknown0x10)) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession != nullptr && pSession->GetStatus() == session::Session::STATUS_JOINT) {
        pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x74(pSession->m_SessionIds[pSession->m_CurrentIndex]);
    }
    SetStep(&NexProcessHostMigrationJob::WaitAfterPrepareForBecomingHost, "NexProcessHostMigrationJob::WaitAfterPrepareForBecomingHost");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040DD88
bool nn::pia::inet::NexProcessHostMigrationJob::CheckWhetherReselectNewHost()
{
    if (!m_IsMultiMigration) {
        return false;
    }
    if (!IsDispatchTimeBefore(m_ReselectDeadline)) {
        return false;
    }
    // the ranking starts again without the station that did not greet
    if (m_GreetingStationIndex != STATION_INDEX_UNIDENTIFIED) {
        m_OldHostStationIndex = m_GreetingStationIndex;
    }
    m_IsWaitingGreeting = false;
    m_IsWaitingMigrationFinish = false;
    m_IsHostAllGreetingAnswered = false;
    m_GreetingStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_NewHostStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_Unknown0x6E++;
    SetStep(&NexProcessHostMigrationJob::InetSendRankDecision, "NexProcessHostMigrationJob::InetSendRankDecision");
    StopResendingAll();
    memset(m_RankDecisions, 0, sizeof(m_RankDecisions));
    m_IsFromMessage = false;
    return true;
}

// 0x0040DE98
bool nn::pia::inet::NexProcessHostMigrationJob::IsValidHostMigrationSetting()
{
    NexFacade* pFacade = NexFacade::s_pInstance;
    if ((m_IsMultiMigration && pFacade->m_pNgsBridge == nullptr) || pFacade->m_Unknown0x10 == 0) {
        return false;
    }
    if (m_IsMultiMigration) {
        m_IsSessionHostChanged = false;
        pFacade->RegisterNexNotificationEventHandler4Pia(m_pNotificationEventHandler);
    }
    return true;
}

// 0x0040DEF8
void nn::pia::inet::NexProcessHostMigrationJob::vf_0x20()
{
    SetStep(&NexProcessHostMigrationJob::InetCheckMatchMakingClientHostIsUpdated, "NexProcessHostMigrationJob::InetCheckMatchMakingClientHostIsUpdated");
}

// 0x0040DF20
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetGetMatchMakingClientHost()
{
    if (GetSessionURLs()) {
        if (m_IsMultiMigration) {
            SetStep(&NexProcessHostMigrationJob::InetWaitMatchMakingClientHost, "NexProcessHostMigrationJob::InetWaitMatchMakingClientHost");
        } else {
            SetStep(&NexProcessHostMigrationJob::InetCheckMatchMakingClientHost, "NexProcessHostMigrationJob::InetCheckMatchMakingClientHost");
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040E0B0
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetMakeHostCandidateRanking()
{
    if (MakeHostCandidateRanking(m_OldHostStationIndex, m_Ranking, &m_MeshVersion, &m_DirectionsVersion, false).IsFailure()) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexProcessHostMigrationJob::InetSendRankDecision, "NexProcessHostMigrationJob::InetSendRankDecision");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040E1B4
bool nn::pia::inet::NexProcessHostMigrationJob::IsCompletedUpdateSessionHost()
{
    return m_pNexCallContext->m_State != nex::CallContext::STATE_CALL_IN_PROGRESS;
}

// 0x0040E1CC
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetCheckOldHostDisconnection()
{
    if (session::Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (IsDispatchTimeBefore(m_OldHostCheckTime)) {
        if (!m_IsMultiMigration) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_GreetingStationIndex != STATION_INDEX_UNIDENTIFIED) {
            SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        bool isLower = false;
        for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
            if (m_RankDecisions[i] == 1) {
                SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
                return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
            }
            if (m_RankDecisions[i] == 2) {
                isLower = true;
            }
        }
        if (isLower) {
            SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti, "NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the host on the server again
    if (GetSessionURLs()) {
        if (m_IsMultiMigration) {
            m_Deadline += common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * OLD_HOST_CHECK_INTERVAL_MSEC);
        }
        SetStep(&NexProcessHostMigrationJob::InetWaitCheckOldHostDisconnection, "NexProcessHostMigrationJob::InetWaitCheckOldHostDisconnection");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040E504
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetWaitMatchMakingClientHost()
{
    nex::ProtocolCallContext* pContext = m_pNexCallContext;
    if (pContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        if (session::Mesh::s_pInstance->CheckJoined() != common::RESULT_NOT_JOINED && IsDispatchTimeBefore(m_Deadline)) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_GreetingStationIndex != STATION_INDEX_UNIDENTIFIED) {
        SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    bool isLower = false;
    for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
        if (m_RankDecisions[i] == 1) {
            SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_RankDecisions[i] == 2) {
            isLower = true;
        }
    }
    // who is the host on the server
    bool isBecomingHost = false;
    nex::qResult result = pContext->m_Result;
    if (!result) {
        isBecomingHost = true;
    } else {
        u32 principalId = GetHostPrincipalId();
        if (principalId != 0) {
            StationIndex index = GetStationIndexByPrincipalId(principalId);
            if (index <= STATION_INDEX_MAX) {
                if (m_OldHostStationIndex == index) {
                    if (m_IsFromMessage || isLower) {
                        isBecomingHost = true;
                    } else {
                        // the old host is still the host: it may be gone soon
                        m_OldHostCheckTime = GetTimeAfter(OLD_HOST_CHECK_INTERVAL_MSEC);
                        m_OldHostDisconnectionDeadline = GetTimeAfter(OLD_HOST_CHECK_INTERVAL_MSEC * 5);
                        SetStep(&NexProcessHostMigrationJob::InetCheckOldHostDisconnection, "NexProcessHostMigrationJob::InetCheckOldHostDisconnection");
                        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
                    }
                } else if (session::Mesh::s_pInstance->m_LocalStationIndex == index) {
                    isBecomingHost = true;
                } else if (session::Mesh::s_pInstance->CheckStationIndexIsValid(index)) {
                    // another station is the host: its greeting
                    m_ReselectDeadline = common::Scheduler::s_pInstance->m_DispatchTime;
                    SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
                    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
                }
            }
        }
    }
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (isBecomingHost && !isLower && common::IsValidPointer(pRelayRouteManager)) {
        // the old host must be reached directly
        StationIndex relay;
        if (pRelayRouteManager->GetRelayRoute(session::Mesh::s_pInstance->m_LocalStationIndex, m_OldHostStationIndex, &relay).IsFailure() ||
            m_OldHostStationIndex != relay) {
            SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    if (!isBecomingHost) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti, "NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040E958
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetCheckMatchMakingClientHost()
{
    if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        if (session::Mesh::s_pInstance->CheckJoined() != common::RESULT_NOT_JOINED && IsDispatchTimeBefore(m_Deadline)) {
            return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, 100);
        }
        m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    nex::qResult result = m_pNexCallContext->m_Result;
    if (result) {
        SetStep(&NexProcessHostMigrationJob::InetCheckOldHostDisconnection, "NexProcessHostMigrationJob::InetCheckOldHostDisconnection");
        m_OldHostCheckTime = GetTimeAfter(OLD_HOST_CHECK_INTERVAL_MSEC);
        m_OldHostDisconnectionDeadline = GetTimeAfter(OLD_HOST_CHECK_INTERVAL_MSEC * 5);
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHost, "NexProcessHostMigrationJob::InetPrepareForBecomingHost");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040EB9C
bool nn::pia::inet::NexProcessHostMigrationJob::CheckWhetherSendMigrationFinish()
{
    if (!m_IsMultiMigration) {
        return true;
    }
    m_Deadline += common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * 2000);
    SetStep(&NexProcessHostMigrationJob::InetGetMatchMakingClientHostLastConfirmation, "NexProcessHostMigrationJob::InetGetMatchMakingClientHostLastConfirmation");
    return false;
}

// 0x0040EC08
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti()
{
    m_NewHostStationIndex = session::Mesh::s_pInstance->m_LocalStationIndex;
    m_IsWaitingGreeting = false;
    StopResendingAll();
    DisconnectStation(m_OldHostStationIndex);
    m_Deadline = common::Time(m_Deadline.m_Tick - common::TimeSpan::GetTicksPerMSec().GetTick() * 3000);
    SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHost, "NexProcessHostMigrationJob::InetPrepareForBecomingHost");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040ED0C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::WaitAfterPrepareForBecomingHost()
{
    if (IsCompletedUpdateSessionHost()) {
        UpdateHostStationIndexByLocalStationIndex();
        SetStep(&ProcessHostMigrationJob::SendGreetingMessage, "NexProcessHostMigrationJob::SendGreetingMessage");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040EDAC
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetWaitCheckOldHostDisconnection()
{
    if (session::Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED || !IsDispatchTimeBefore(m_Deadline)) {
        if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
        }
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_IsMultiMigration) {
        if (m_GreetingStationIndex != STATION_INDEX_UNIDENTIFIED) {
            if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
                m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
            }
            SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        bool isLower = false;
        for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
            if (m_RankDecisions[i] == 1) {
                if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
                    m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
                }
                SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
                return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
            }
            if (m_RankDecisions[i] == 2) {
                isLower = true;
            }
        }
        if (isLower) {
            if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
                m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
            }
            SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti, "NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    nex::qResult result = m_pNexCallContext->m_Result;
    if (!result) {
        // no host on the server: the local station takes it
        if (m_IsMultiMigration) {
            SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti, "NexProcessHostMigrationJob::InetPrepareForBecomingHostMulti");
        } else {
            SetStep(&NexProcessHostMigrationJob::InetPrepareForBecomingHost, "NexProcessHostMigrationJob::InetPrepareForBecomingHost");
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    u32 principalId = GetHostPrincipalId();
    if (principalId != 0) {
        StationIndex index = GetStationIndexByPrincipalId(principalId);
        if (index <= STATION_INDEX_MAX && m_OldHostStationIndex == index && IsDispatchTimeBefore(m_OldHostDisconnectionDeadline)) {
            // the old host is still the host: check again
            m_OldHostCheckTime = GetTimeAfter(OLD_HOST_CHECK_INTERVAL_MSEC);
            SetStep(&NexProcessHostMigrationJob::InetCheckOldHostDisconnection, "NexProcessHostMigrationJob::InetCheckOldHostDisconnection");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040F1D4 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetCleanupOldHostInfoOnMultiCandidate()
{
    if (CleanupOldHostInfoCommonProc()) {
        SetStep(&NexProcessHostMigrationJob::InetSendRankDecision, "NexProcessHostMigrationJob::InetSendRankDecision");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040F2B8
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetWaitMatchMakingClientHostIsUpdated()
{
    if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        if (session::Mesh::s_pInstance->CheckJoined() != common::RESULT_NOT_JOINED && IsDispatchTimeBefore(m_Deadline)) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_GreetingStationIndex == STATION_INDEX_UNIDENTIFIED) {
        nex::qResult result = m_pNexCallContext->m_Result;
        if (result) {
            SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    SetStep(&ProcessHostMigrationJob::WaitNewHostGreeting, "NexProcessHostMigrationJob::WaitNewHostGreeting");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040F474
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetCheckMatchMakingClientHostIsUpdated()
{
    if (GetSessionURLs()) {
        SetStep(&NexProcessHostMigrationJob::InetWaitMatchMakingClientHostIsUpdated, "NexProcessHostMigrationJob::InetWaitMatchMakingClientHostIsUpdated");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040F568
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetGetMatchMakingClientHostLastConfirmation()
{
    if (GetSessionURLs()) {
        SetStep(&NexProcessHostMigrationJob::InetWaitMatchMakingClientHostLastConfirmation,
                "NexProcessHostMigrationJob::InetWaitMatchMakingClientHostLastConfirmation");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040F65C
nn::pia::common::ExecuteResult nn::pia::inet::NexProcessHostMigrationJob::InetWaitMatchMakingClientHostLastConfirmation()
{
    if (m_pNexCallContext->m_State == nex::CallContext::STATE_CALL_IN_PROGRESS) {
        if (session::Mesh::s_pInstance->CheckJoined() != common::RESULT_NOT_JOINED && IsDispatchTimeBefore(m_Deadline)) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        m_pNexCallContext->Cancel(nex::CallContext::STATE_CANCELLED);
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    nex::qResult result = m_pNexCallContext->m_Result;
    if (!result) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (!IsDispatchTimeBefore(m_Deadline)) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // the server knows the local station as the host
    u32 principalId = GetHostPrincipalId();
    if (principalId != 0) {
        StationIndex index = transport::StationConnectionInfoTable::s_pInstance->GetStationIndexByPrincipalID(principalId);
        if (index <= STATION_INDEX_MAX && session::Mesh::s_pInstance->m_LocalStationIndex == index) {
            SetStep(&ProcessHostMigrationJob::SendMigrationFinish, "NexProcessHostMigrationJob::SendMigrationFinish");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "NexProcessHostMigrationJob::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040F8C8
nn::pia::inet::NexProcessHostMigrationJob::NexProcessHostMigrationJob()
    : m_RankDecisionDeadline(), m_ReselectDeadline(), m_ReselectWaitMSec(RESELECT_WAIT_MSEC), m_OldHostCheckTime(), m_OldHostDisconnectionDeadline()
{
    void* pBuffer = pead::AllocMemory(sizeof(nex::ProtocolCallContext), common::HeapManager::GetHeap());
    m_pNexCallContext = ::new (pBuffer) nex::ProtocolCallContext();
    if (m_IsMultiMigration) {
        m_pHostUrls = common::NewObj<nex::qList<nex::StationURL> >();
        for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
            m_ResendIds[i] = 0;
        }
        m_TimeoutMSec = MULTI_TIMEOUT_MSEC;
        pBuffer = pead::AllocMemory(sizeof(NexNotificationEventHandler4Pia), common::HeapManager::GetHeap());
        m_pNotificationEventHandler = ::new (pBuffer) NexNotificationEventHandler4Pia(this);
    } else {
        if (transport::Transport::s_pInstance->m_pRelayRouteManager != nullptr) {
            m_pHostUrls = common::NewObj<nex::qList<nex::StationURL> >();
        } else {
            m_pHostUrls = nullptr;
        }
        m_pNotificationEventHandler = nullptr;
    }
    m_IsSessionHostChanged = false;
}

// 0x0040F9FC
// 0x0040F9EC (deleting dtor)
nn::pia::inet::NexProcessHostMigrationJob::~NexProcessHostMigrationJob()
{
    if (m_pNexCallContext != nullptr) {
        m_pNexCallContext->~ProtocolCallContext();
        pead::FreeMemory(m_pNexCallContext);
        m_pNexCallContext = nullptr;
    }
    if (m_pHostUrls != nullptr) {
        m_pHostUrls->clear();
        if (m_pHostUrls != nullptr) {
            m_pHostUrls->~qList();
            pead::FreeMemory(m_pHostUrls);
        }
        m_pHostUrls = nullptr;
    }
    if (m_pNotificationEventHandler != nullptr) {
        NexFacade::s_pInstance->UnregisterNexNotificationEventHandler4Pia(m_pNotificationEventHandler);
        if (m_pNotificationEventHandler != nullptr) {
            m_pNotificationEventHandler->~NotificationEventHandler();
            pead::FreeMemory(m_pNotificationEventHandler);
        }
        m_pNotificationEventHandler = nullptr;
    }
}

// 0x0072F85C
void nn::pia::inet::NexProcessHostMigrationJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
