#include "nn/pia/transport/transport_ProtocolManager.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/transport/transport_ProtocolEvent.h"
#include "nn/pia/transport/transport_ProtocolMessageFilteringManager.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the bit of the protocol type in the monitoring data (name is ours)
u32 GetProtocolMonitoringBit(u16 protocolType)
{
    switch (protocolType) {
    case PROTOCOL_TYPE_RELAY:
        return 1 << 0;
    case PROTOCOL_TYPE_STATION:
        return 1 << 1;
    case PROTOCOL_TYPE_MESH:
        return 1 << 2;
    case PROTOCOL_TYPE_SYNC_CLOCK:
        return 1 << 3;
    case PROTOCOL_TYPE_NAT:
        return 1 << 4;
    case PROTOCOL_TYPE_RTT:
        return 1 << 5;
    case PROTOCOL_TYPE_SYNC:
        return 1 << 6;
    case PROTOCOL_TYPE_UNRELIABLE:
        return 1 << 7;
    case PROTOCOL_TYPE_RELIABLE:
        return 1 << 8;
    case PROTOCOL_TYPE_RELIABLE_BROADCAST:
        return 1 << 9;
    case PROTOCOL_TYPE_FEEDBACK:
        return 1 << 10;
    case PROTOCOL_TYPE_GATEWAY:
        return 1 << 11;
    case PROTOCOL_TYPE_ROUNDROBIN_UNRELIABLE:
        return 1 << 12;
    case PROTOCOL_TYPE_SYNC_OLD:
        return 1 << 13;
    case PROTOCOL_TYPE_CLONE:
        return 1 << 14;
    case PROTOCOL_TYPE_VOICE:
        return 1 << 15;
    case PROTOCOL_TYPE_SESSION:
        return 1 << 16;
    case PROTOCOL_TYPE_UNKNOWN_0420:
        return 1 << 17;
    case PROTOCOL_TYPE_BANDWIDTH_CHECKER:
        return 1 << 18;
    default:
        return 0;
    }
}
} // namespace

// 0x00450848 (name is ours)
nn::Result nn::pia::transport::ProtocolManager::Initialize()
{
    m_IsStarted = false;
    return nn::Result();
}

// 0x00450858 | fefates:bytes [tier B]
void* nn::pia::transport::ProtocolManager::AllocProtocol(unsigned int size)
{
    u8* p = static_cast<u8*>(pead::AllocMemory(size, common::HeapManager::GetHeap()));
    if (p == nullptr) {
        return nullptr;
    }
    for (u32 i = 0; i < size; i++) {
        ::new (&p[i]) u8();
    }
    return p;
}

// 0x004508A4 | fefates:bytes [tier B]
nn::pia::transport::Protocol* nn::pia::transport::ProtocolManager::SearchProtocol(nn::pia::transport::ProtocolId protocolId, unsigned short protocolType)
{
    if (protocolId.GetType() != protocolType) {
        return nullptr;
    }
    for (Protocol* p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        if (p->m_ProtocolId == protocolId) {
            return p;
        }
    }
    return nullptr;
}

// 0x00450904 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::DestroyProtocol(unsigned int protocolId)
{
    for (Protocol* p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        if (p->m_ProtocolId.m_Id == protocolId) {
            if (!p->IsEnableProtocolFiltering()) {
                ProtocolMessageFilteringManager::s_pInstance->RemoveNoFilteringProtocolType(p->m_ProtocolId.GetType());
            }
            m_ProtocolList.Erase(p);
            delete p;
            return;
        }
    }
}

// 0x004509BC | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::CleanupProtocols()
{
    for (Protocol* p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        p->Cleanup();
    }
}

// 0x00450A14 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ProtocolManager::StartupProtocols(nn::pia::StationIndex stationIndex)
{
    if (!m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    for (Protocol* p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        nn::Result result = p->Startup(stationIndex);
        if (result.IsFailure()) {
            CleanupProtocols();
            return result;
        }
    }
    return nn::Result();
}

// 0x00450AEC | fefates:bytes [tier B]
nn::pia::transport::ProtocolId nn::pia::transport::ProtocolManager::CreateProtocolImpl(nn::pia::transport::Protocol* pProtocol, nn::pia::transport::ProtocolId protocolId)
{
    pProtocol->SetPort(protocolId.GetPort());
    Protocol* p;
    for (p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        if (protocolId == p->m_ProtocolId) {
            return ProtocolId::INVALID;
        }
        if (protocolId.m_Id < p->m_ProtocolId.m_Id) {
            break;
        }
    }
    m_ProtocolList.InsertBefore(p, pProtocol);
    if (!pProtocol->IsEnableProtocolFiltering()) {
        ProtocolMessageFilteringManager::s_pInstance->AddNoFilteringProtocolType(protocolId.GetType());
    }
    common::g_SessionBeginMonitoringContent.m_UnusedProtocolBitmap &= ~GetProtocolMonitoringBit(protocolId.GetType());
    return protocolId;
}

// 0x00450E4C | fefates:bytes [tier B]
nn::Result nn::pia::transport::ProtocolManager::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event)
{
    if (!m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    // the first error
    nn::Result result;
    for (Protocol* p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        nn::Result protocolResult = p->UpdateProtocolEvent(event);
        if (protocolResult.IsFailure() && result.IsSuccess()) {
            result = protocolResult;
        }
    }
    return result;
}

// 0x00450ED8 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::Cleanup()
{
    for (Protocol* p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        p->m_pPacketHandler = nullptr;
    }
    m_IsStarted = false;
}

// 0x00450F30 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ProtocolManager::Startup(nn::pia::transport::PacketHandler* pPacketHandler)
{
    if (!common::IsValidPointer(pPacketHandler)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    for (Protocol* p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        p->m_pPacketHandler = pPacketHandler;
    }
    m_IsStarted = true;
    return nn::Result();
}

// 0x00450FBC | fefates:bytes [tier B]
nn::Result nn::pia::transport::ProtocolManager::Dispatch()
{
    if (!m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    for (Protocol* p = m_ProtocolList.Begin(); p != m_ProtocolList.End(); p = m_ProtocolList.Advance(p)) {
        nn::Result result = p->Dispatch();
        if (result.IsFailure()) {
            return result;
        }
    }
    return nn::Result();
}

// 0x00451034 | fefates:bytes [tier B]
void nn::pia::transport::ProtocolManager::Finalize()
{
    m_IsStarted = false;
    while (m_ProtocolList.GetCount() != 0) {
        Protocol* p = m_ProtocolList.PopBack();
        if (p != nullptr) {
            delete p;
        }
    }
}

// 0x00451090 | fefates:bytes [tier B]
nn::pia::transport::ProtocolManager::ProtocolManager() : m_IsStarted(false)
{
    m_ProtocolList.SetOffset(offsetof(Protocol, m_ListNode));
}

// 0x004510CC | fefates:bytes [tier B]
nn::pia::transport::ProtocolManager::~ProtocolManager()
{
    Finalize();
}

// 0x007352EC
void nn::pia::transport::ProtocolManager::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
