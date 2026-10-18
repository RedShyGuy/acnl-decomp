#include "nn/nex/nex_ConnectivityManager.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_InstanceTable.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_Relay.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StationURL.h"

namespace nn {
namespace nex {
namespace {
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
// the bit of the parameter "type" of a public URL
const u32 URL_TYPE_PUBLIC = 2;
} // namespace

// (inline in the constructor of Network)
nn::nex::ConnectivityManager::ConnectivityManager()
    : PseudoSingleton(KIND), m_Unknown0x11(0), m_Unknown0x12(1), m_PortNumber(RootTransport::GetWellKnownPortNumber()), m_Unknown0x16(0), m_Unknown0x18(0),
      m_CallContextId(0), m_Unknown0x20(0), m_Unknown0x24(0), m_HasPacketBuffer(false)
{
}

// 0x00392B78 | fefates:callgraph [tier C]
void nn::nex::ConnectivityManager::SetRelayPublicAddress()
{
    qList<StationURL> urls;
    Network::GetInstance()->GetStationURLs(urls);
    for (qList<StationURL>::Node* pNode = urls.GetFirst(); !urls.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (!(pNode->m_Value.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC)) {
            continue;
        }
        if (pNode->m_Value.GetParamValue(L"RVCID", 0) == 0) {
            continue;
        }
        Relay* pRelay = RootTransport::GetInstance()->GetRelay();
        pNode->m_Value.ValidateData();
        pRelay->m_Address.m_SockAddr = pNode->m_Value.m_pAddress->m_SockAddr;
    }
}

// 0x00392CAC slot 0x00
// 0x00392C7C (deleting dtor)
nn::nex::ConnectivityManager::~ConnectivityManager()
{
    if (m_CallContextId != 0) {
        Core* pCore = Core::GetInstance();
        ScopedCS lock((pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock);
        CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
        if (pContext != nullptr && pContext->m_State == CallContext::STATE_CALL_IN_PROGRESS) {
            pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(RESULT_CANCELLED), true);
        }
    }
    if (m_HasPacketBuffer) {
        PacketBufferManager* pManager = PacketBufferManager::GetInstance();
        ScopedCS lock(PacketBufferManager::s_Lock);
        pManager->m_Unknown0x24--;
        m_HasPacketBuffer = false;
    }
}

} // namespace nex
} // namespace nn
