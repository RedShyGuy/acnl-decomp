#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace session {
namespace {
typedef common::ObjList<StationId> StationIdList;
typedef common::ObjList<transport::StationIdTable::Entry> EntryList;

// the milliseconds since the time by the scheduler (name is ours)
inline u32 GetElapsedMSec(const common::Time& time)
{
    return (common::Scheduler::s_pInstance->m_DispatchTime - time).m_Tick / common::TimeSpan::GetTicksPerMSec().m_Tick;
}
} // namespace

// 0x004320F4
void nn::pia::session::JointSessionJob::FreeStationIdList()
{
    if (m_pStationIdNodeBuffer != nullptr) {
        m_StationIdList.Clear();
        if (m_pStationIdNodeBuffer != nullptr) {
            pead::FreeMemory(m_pStationIdNodeBuffer);
        }
        m_pStationIdNodeBuffer = nullptr;
    }
}

// 0x00432178
void nn::pia::session::JointSessionJob::AllocateStationIdList()
{
    u32 stationNum = transport::Transport::s_pInstance->m_StationNum;
    u64* pBuffer = common::NewArray<u64>((stationNum * sizeof(StationIdList::Node) + sizeof(u64) - 1) / sizeof(u64));
    m_pStationIdNodeBuffer = pBuffer;
    m_StationIdList.Initialize(reinterpret_cast<StationIdList::Node*>(pBuffer), stationNum);
}

// 0x0043224C (name is ours)
bool nn::pia::session::JointSessionJob::MarkStationLeft(const nn::pia::StationId& stationId)
{
    if (m_StationId == stationId) {
        m_Unknown0x9A = true;
        return true;
    }
    return false;
}

// 0x00432284 (name is ours)
nn::Result nn::pia::session::JointSessionJob::ReceiveSessionInfo(u8 phase, u32 sessionId, u32 ownerPrincipalId, u8 signatureMode, const u8* pSignatureKey,
                                                                 u8 signatureKeySize, const StationId& stationId)
{
    if (m_Phase == 0 || m_Phase != phase || m_StationId != stationId) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = vf_0x3C(phase, sessionId);
    if (result.IsFailure()) {
        return result;
    }
    // the owner and the signature of the other session
    if (ownerPrincipalId != 0 && signatureMode != 0xFF && pSignatureKey != nullptr && signatureKeySize != 0) {
        Session* pSession = Session::s_pInstance;
        CommonMatchmakeSession* pOtherSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
        if (pOtherSession->vf_0x90() == 0) {
            pOtherSession->vf_0x94(ownerPrincipalId);
        }
        pOtherSession->SetSignatureSetting(static_cast<common::SignatureSetting::Mode>(signatureMode), pSignatureKey, signatureKeySize);
    }
    m_JointSessionId = sessionId;
    return nn::Result();
}

// 0x00432388
void nn::pia::session::JointSessionJob::vf_0x58(u32, u32)
{
    // empty (in the original too)
}

