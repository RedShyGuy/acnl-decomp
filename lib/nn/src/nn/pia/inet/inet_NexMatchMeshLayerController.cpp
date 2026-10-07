#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include <new>
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_MatchmakeExtensionClient.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController_NexNotificationEventHandler.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"
#include "nn/pia/session/session_Session.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0040FBD8
void nn::pia::inet::NexMatchMeshLayerController::vf_0x14()
{
    CleanupMesh();
    NexFacade::s_pInstance->StopNatSession();
    NexFacade::s_pInstance->Cleanup();
}

// 0x0040FC0C
nn::Result nn::pia::inet::NexMatchMeshLayerController::StartupMesh(nn::pia::session::CommonMatchmakeSession* pSession, bool isBandwidthCheckOneWay)
{
    return StartupMeshCore(pSession, isBandwidthCheckOneWay);
}

// 0x0040FC18
void nn::pia::inet::NexMatchMeshLayerController::vf_0x38()
{
    NexFacade::s_pInstance->m_pNatTraverser->m_pProtocol->m_Unknown0x4C5 = true;
}

// 0x0040FC38 (name is ours)
bool nn::pia::inet::NexMatchMeshLayerController::HasSessionEntry(u32 sessionId) const
{
    NexNotificationEventHandler* pHandler = m_pNotificationEventHandler;
    if (pHandler == nullptr) {
        return false;
    }
    for (u32 i = 0; i < NexNotificationEventHandler::SESSION_ENTRY_NUM; i++) {
        if (pHandler->m_Entries[i].m_SessionId == sessionId) {
            return true;
        }
    }
    return false;
}

// 0x0040FC78 (name is ours)
void nn::pia::inet::NexMatchMeshLayerController::RemoveSessionEntry(u32 sessionId)
{
    if (m_pNotificationEventHandler != nullptr) {
        m_pNotificationEventHandler->RemoveSession(sessionId);
    }
}

// 0x0040FC8C (name is ours)
nn::Result nn::pia::inet::NexMatchMeshLayerController::StartNatSession()
{
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pMatchmakeSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    nn::Result result = NexFacade::s_pInstance->Startup(pMatchmakeSession->m_pClient);
    if (result.IsFailure()) {
        NexFacade::s_pInstance->Cleanup();
        return result;
    }
    result = NexFacade::s_pInstance->StartNatSessionAsync();
    if (result.IsFailure()) {
        NexFacade::s_pInstance->StopNatSession();
        NexFacade::s_pInstance->Cleanup();
    }
    return result;
}

// 0x0040FD28 (name is ours)
void nn::pia::inet::NexMatchMeshLayerController::ResetSessionEntries()
{
    if (m_pNotificationEventHandler != nullptr) {
        m_pNotificationEventHandler->Reset();
    }
}

// 0x0040FDDC (name is ours)
nn::pia::common::ExecuteResult::State nn::pia::inet::NexMatchMeshLayerController::WaitStartNatSession(nn::Result* pResult)
{
    if (!NexFacade::s_pInstance->IsCompletedStartNatSession()) {
        return common::ExecuteResult::STATE_NEXT_DISPATCH;
    }
    nn::Result result = NexFacade::s_pInstance->GetStartNatSessionResult();
    if (result.IsSuccess()) {
        session::Session* pSession = session::Session::s_pInstance;
        NexFacade::s_pInstance->m_Unknown0x10 = pSession->m_SessionIds[pSession->m_CurrentIndex];
        *pResult = result;
        return common::ExecuteResult::STATE_CONTINUE;
    }
    *pResult = result;
    NexFacade::s_pInstance->StopNatSession();
    NexFacade::s_pInstance->Cleanup();
    return common::ExecuteResult::STATE_SUCCESS;
}

// 0x0040FE7C
u8 nn::pia::inet::NexMatchMeshLayerController::GetNetworkStatus()
{
    nex::qResult result = m_pNgsBridge->IsConnected();
    if (!result) {
        return 3;
    }
    return m_pNotificationEventHandler->m_IsSessionDeleted ? 2 : 1;
}

// 0x0040FECC (name is ours)
nn::Result nn::pia::inet::NexMatchMeshLayerController::CancelStartNatSession()
{
    return NexFacade::s_pInstance->CancelStartNatSession();
}

// 0x0040FEE4
bool nn::pia::inet::NexMatchMeshLayerController::HasOtherSession(u32 sessionId, u32 otherSessionId)
{
    for (u32 i = 0; i < GetSessionEntryNum(); i++) {
        u32 id = GetSessionId(i);
        if (id != 0 && id != sessionId && id != otherSessionId) {
            return true;
        }
    }
    return false;
}

// 0x00410CE8
void nn::pia::inet::NexMatchMeshLayerController::Cleanup()
{
    CleanupMesh();
    NexFacade::s_pInstance->StopNatSession();
    NexFacade::s_pInstance->Cleanup();
    m_pNotificationEventHandler->Reset();
    ClearUnknown0x69();
    static_cast<NexMatchmakeSession*>(session::Session::s_pInstance->m_pMatchmakeSessions[session::Session::s_pInstance->m_CurrentIndex])->Unbind();
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pOther = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]);
    if (pOther != nullptr) {
        pOther->Unbind();
    }
    if (m_pNgsBridge != nullptr) {
        m_pNgsBridge->UnregisterNotificationEventHandler(m_pNotificationEventHandler);
        m_pNgsBridge = nullptr;
    }
}

