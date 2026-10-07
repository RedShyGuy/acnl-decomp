#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/nex/nex_Globals.h"
#include "nn/nex/nex_NATTraversalRelayClient.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qVector.h"
#include "nn/pia/common/common_SignatureSetting.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_Api.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexNatRelay.h"
#include "nn/pia/inet/inet_NexNatTraversalProtocol.h"
#include "nn/pia/inet/inet_Socket.h"
#include "nn/pia/inet/inet_SocketInputStream.h"
#include "nn/pia/inet/inet_SocketOutputStream.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationLocation.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// the size of the memory of the relay client
const u32 RELAY_CLIENT_BUFFER_SIZE = 24;
const u64 TRACE_FLAG = 0x400ULL;
} // namespace

// 0x00975A64
bool NexFacade::s_IsNatSessionSkipped;
// 0x00975A68
NexFacade* NexFacade::s_pInstance;

// 0x00412FB4 | fefates:bytes [tier B]
nn::Result nn::pia::inet::NexFacade::initialize()
{
    m_pSocket = new Socket();
    m_pRelayClientBuffer = common::NewArray<u8>(RELAY_CLIENT_BUFFER_SIZE);
    m_pRelayClient = nullptr;
    m_pNatTraverser = new NatTraverser();
    m_pNatRelay = new NexNatRelay();
    return nn::Result();
}

// 0x00357848 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::ConvertInetAddressToNexInetAddress(const nn::pia::common::InetAddress& address, nn::nex::InetAddress* pNexAddress)
{
    pNexAddress->SetAddress(address.m_Address);
    pNexAddress->SetPortNumber(address.m_Port);
}

// 0x0041304C | fefates:callgraph [tier C]
bool nn::pia::inet::NexFacade::IsBehindNat(const nn::pia::transport::StationLocation& location)
{
    return location.m_Type & 1;
}

// 0x00413058 | fefates:bytes [tier B]
bool nn::pia::inet::NexFacade::IsEdmMapping(const nn::pia::transport::StationLocation& location)
{
    return location.m_NatMapping == 2;
}

// 0x0041306C | fefates:callgraph [tier C]
bool nn::pia::inet::NexFacade::IsEimMapping(const nn::pia::transport::StationLocation& location)
{
    return location.m_NatMapping == 1;
}