// 0x0043238C (name is ours)
nn::Result nn::pia::session::JointSessionJob::Start(u8 phase, u8 messageType, const StationId* pStationIds, u32 stationNum, const StationId& stationId)
{
    if (phase == 0 || phase >= 11 || !common::IsValidPointer(pStationIds) || stationNum == 0 || stationNum > m_StationIdList.GetCapacity() ||
        stationId == GetStationIdOfIndex253()) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    Session* pSession = Session::s_pInstance;
    if (stationId == pSession->m_LocalStationId) {
        return common::RESULT_INVALID_STATE;
    }
    if (phase == 9 || phase == 10) {
        // the joint session goes on with the other session
        if (pSession->m_State != 4 || pSession->m_SessionIds[pSession->m_CurrentIndex] == 0) {
            return common::RESULT_INVALID_STATE;
        }
        m_Unknown0x9F = true;
    } else {
        if (pSession->m_State != 2 || pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0) {
            return common::RESULT_INVALID_STATE;
        }
        pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->Cleanup();
        m_Unknown0x9F = false;
    }
    if (Session::s_pInstance->m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = vf_0x38(phase, messageType, pStationIds, stationNum, stationId);
    if (result.IsFailure()) {
        return result;
    }
    m_Unknown0x98 = 0;
    if (phase == 9 && messageType == 8) {
        m_StationId = GetStationIdOfIndex253();
    } else {
        m_StationId = stationId;
    }
    m_Unknown0x9A = 0;
    m_Phase = phase;
    m_Unknown0x9D = 0;
    m_IsFailed = false;
    m_FailureResult = nn::Result();
    m_StartTime = common::Scheduler::s_pInstance->m_DispatchTime;
    m_Unknown0xC0 = 0;
    Reset(true);
    return nn::Result();
}

// 0x00432594 (name is ours)
nn::Result nn::pia::session::JointSessionJob::Restart(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId& stationId)
{
    if (phase != 9 || !common::IsValidPointer(pStationIds) || stationNum == 0 || stationNum > m_StationIdList.GetCapacity() ||
        stationId == GetStationIdOfIndex253()) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (stationId == Session::s_pInstance->m_LocalStationId) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = vf_0x40(phase, pStationIds, stationNum, stationId);
    if (result.IsFailure()) {
        return result;
    }
    m_Unknown0x98 = 0;
    m_StationId = stationId;
    m_Unknown0x9A = 0;
    m_Phase = phase;
    return nn::Result();
}

// 0x0043268C (name is ours)
nn::Result nn::pia::session::JointSessionJob::ReceiveAck20(u8 phase, const StationId& stationId)
{
    if (m_Phase == 0 || m_Phase != phase) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = vf_0x48(phase, stationId);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x004326D0
void nn::pia::session::JointSessionJob::vf_0x4C(const nn::pia::StationId&)
{
    // empty (in the original too)
}

// 0x004326D4
void nn::pia::session::JointSessionJob::vf_0x5C(u32)
{
    // empty (in the original too)
}

// 0x004326D8
void nn::pia::session::JointSessionJob::vf_0x54(u32, u32)
{
    // empty (in the original too)
}

// 0x004326DC (name is ours)
nn::Result nn::pia::session::JointSessionJob::ReceiveAck10(u8 phase, const StationId& stationId)
{
    if (m_Phase == 0 || m_Phase != phase || m_StationId != stationId) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = vf_0x44(phase, stationId);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x00432740
void nn::pia::session::JointSessionJob::vf_0x50(u32, u32)
{
    // empty (in the original too)
}

// 0x00432744 (name is ours)
void nn::pia::session::JointSessionJob::RemoveUnknownStations()
{
    StationId stationIds[16];
    u32 num = 0;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    StationIdStatusTable* pStatusTable = Session::s_pInstance->m_pStationIdStatusTable;
    for (EntryList::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End(); pNode = EntryList::Advance(pNode)) {
        bool isFound = false;
        for (StationIdList::Node* pIdNode = m_StationIdList.Begin(); pIdNode != m_StationIdList.End(); pIdNode = StationIdList::Advance(pIdNode)) {
            if (pNode->m_Value.m_StationId == pIdNode->m_Value) {
                isFound = true;
                break;
            }
        }
        if (!isFound && !pStatusTable->IsNotified(pNode->m_Value.m_StationId)) {
            stationIds[num++] = pNode->m_Value.m_StationId;
        }
    }
    for (u32 i = 0; i < num; i++) {
        pTable->Remove(stationIds[i]);
        pStatusTable->Remove(stationIds[i]);
    }
}

// 0x00432870 (name is ours)
void nn::pia::session::JointSessionJob::BeginJointSession()
{
    Session* pSession = Session::s_pInstance;
    StationId stationIds[16];
    u32 leftNum = 0;
    u32 stayingNum = 0;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    // the stations that are not in the joint session
    if ((m_Phase == 9 || m_Phase == 10) && m_Unknown0x98) {
        StationIdStatusTable* pStatusTable = pSession->m_pStationIdStatusTable;
        for (EntryList::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End(); pNode = EntryList::Advance(pNode)) {
            u32 sessionId;
            if (!pStatusTable->GetSessionId(pNode->m_Value.m_StationId, &sessionId) || pSession->m_SessionIds[pSession->m_CurrentIndex] != sessionId) {
                stationIds[leftNum++] = pNode->m_Value.m_StationId;
            }
        }
    } else {
        for (EntryList::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End(); pNode = EntryList::Advance(pNode)) {
            bool isFound = false;
            for (StationIdList::Node* pIdNode = m_StationIdList.Begin(); pIdNode != m_StationIdList.End();
                 pIdNode = StationIdList::Advance(pIdNode)) {
                if (pNode->m_Value.m_StationId == pIdNode->m_Value) {
                    isFound = true;
                    break;
                }
            }
            if (isFound) {
                stayingNum++;
            } else {
                stationIds[leftNum++] = pNode->m_Value.m_StationId;
            }
        }
    }
    for (u32 i = 0; i < leftNum; i++) {
        if (pSession->m_pStationIdStatusTable->IsNotified(stationIds[i])) {
            pSession->NotifyEvent(Session::EVENT_TYPE_LEAVE, stationIds[i]);
            pSession->m_pStationIdStatusTable->SetNotified(stationIds[i], false);
            pSession->m_pStationIdStatusTable->SetValid(stationIds[i], false);
            pSession->m_pStationIdStatusTable->SetStatus(stationIds[i], false);
            pSession->RemoveFromStationIdList(stationIds[i]);
        }
    }
    StationIdStatusTable* pStatusTable = Session::s_pInstance->m_pStationIdStatusTable;
    for (EntryList::Node* pNode = pTable->m_List.Begin(); pNode != pTable->m_List.End(); pNode = EntryList::Advance(pNode)) {
        pStatusTable->SetStationIndex(pNode->m_Value.m_StationId, pNode->m_Value.m_StationIndex);
    }
    Session::EventType eventType;
    switch (m_Phase) {
    case 6:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_STARTED_6;
        break;
    case 7:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_STARTED_7;
        break;
    case 8:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_STARTED_8;
        break;
    case 9:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_STARTED_9;
        break;
    case 10:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_STARTED_10;
        break;
    default:
        eventType = Session::EVENT_TYPE_LEAVE;
        break;
    }
    pSession->NotifyEvent(eventType, GetStationIdOfIndex253());
    pSession->m_State = 3;
    pSession->SetTransportState(false);
    // the monitoring data
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    u8 count = content.m_Unknown0x16F;
    if (count == 0xFF) {
        content.m_Unknown0x16F = 1;
    } else if (count < 0xFE) {
        content.m_Unknown0x16F = count + 1;
    }
    Session* pSession2 = Session::s_pInstance;
    content.m_Unknown0x174 = pSession2->m_SessionIds[pSession2->m_CurrentIndex];
    content.m_Unknown0x17C = stayingNum;
    content.m_Unknown0x25C = m_Phase;
}

// 0x00432B98 (name is ours)
bool nn::pia::session::JointSessionJob::CompleteJointSession()
{
    Session* pSession = Session::s_pInstance;
    Session::EventType eventType;
    switch (m_Phase) {
    case 6:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_DONE_6;
        break;
    case 7:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_DONE_7;
        break;
    case 8:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_DONE_8;
        break;
    case 9:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_DONE_9;
        break;
    case 10:
        eventType = Session::EVENT_TYPE_JOINT_SESSION_DONE_10;
        break;
    default:
        return false;
    }
    // the host of the mesh is the host of the joint session
    transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(Mesh::s_pInstance->m_HostStationIndex);
    if (pHostStation == nullptr) {
        return false;
    }
    u32 hostPrincipalId;
    if (pHostStation->GetPrincipalId(&hostPrincipalId).IsFailure()) {
        return false;
    }
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, hostPrincipalId).IsFailure()) {
        return false;
    }
    if (m_Phase == 9 || m_Phase == 10) {
        pSession->m_JointHostStationId = GetStationIdOfIndex253();
        pSession->m_Unknown0xFC = 0;
    } else {
        pSession->m_JointHostStationId = entry.m_StationId;
        u32 sessionId = 0;
        if (!Session::s_pInstance->m_pStationIdStatusTable->GetSessionId(entry.m_StationId, &sessionId) || sessionId == 0) {
            return false;
        }
        pSession->m_Unknown0xFC = sessionId;
        // the local host must own both sessions
        if (pSession->m_LocalStationId == pSession->m_JointHostStationId) {
            if (pSession->m_LocalStationId.m_Low != pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90()) {
                return false;
            }
            if (pSession->m_LocalStationId.m_Low != pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->vf_0x90()) {
                return false;
            }
        }
    }
    // the owner of the current session is the host of the session
    u32 ownerPrincipalId = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x90();
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, ownerPrincipalId).IsFailure()) {
        return false;
    }
    pSession->SetTransportState(true);
    pSession->m_State = m_Phase == 8 || m_Phase == 6 || m_Phase == 7 ? 4 : 2;
    pSession->m_pStationIdStatusTable->RemoveLostStations();
    pSession->m_pStationIdStatusTable->NotifyStations();
    if (entry.m_StationId != pSession->m_HostStationId) {
        pSession->m_HostStationId = entry.m_StationId;
        pSession->NotifyEvent(Session::EVENT_TYPE_HOST_CHANGED, entry.m_StationId);
    }
    pSession->NotifyEvent(eventType, GetStationIdOfIndex253());
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_Unknown0x178 = GetElapsedMSec(m_StartTime);
    content.m_Unknown0x17D = m_Unknown0xC0;
    m_Phase = 0;
    return true;
}

