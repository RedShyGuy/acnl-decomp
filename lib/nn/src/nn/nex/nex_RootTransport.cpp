#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_InstanceTable.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_Platform.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_Relay.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StringConversion.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/nex/nex_VirtualFilterDevice.h"
#include "nn/socket/socket_Api.h"
#include "nn/socket/socket_Types.h"

#include <cwchar>

namespace nn {
namespace nex {
namespace {
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_NOT_IMPLEMENTED = static_cast<int>(0x80010002);
// the sizes of the texts of an address
const u32 ADDRESS_LENGTH = 20;
const u32 ADDRESS_WIDE_LENGTH = 128;

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// slot 0x08 of a data handler: true if it took the data
typedef bool (*HandleDataFunction)(void* pHandler, Buffer* pData, StationURL* pURL);
inline HandleDataFunction GetHandleDataFunction(void* pHandler)
{
    return reinterpret_cast<HandleDataFunction>((*reinterpret_cast<void***>(pHandler))[2]);
}

// (inline) the dotted text of the address
inline void ToAddressText(const InetAddress& address, wchar_t* pText)
{
    char text[ADDRESS_LENGTH];
    pText[0] = L'\0';
    nn::socket::InetNtoP(nn::socket::AF_INET, &address.m_SockAddr.m_Address, text, ADDRESS_LENGTH);
    StringConversion::Char8_2T(text, pText, ADDRESS_WIDE_LENGTH * sizeof(wchar_t));
}

// (inline) the URL of the address and port for a NAT check
inline void SetNATCheckURL(StringStream* pStream, const wchar_t* pAddress, unsigned short port)
{
    pStream->Clear();
    *pStream << L"prudp:/address=" << pAddress << L";port=" << static_cast<int>(port);
}
} // namespace

// 0x00ADDE38 (name is ours)
PseudoGlobalVariable<u16> RootTransport::s_WellKnownPortNumber(9203);
// 0x0096C9AC (name is ours)
s32 RootTransport::s_CheckAndRelayTimeout = 60000;
// 0x0096CA5C (name is ours)
const wchar_t* RootTransport::s_pNATCheckServerName1 = L"nncs1.app.nintendowifi.net";
// 0x0096CA60 (name is ours)
const wchar_t* RootTransport::s_pNATCheckServerName2 = L"nncs2.app.nintendowifi.net";

// 0x0036AED4 slot 0x08
nn::nex::qResult nn::nex::RootTransport::Start()
{
    return qResult(RESULT_SUCCESS);
}

// 0x0036AEF0 | fefates:callgraph [tier C]
nn::nex::RootTransport* nn::nex::RootTransport::GetInstance()
{
    Network* pNetwork = Network::GetInstance();
    return pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
}

// 0x0036B04C (name is ours)
bool nn::nex::RootTransport::ReceiveUnhandled(nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress)
{
    ScopedCS lock(GetSchedulerLock());
    for (qList<void*>::Node* pNode = m_DataHandlers.GetFirst(); !m_DataHandlers.IsEnd(pNode); pNode = pNode->m_pNext) {
        StationURL url;
        url.ValidateData();
        url.m_pAddress->m_SockAddr = pAddress->m_SockAddr;
        url.m_IsFormatted = false;
        if (GetHandleDataFunction(pNode->m_Value)(pNode->m_Value, pData, &url)) {
            return false;
        }
    }
    return true;
}

// 0x0036B148 | fefates:callgraph [tier C]
void nn::nex::RootTransport::SetNATProperties(const nn::nex::NATProperties& properties)
{
    m_NATProperties.m_PrivateAddress = properties.m_PrivateAddress;
    m_NATProperties.m_PrivatePort = properties.m_PrivatePort;
    m_NATProperties.m_PublicAddress = properties.m_PublicAddress;
    m_NATProperties.m_PublicPort = properties.m_PublicPort;
    m_NATProperties.m_NatMapping = properties.m_NatMapping;
    m_NATProperties.m_NatFiltering = properties.m_NatFiltering;
    m_NATProperties.m_PortIncrement = properties.m_PortIncrement;
    m_NATProperties.m_IsPortPreserved = properties.m_IsPortPreserved;
}

// 0x0036B1A0 (name is ours)
bool nn::nex::RootTransport::ResolveRelayAddress()
{
    if (!GetRelay()->m_IsEnabled || m_IsRelayAddressResolved) {
        return true;
    }
    if (m_RelayServerName.m_pString != nullptr && wcslen(m_RelayServerName.m_pString) != 0) {
        *reinterpret_cast<u32*>(&m_RelayAddress.m_SockAddr) = 0;
        m_RelayAddress.m_SockAddr.m_Address = 0;
        m_RelayAddress.m_SockAddr.m_Family = nn::socket::AF_INET;
        m_RelayAddress.m_SockAddr.m_Port = __builtin_bswap16(RELAY_PORT);
        if (!m_RelayAddress.SetAddress(m_RelayServerName.m_pString)) {
            return false;
        }
    }
    m_IsRelayAddressResolved = __builtin_bswap32(m_RelayAddress.m_SockAddr.m_Address) != 0;
    return true;
}

// 0x0036B24C | mk7dlp:bytes [tier A]
u16 nn::nex::RootTransport::GetNextPortNumber()
{
    return Platform::GetRandomNumber(0x3FFF) + 0xC000;
}

// 0x0036B268 (name is ours)
s32 nn::nex::RootTransport::GetCheckAndRelayTimeout()
{
    return s_CheckAndRelayTimeout;
}

// 0x0036B278
u16 nn::nex::RootTransport::GetWellKnownPortNumber()
{
    return s_WellKnownPortNumber.Get();
}

// 0x0036B2C4
void nn::nex::RootTransport::SetWellKnownPortNumber(unsigned short port)
{
    s_WellKnownPortNumber.Get() = port;
}

// 0x0036B314 (name is ours)
bool nn::nex::RootTransport::SetNATCheckServer1(const wchar_t* pServerName, unsigned short port1, unsigned short port2)
{
    if (port1 == 0 || port2 == 0) {
        return false;
    }
    InetAddress address(pServerName, 0);
    if (address.GetAddress() == 0) {
        return false;
    }
    wchar_t text[ADDRESS_WIDE_LENGTH];
    ToAddressText(address, text);
    SetNATCheckURL(m_NATCheckURLs.m_pBegin[0], text, port1);
    address.m_SockAddr.m_Port = __builtin_bswap16(port2);
    ToAddressText(address, text);
    SetNATCheckURL(m_NATCheckURLs.m_pBegin[1], text, port2);
    return true;
}

// 0x0036B458 (name is ours)
bool nn::nex::RootTransport::SetNATCheckServer2(const wchar_t* pServerName, unsigned short port1, unsigned short port2)
{
    if (port1 == 0 || port2 == 0) {
        return false;
    }
    InetAddress address(pServerName, 0);
    if (address.GetAddress() == 0) {
        return false;
    }
    wchar_t text[ADDRESS_WIDE_LENGTH];
    ToAddressText(address, text);
    SetNATCheckURL(m_NATCheckURLs.m_pBegin[2], text, port1);
    address.m_SockAddr.m_Port = __builtin_bswap16(port2);
    ToAddressText(address, text);
    SetNATCheckURL(m_NATCheckURLs.m_pBegin[3], text, port2);
    return true;
}

// 0x0036B59C slot 0x3C
void nn::nex::RootTransport::DropPoolPackets()
{
    // nothing to do
}

// 0x0036B5A0 (name is ours)
bool nn::nex::RootTransport::InitializeNATCheckURLs()
{
    if (m_AreNATCheckURLsReady) {
        return true;
    }
    if (!SetNATCheckServer1(s_pNATCheckServerName1, NAT_CHECK_PORT_1, NAT_CHECK_PORT_2)) {
        return false;
    }
    if (!SetNATCheckServer2(s_pNATCheckServerName2, NAT_CHECK_PORT_1, NAT_CHECK_PORT_2)) {
        return false;
    }
    StringStream** ppURLs = m_NATCheckURLs.m_pBegin;
    m_AreNATCheckURLsReady =
        ppURLs[0]->GetLength() != 0 && ppURLs[1]->GetLength() != 0 && ppURLs[2]->GetLength() != 0 && ppURLs[3]->GetLength() != 0;
    return true;
}

// 0x0036B678 slot 0x38
nn::nex::qResult nn::nex::RootTransport::SendRaw(nn::nex::StationURL* pURL, nn::nex::Buffer* pBuffer)
{
    return qResult(RESULT_NOT_IMPLEMENTED);
}

// 0x0036B694 slot 0x2C
nn::nex::Relay* nn::nex::RootTransport::GetRelay()
{
    return nullptr;
}

// 0x0036B69C slot 0x28
nn::nex::Router* nn::nex::RootTransport::GetRouter()
{
    return nullptr;
}

// 0x0036B6A4
nn::nex::RootTransport::RootTransport()
    : m_Unknown0x04(LOCK_LEVEL), m_Unknown0x2C(LOCK_LEVEL), m_MaxPacketSize(DEFAULT_MAX_PACKET_SIZE), m_Unknown0x58(0), m_pUnknown0xAC(nullptr), m_PortNumber(0),
      m_AreNATCheckURLsReady(false), m_IsRelayAddressResolved(false), m_Unknown0xFE(false), m_RelayServerName(L"")
{
    m_StreamManager.m_pTransport = this;
    m_NATCheckURLs.ResizeInline(NAT_CHECK_URL_COUNT, nullptr);
    for (int i = 0; i < static_cast<int>(NAT_CHECK_URL_COUNT); i++) {
        m_NATCheckURLs.m_pBegin[i] = new StringStream();
    }
    m_pFilterDevice = new (MemoryManager::Allocate(sizeof(VirtualFilterDevice))) VirtualFilterDevice();
}

// 0x0036BCD0 slot 0x00
// 0x0036BCA0 (deleting dtor)
nn::nex::RootTransport::~RootTransport()
{
    for (int i = 0; i < static_cast<int>(NAT_CHECK_URL_COUNT); i++) {
        if (m_NATCheckURLs.m_pBegin[i] != nullptr) {
            delete m_NATCheckURLs.m_pBegin[i];
        }
    }
    if (m_pFilterDevice != nullptr) {
        m_pFilterDevice->~VirtualFilterDevice();
        MemoryManager::FreeInline(m_pFilterDevice);
    }
}

// 0x0072A99C | fefates:callgraph [tier C]
nn::nex::NATProperties nn::nex::RootTransport::GetNATProperties() const
{
    return m_NATProperties;
}

} // namespace nex
} // namespace nn