// 0x0041307C | fefates:bytes [tier B]
nn::Result nn::pia::inet::NexFacade::CreateInstance()
{
    if (!IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!IsInSetupMode()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new NexFacade();
    s_pInstance->initialize();
    return nn::Result();
}

// 0x0041314C | fefates:bytes [tier B]
nn::Result nn::pia::inet::NexFacade::CreateProtocols()
{
    nn::Result result = m_pNatTraverser->CreateProtocols();
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x00413168 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        s_pInstance->finalize();
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x004131B0 (name is ours)
nn::Result nn::pia::inet::NexFacade::StartNatSession(nn::pia::common::CallContext* pCallContext)
{
    return startNatSessionCore(pCallContext);
}

// 0x004131BC | fefates:bytes [tier B]
nn::Result nn::pia::inet::NexFacade::startNatSessionCore(nn::pia::common::CallContext* pCallContext)
{
    if (!common::IsValidPointer(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pOutputStream = static_cast<SocketOutputStream*>(transport::Transport::s_pInstance->GetOutputStream());
    if (!common::IsValidPointer(m_pOutputStream)) {
        return common::RESULT_INVALID_STATE;
    }
    m_pInputStream = static_cast<SocketInputStream*>(transport::Transport::s_pInstance->GetInputStream());
    if (!common::IsValidPointer(m_pInputStream) || m_pNatTraverser == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pNatTraverser->Startup(pCallContext);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x00413268 slot 0x14 | slot vf_0x14 of nn::pia::inet::NexFacade
nn::Result nn::pia::inet::NexFacade::StartNatSessionAsync()
{
    if (m_StartNatSessionCallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_StartNatSessionCallContext.Reset();
    return startNatSessionCore(&m_StartNatSessionCallContext);
}

// 0x004132A8 | fefates:callgraph [tier C]
u8 nn::pia::inet::NexFacade::GetNatPropertyMapping()
{
    NatTraverser* pNatTraverser = m_pNatTraverser;
    if (pNatTraverser != nullptr) {
        return pNatTraverser->m_NatProperty.m_NatMapping;
    }
    return 0;
}

// 0x004132B8 | fefates:bytes [tier B]
nn::Result nn::pia::inet::NexFacade::CompleteStartNatSession(unsigned short port)
{
    if (m_pSocket->Open().IsFailure() || m_pSocket->Bind(port).IsFailure()) {
        return common::RESULT_NAT_CHECK_FAILED;
    }
    nn::Result result = m_pOutputStream->ImportSocket(m_pSocket);
    if (result.IsFailure()) {
        return result;
    }
    result = m_pInputStream->ImportSocket(m_pSocket);
    if (result.IsFailure()) {
        return result;
    }
    result = m_pOutputStream->Startup();
    if (result.IsFailure()) {
        return result;
    }
    result = m_pInputStream->Startup();
    if (result.IsFailure()) {
        return result;
    }
    m_IsNatSessionStarted = true;
    return nn::Result();
}

// 0x00413360 slot 0x1C | fefates:bytes
nn::Result nn::pia::inet::NexFacade::GetStartNatSessionResult()
{
    if (!m_StartNatSessionCallContext.IsFinished()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_StartNatSessionCallContext.m_Result;
}

// 0x00413388 (name is ours)
nn::Result nn::pia::inet::NexFacade::CancelStartNatSession()
{
    if (m_StartNatSessionCallContext.GetState() != common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_StartNatSessionCallContext.Cancel();
    return nn::Result();
}

// 0x004133B0 slot 0x18 | fefates:bytes
bool nn::pia::inet::NexFacade::IsCompletedStartNatSession()
{
    return m_StartNatSessionCallContext.IsFinished();
}

// 0x004133D4 | fefates:bytes [tier B]
void nn::pia::inet::NexFacade::ConvertNexStationUrlToStationLocation(const nn::nex::StationURL& url, nn::pia::transport::StationLocation* pLocation)
{
    common::InetAddress address;
    const nex::InetAddress* pNexAddress = url.GetInetAddress();
    address.m_Address = pNexAddress->GetAddress();
    address.m_Port = pNexAddress->GetPortNumber();
    pLocation->m_StationAddress.SetInetAddress(address);
    pLocation->m_PrincipalId = url.GetPrincipalID();
    pLocation->m_ConnectionId = url.GetConnectionID();
    pLocation->m_StationKey = url.GetRVConnectionID();
    pLocation->m_UrlType = url.GetURLType();
    pLocation->m_StreamId = url.GetStreamID();
    pLocation->m_StreamType = url.GetStreamType();
    pLocation->m_NatMapping = url.GetNATMapping();
    pLocation->m_NatFiltering = url.GetNATFiltering();
    pLocation->m_Type = url.GetType();
    pLocation->m_ProbeRequestInitiation = url.GetProbeRequestInitiation();
}

// 0x004134A0 | fefates:bytes [tier B]
bool nn::pia::inet::NexFacade::RegisterNexNotificationEventHandler4Pia(nn::nex::NotificationEventHandler* pHandler)
{
    if (m_pNgsBridge != nullptr && m_pNgsBridge->RegisterNotificationEventHandler(pHandler)) {
        m_pNotificationEventHandler = pHandler;
        return true;
    }
    return false;
}

// 0x00413554 | fefates:bytes [tier B]
bool nn::pia::inet::NexFacade::UnregisterNexNotificationEventHandler4Pia(nn::nex::NotificationEventHandler* pHandler)
{
    if (m_pNgsBridge != nullptr && m_pNgsBridge->UnregisterNotificationEventHandler(pHandler)) {
        m_pNotificationEventHandler = nullptr;
        return true;
    }
    return false;
}

// 0x004134E0 (name is ours)
nn::Result nn::pia::inet::NexFacade::ConvertNexSessionKeyToSignatureSetting(const nn::nex::qVector<u8>& key, nn::pia::common::SignatureSetting* pSetting)
{
    if (!common::IsValidPointer(pSetting) || pSetting->m_pKey == nullptr || key.size() > pSetting->m_KeySize) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u8* pDst = static_cast<u8*>(const_cast<void*>(pSetting->m_pKey));
    for (const u8* p = key.begin(); p != key.end(); p++) {
        *pDst++ = *p;
    }
    return nn::Result();
}

// 0x00413594 | fefates:callgraph [tier C]
nn::Result nn::pia::inet::NexFacade::ConvertNexStationUrlToStationConnectionInfo(const nn::nex::qList<nn::nex::StationURL>& urls, nn::pia::transport::StationConnectionInfo* pInfo)
{
    if (urls.GetSize() != 1 && urls.GetSize() != 2) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    typename nex::qList<nex::StationURL>::Node* pNode = urls.GetFirst();
    ConvertNexStationUrlToStationLocation(pNode->m_Value, &pInfo->m_PublicLocation);
    pInfo->m_PublicLocation.Trace(TRACE_FLAG);
    pNode = pNode->m_pNext;
    if (urls.IsEnd(pNode)) {
        return nn::Result();
    }
    ConvertNexStationUrlToStationLocation(pNode->m_Value, &pInfo->m_PrivateLocation);
    pInfo->m_PrivateLocation.Trace(TRACE_FLAG);
    return nn::Result();
}

// 0x00413624 slot 0x00 | fefates:bytes
nn::Result nn::pia::inet::NexFacade::Bind(nn::pia::inet::NexFacade::LoginInfo* pLoginInfo)
{
    if (!common::IsValidPointer(pLoginInfo) || !common::IsValidPointer(pLoginInfo->m_pBackEndServices) ||
        !common::IsValidPointer(pLoginInfo->m_pBackEndServices->m_pCredentials) || pLoginInfo->m_Unknown0x4 == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_Unknown0x4 = pLoginInfo->m_Unknown0x4;
    m_pNgsBridge = pLoginInfo->m_pBackEndServices->m_pNgsBridge;
    return nn::Result();
}

// 0x00413694 slot 0x04 | slot vf_0x04 of nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::Unbind()
{
    m_Unknown0x4 = 0;
    m_pNgsBridge = nullptr;
}

// 0x004136A4 slot 0x0C | fefates:bytes
void nn::pia::inet::NexFacade::Cleanup()
{
    if (!m_IsStarted) {
        return;
    }
    m_IsStarted = false;
    if (m_pRelayClient != nullptr) {
        m_pRelayClient->Unbind();
        nex::NATTraversalRelayClient* pClient = m_pRelayClient;
        if (pClient->m_pRelayInterface != nullptr) {
            pClient->m_pRelayInterface->UnregisterRelayClient();
            pClient->m_pRelayInterface = nullptr;
        }
        // (in its own memory: only destroyed)
        m_pRelayClient->~NATTraversalRelayClient();
        m_pRelayClient = nullptr;
        m_pNatTraverser->m_pProtocol->UnregisterRelay();
    }
    if (m_pNgsBridge != nullptr && m_pNotificationEventHandler != nullptr) {
        m_pNgsBridge->UnregisterNotificationEventHandler(m_pNotificationEventHandler);
    }
    m_pNotificationEventHandler = nullptr;
    m_pMatchMakingClient = nullptr;
    m_Unknown0x10 = 0;
}

// 0x00413748 slot 0x08 | slot vf_0x08 of nn::pia::inet::NexFacade
nn::Result nn::pia::inet::NexFacade::Startup(nn::nex::MatchMakingClient* pClient)
{
    if (!common::IsValidPointer(pClient)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!common::IsValidPointer(m_pNgsBridge) || m_Unknown0x4 == 0 || (nex::g_Unknown0x96C91E != 3 && nex::g_Unknown0x96C91E != 2)) {
        return common::RESULT_INVALID_STATE;
    }
    m_Unknown0x10 = 0;
    m_pMatchMakingClient = pClient;
    if (m_pRelayClientBuffer == nullptr || nex::RVClientCore::GetInstance() == nullptr || nex::RVClientCore::GetInstance()->m_Unknown0xC == 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (!s_IsNatSessionSkipped) {
        // the relay client of nex in its own memory, with the relay of pia
        m_pRelayClient = ::new (m_pRelayClientBuffer) nex::NATTraversalRelayClient();
        m_pNatTraverser->m_pProtocol->RegisterRelay(m_pNatRelay);
        m_pRelayClient->Init();
        m_pRelayClient->m_pRelayInterface = &m_pNatRelay->m_RelayInterface;
        m_pNatRelay->m_RelayInterface.RegisterRelayClient(m_pRelayClient);
        nex::NATTraversalRelayClient* pRelayClient = m_pRelayClient;
        pRelayClient->Bind(m_pNgsBridge->GetCredentials());
    }
    m_IsStarted = true;
    return nn::Result();
}

// 0x0041387C | fefates:bytes [tier B]
bool nn::pia::inet::NexFacade::IsGlobal(const nn::pia::transport::StationLocation& location)
{
    return IsPublic(location) && !IsBehindNat(location);
}

// 0x004138A0 | fefates:callgraph [tier C]
bool nn::pia::inet::NexFacade::IsPublic(const nn::pia::transport::StationLocation& location)
{
    return (location.m_Type & 2) >> 1;
}

// 0x004138B0 slot 0x38 | fefates:bytes
void nn::pia::inet::NexFacade::finalize()
{
    if (m_pNatTraverser != nullptr) {
        delete m_pNatTraverser;
        m_pNatTraverser = nullptr;
    }
    if (m_pNatRelay != nullptr) {
        delete m_pNatRelay;
        m_pNatRelay = nullptr;
    }
    if (m_pRelayClientBuffer != nullptr) {
        common::DeleteArray(m_pRelayClientBuffer);
        m_pRelayClientBuffer = nullptr;
    }
    if (m_pSocket != nullptr) {
        delete m_pSocket;
        m_pSocket = nullptr;
    }
}

// 0x0041395C
// 0x0041393C (deleting dtor)
nn::pia::inet::NexFacade::~NexFacade()
{
    // the call context is destroyed (empty)
}

// 0x004268B8 slot 0x24 | slot vf_0x24 of nn::pia::inet::NexFacade
void nn::pia::inet::NexFacade::StopNatSession()
{
    m_IsNatSessionStarted = false;
    if (m_pOutputStream != nullptr) {
        m_pOutputStream->Cleanup();
    }
    if (m_pInputStream != nullptr) {
        m_pInputStream->Cleanup();
    }
    if (m_pSocket->m_OpenNum > 0) {
        m_pSocket->Close();
    }
    m_pOutputStream = nullptr;
    m_pInputStream = nullptr;
    if (m_pNatTraverser != nullptr) {
        m_pNatTraverser->Cleanup();
    }
    m_StartNatSessionCallContext.Reset();
}

// 0x0072FAD0 slot 0x28
void nn::pia::inet::NexFacade::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