// 0x00432ECC (name is ours)
void nn::pia::session::JointSessionJob::FailJointSession()
{
    Session* pSession = Session::s_pInstance;
    if (pSession->m_DisconnectState != 3) {
        pSession->m_DisconnectState = 2;
    }
    pSession->m_pStationIdStatusTable->RemoveLostStations();
    Session::s_pInstance->NotifyEvent(Session::EVENT_TYPE_JOINT_SESSION_FAILED, GetStationIdOfIndex253());
    m_Phase = 0;
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_Unknown0x178 = GetElapsedMSec(m_StartTime);
    content.m_Unknown0x17D = m_Unknown0xC0;
}

// 0x00432F64 (name is ours)
void nn::pia::session::JointSessionJob::UpdateMonitoringPhase()
{
    common::g_SessionBeginMonitoringContent.m_Unknown0x25D = Session::s_pInstance->GetJointSessionPhase();
}

// 0x00432F88 (name is ours)
void nn::pia::session::JointSessionJob::Cleanup()
{
    vf_0x60();
    if (common::IsValidPointer(m_pCallContext)) {
        if (m_pCallContext->m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
    m_Phase = 0;
    m_Unknown0x59 = 0;
    m_MessageType = 0;
    m_StationId = GetStationIdOfIndex253();
    m_JointSessionId = 0;
    m_Unknown0x98 = 0;
    m_Unknown0x99 = 0;
    m_Unknown0x9A = 0;
    m_Unknown0x9B = 0;
    m_Unknown0x9C = 0;
    m_Unknown0x9D = 0;
    m_IsFailed = false;
    m_FailureResult = nn::Result();
    m_Unknown0x9F = false;
    if (m_StationIdList.GetCount() != 0) {
        m_StationIdList.ClearNodes();
    }
}

// 0x00433094
nn::pia::session::JointSessionJob::JointSessionJob()
    : m_pCallContext(nullptr), m_Phase(0), m_Unknown0x59(0), m_pStationIdNodeBuffer(nullptr), m_JointSessionId(0), m_Unknown0x98(0), m_Unknown0x99(0),
      m_Unknown0x9A(0), m_Unknown0x9C(0), m_IsFailed(false), m_Unknown0x9F(false), m_FailureResult(common::RESULT_NOT_SET)
{
    AllocateStationIdList();
}

// 0x004332C0
// 0x00433214 (deleting dtor)
nn::pia::session::JointSessionJob::~JointSessionJob()
{
    FreeStationIdList();
}

// 0x007338CC
void nn::pia::session::JointSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