// 0x00410E28
nn::Result nn::pia::inet::NexMatchMeshLayerController::Startup(bool isHostMigrationEnabled, const u8* pIdentificationData, nn::pia::common::Crypto::Mode cryptoMode,
                                                             u32 timeoutMSec, u32 keepAliveIntervalMSec, s32 bandwidthCheckBandwidth, u32 bandwidthCheckPacketSize,
                                                             bool isBandwidthCheckOneWay, s32 bandwidthCheckDurationMSec,
                                                             const nn::pia::transport::Station::PlayerName* pPlayerName, bool value)
{
    if (!common::IsValidPointer(NexFacade::s_pInstance)) {
        return common::RESULT_INVALID_STATE;
    }
    m_pNgsBridge = NexFacade::s_pInstance->m_pNgsBridge;
    if (!common::IsValidPointer(m_pNgsBridge)) {
        return common::RESULT_INVALID_STATE;
    }
    // the principal of the game
    session::Session* pSession = session::Session::s_pInstance;
    pSession->m_Unknown0x8C = m_pNgsBridge->GetCredentials()->m_PrincipalId;
    static_cast<NexMatchmakeSession*>(session::Session::s_pInstance->m_pMatchmakeSessions[session::Session::s_pInstance->m_CurrentIndex])->Bind(m_pNgsBridge);
    pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pOther = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]);
    if (pOther != nullptr) {
        pOther->Bind(m_pNgsBridge);
    }
    m_pNgsBridge->RegisterNotificationEventHandler(m_pNotificationEventHandler);
    NexMatchmakeSession::s_NetworkErrorCode = 0;
    return MeshLayerController::Startup(isHostMigrationEnabled, pIdentificationData, cryptoMode, timeoutMSec, keepAliveIntervalMSec, bandwidthCheckBandwidth,
                                        bandwidthCheckPacketSize, isBandwidthCheckOneWay, bandwidthCheckDurationMSec, pPlayerName, value);
}

// 0x00410F40
nn::pia::inet::NexMatchMeshLayerController::NexMatchMeshLayerController() : m_pNgsBridge(nullptr)
{
    void* pBuffer = pead::AllocMemory(sizeof(NexNotificationEventHandler), common::HeapManager::GetHeap());
    m_pNotificationEventHandler = ::new (pBuffer) NexNotificationEventHandler();
}

// 0x00411074
// 0x00411028 (deleting dtor)
nn::pia::inet::NexMatchMeshLayerController::~NexMatchMeshLayerController()
{
    if (m_pNotificationEventHandler != nullptr) {
        m_pNotificationEventHandler->~NexNotificationEventHandler();
        pead::FreeMemory(m_pNotificationEventHandler);
        m_pNotificationEventHandler = nullptr;
    }
}

// 0x0072F860 (name is ours)
u32 nn::pia::inet::NexMatchMeshLayerController::GetSessionId(u32 index) const
{
    if (m_pNotificationEventHandler == nullptr) {
        return 0;
    }
    return index < NexNotificationEventHandler::SESSION_ENTRY_NUM ? m_pNotificationEventHandler->m_Entries[index].m_SessionId : 0;
}

// 0x0072F884 (name is ours)
nn::pia::inet::NexMatchMeshLayerController::SessionEntry*
nn::pia::inet::NexMatchMeshLayerController::FindSessionEntry(u32 sessionId) const
{
    if (m_pNotificationEventHandler == nullptr) {
        return nullptr;
    }
    return m_pNotificationEventHandler->FindEntry(sessionId);
}

// 0x0072F8D4 (name is ours)
bool nn::pia::inet::NexMatchMeshLayerController::HasParticipant(u32 sessionId, u32 principalId) const
{
    if (m_pNotificationEventHandler == nullptr) {
        return false;
    }
    SessionEntry* pEntry = m_pNotificationEventHandler->FindEntry(sessionId);
    if (pEntry == nullptr) {
        return false;
    }
    typedef SessionEntry::List List;
    for (List::Node* node = pEntry->m_List.Begin(); node != pEntry->m_List.End(); node = List::Advance(node)) {
        if (node->m_Value.m_PrincipalId == principalId && node->m_Value.m_Count > 0) {
            return true;
        }
    }
    return false;
}

// 0x0072F970 (name is ours)
u32 nn::pia::inet::NexMatchMeshLayerController::GetSessionEntryNum() const
{
    return m_pNotificationEventHandler != nullptr ? NexNotificationEventHandler::SESSION_ENTRY_NUM : 0;
}

// 0x0072F980
u32 nn::pia::inet::NexMatchMeshLayerController::vf_0x28()
{
    NexNotificationEventHandler* pHandler = m_pNotificationEventHandler;
    if (pHandler == nullptr) {
        return 0;
    }
    u16 num = 0;
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession != nullptr) {
        u32 sessionId = pSession->GetJointSessionId() != 0 ? pSession->GetJointSessionId() : pSession->m_SessionIds[pSession->m_CurrentIndex];
        SessionEntry* pEntry = pHandler->FindEntry(sessionId);
        if (pEntry != nullptr) {
            typedef SessionEntry::List List;
            for (List::Node* node = pEntry->m_List.Begin(); node != pEntry->m_List.End(); node = List::Advance(node)) {
                if (node->m_Value.m_PrincipalId != 0 && node->m_Value.m_Count > 0) {
                    num++;
                }
            }
        }
    }
    return num;
}

// 0x0072FA5C
void nn::pia::inet::NexMatchMeshLayerController::vf_0x1C()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
