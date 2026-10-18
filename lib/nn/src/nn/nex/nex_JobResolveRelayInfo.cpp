#include "nn/nex/nex_JobResolveRelayInfo.h"
#include "nn/nex/nex_ConnectivityManager.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_NATTraversalEngine.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_Relay.h"
#include "nn/nex/nex_RelayMessage.h"
#include "nn/nex/nex_RelayStream.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_StringConversion.h"
#include "nn/socket/socket_Api.h"

namespace nn {
namespace nex {
namespace {
// the results of the relay information (the names are ours)
const s8 RESULT_RELAY_AS_NEEDED = 1;
const s8 RESULT_RELAY_UNLESS_DEPENDENT = 2;
const s8 RESULT_RELAY_ALWAYS = 3;
// the NAT maps the address per destination; the port increment of a NAT that keeps the order
const u8 NAT_MAPPING_DEPENDENT = 2;
const u32 PORT_INCREMENT_SEQUENTIAL = 1;
// the port of the relay server if the information has none
const u16 DEFAULT_RELAY_PORT = 12525;
// the sizes of the text of the relay address
const u32 ADDRESS_TEXT_SIZE = 20;
const u32 ADDRESS_WIDE_TEXT_SIZE = 128;
// the address family of InetNtoP
const s32 FAMILY_INET = 2;

// (inline) the root transport of the network (null if there is none)
inline RootTransport* GetRootTransport()
{
    Network* pNetwork = Network::GetInstance();
    return pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
}
} // namespace

// 0x00393800 | fefates:bytes-fuzzy [tier B]
bool nn::nex::JobResolveRelayInfo::SetRelayAddress()
{
    if (Network::GetInstance() == nullptr || GetRootTransport() == nullptr) {
        return false;
    }
    switch (static_cast<u8>(m_AddressType)) {
    case ADDRESS_TYPE_HOST_NAME:
        Network::GetInstance()->m_pNATTraversalEngine->m_RelayHost = m_Address;
        break;
    case ADDRESS_TYPE_RELAY_ADDRESS: {
        NATTraversalEngine* pEngine = Network::GetInstance()->m_pNATTraversalEngine;
        char text[ADDRESS_TEXT_SIZE];
        wchar_t wideText[ADDRESS_WIDE_TEXT_SIZE];
        nn::socket::InetNtoP(FAMILY_INET, &GetRootTransport()->m_RelayAddress.m_SockAddr.m_Address, text, ADDRESS_TEXT_SIZE);
        StringConversion::Char8_2T(text, wideText, ADDRESS_WIDE_TEXT_SIZE);
        pEngine->m_RelayHost = String(wideText);
        break;
    }
    default:
        FailResolvedRelayInfo();
        return false;
    }
    if (m_Port != 0) {
        Network::GetInstance()->m_pNATTraversalEngine->m_RelayPort = m_Port;
    } else {
        Network::GetInstance()->m_pNATTraversalEngine->m_RelayPort = DEFAULT_RELAY_PORT;
    }
    return true;
}

// 0x00393950 | fefates:bytes-fuzzy [tier B]
void nn::nex::JobResolveRelayInfo::ResolvedRelayInfo()
{
    if (Network::GetInstance() == nullptr || GetRootTransport() == nullptr) {
        return;
    }
    switch (static_cast<s8>(m_Result)) {
    case RESULT_RELAY_AS_NEEDED:
        if (!SetRelayAddress()) {
            return;
        }
        break;
    case RESULT_RELAY_UNLESS_DEPENDENT: {
        if (!SetRelayAddress()) {
            return;
        }
        u8 mapping = GetRootTransport()->GetNATProperties().GetNATMappingInline();
        if (mapping != NAT_MAPPING_DEPENDENT) {
            Network::GetInstance()->m_pNATTraversalEngine->m_UsesRelay = true;
        }
        break;
    }
    case RESULT_RELAY_ALWAYS:
        if (!SetRelayAddress()) {
            return;
        }
        Network::GetInstance()->m_pNATTraversalEngine->m_UsesRelay = true;
        break;
    default:
        FailResolvedRelayInfo();
        return;
    }
    // the relay clock starts at the time of the server
    RelayMessage* pMessage = Network::GetInstance()->m_MessageSelector.m_pRelayMessage;
    pMessage->m_BaseTimeStamp = m_ServerTime.ToEpochTime();
    pMessage->m_BaseTime = SystemClock::GetTime();
    GetRootTransport()->GetRelay()->m_pStream->m_Unknown0x40 = m_RelayId;
    NATProperties properties = GetRootTransport()->GetNATProperties();
    if (properties.GetNATMappingInline() == NAT_MAPPING_DEPENDENT && properties.m_PortIncrement != PORT_INCREMENT_SEQUENTIAL) {
        Network::GetInstance()->m_pNATTraversalEngine->m_UsesRelay = true;
    }
    ConnectivityManager::SetRelayPublicAddress();
}

// 0x00393B34 | fefates:callgraph [tier C]
void nn::nex::JobResolveRelayInfo::FailResolvedRelayInfo()
{
    if (GetRootTransport() == nullptr) {
        return;
    }
    GetRootTransport()->GetRelay()->EnableRelay(false);
}

} // namespace nex
} // namespace nn
