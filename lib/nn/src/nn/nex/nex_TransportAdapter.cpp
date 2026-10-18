#include "nn/nex/nex_TransportAdapter.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_InterfaceInfo.h"
#include "nn/nex/nex_StationContactInfo.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qResult.h"
#include "nn/socket/detail/detail_Api.h"

namespace nn {
namespace nex {
namespace {
const int RESULT_SUCCESS = qResult::SUCCESS;
} // namespace

// 0x0096C99C (name is ours)
TransportAdapter* TransportAdapter::s_pInstance;
// 0x00ADDDB8 (name is ours)
TransportAdapter TransportAdapter::s_DefaultInstance;

// 0x00383F78 slot 0x00
// 0x00383F54 (deleting dtor)
nn::nex::TransportAdapter::~TransportAdapter()
{
    // nothing to do
}

// 0x00383A4C slot 0x08 | fefates:bytes
bool nn::nex::TransportAdapter::GetLocalURLs(nn::nex::qList<nn::nex::StationURL>* pURLs)
{
    u32 count = 1;
    InterfaceInfo* pInfos = new InterfaceInfo[count];
    u32 hostId = 0;
    nn::socket::detail::GetHostId(&hostId);
    pInfos[0].m_Address = __builtin_bswap32(__builtin_bswap32(hostId));
    pInfos[0].m_Netmask = 0x00FFFFFF;
    pInfos[0].m_BroadcastAddress = 0xFFFFFFFF;
    pInfos[0].Trace(1);
    if (count == 0) {
        delete[] pInfos;
        return false;
    }
    for (u32 i = 0; i < count; i++) {
        if (pInfos[i].m_Flags & InterfaceInfo::FLAG_LOOPBACK) {
            continue;
        }
        StationURL url(L"prudp:/");
        InetAddress address;
        address.m_SockAddr.m_Address = pInfos[i].m_Address;
        url.SetInetAddress(&address);
        pURLs->PushBackInline(url);
    }
    if (pURLs->GetSize() == 0) {
        StationURL url(L"prudp:/");
        InetAddress address;
        address.m_SockAddr.m_Address = pInfos[0].m_Address;
        url.SetInetAddress(&address);
        pURLs->push_back(url);
    }
    SetNetworkInterfaceInfo(pInfos);
    delete[] pInfos;
    return true;
}

// 0x00383D7C slot 0x30
bool nn::nex::TransportAdapter::SetPortNumber(unsigned short port)
{
    m_LocalAddress.m_SockAddr.m_Port = __builtin_bswap16(port);
    return true;
}

// 0x00383D8C slot 0x10
void nn::nex::TransportAdapter::ReleaseAddress(const nn::nex::StationURL& url)
{
    // nothing to do
}

// 0x00383D90 slot 0x0C | fefates:callseq
void nn::nex::TransportAdapter::ResolveAddress(nn::nex::CallContext* pContext, nn::nex::StationURL* pURL)
{
    pContext->InitiateCall();
    pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_SUCCESS), true);
}

// 0x00383DD8 slot 0x18 (name is ours)
bool nn::nex::TransportAdapter::IsReady()
{
    return true;
}

// 0x00383DE0 slot 0x2C | mk7dlp:bytes
bool nn::nex::TransportAdapter::SetLocalAddress(const nn::nex::String& address)
{
    m_LocalAddress.SetAddress(address.m_pString);
    return true;
}

// 0x00383DF8 slot 0x28 | fefates:bytes
bool nn::nex::TransportAdapter::SetNetworkInterfaceInfo(nn::nex::InterfaceInfo* pInfo)
{
    m_LocalAddress.m_SockAddr.m_Address = pInfo->m_Address;
    m_BroadcastAddress.m_SockAddr.m_Address = pInfo->m_BroadcastAddress;
    m_Unknown0x28.SetAddress(L"0.0.0.0");
    m_Unknown0x28.m_SockAddr.m_Port = __builtin_bswap16(0);
    return true;
}

// 0x00383E40 slot 0x14 | fefates:callseq
void nn::nex::TransportAdapter::SortAndFilterTargetURLs(const nn::nex::qProtectedList<nn::nex::StationURL>& localURLs, nn::nex::qList<nn::nex::StationURL>* pTargetURLs)
{
    qList<StationURL> urls;
    for (qList<StationURL>::Node* pNode = localURLs.m_List.GetFirst(); !localURLs.m_List.IsEnd(pNode); pNode = pNode->m_pNext) {
        urls.PushBackInline(pNode->m_Value);
    }
    StationContactInfo local(urls);
    StationContactInfo target(*pTargetURLs);
    local.SortAndFilterTarget(target);
    *pTargetURLs = target.m_URLs;
}

// 0x00383F4C slot 0x20
void nn::nex::TransportAdapter::Load()
{
    // nothing to do
}

// 0x00383F50 slot 0x24
void nn::nex::TransportAdapter::Unload()
{
    // nothing to do
}

// 0x0072B778 slot 0x1C (name is ours)
u32 nn::nex::TransportAdapter::GetAddress(const nn::nex::StationURL& url)
{
    InetAddress address;
    address.SetAddress(url.GetAddress().m_pString);
    return address.GetAddress();
}

} // namespace nex
} // namespace nn
