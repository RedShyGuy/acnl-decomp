#include "nn/nex/nex_SecureConnectionClient.h"
#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_ConnectionManager.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StreamManager.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/nex/nex_qBuffer.h"

namespace nn {
namespace nex {
// 0x0039DB10 | fefates:bytes [tier B]
bool nn::nex::SecureConnectionClient::ReplaceURL(nn::nex::ProtocolCallContext* pContext, const nn::nex::StationURL& target, const nn::nex::StationURL& url)
{
    if (target.GetPortNumber() == 0 || url.GetPortNumber() == 0) {
        return false;
    }
    if (!Network::GetInstance()->ReplaceLocalURL(target, url)) {
        return false;
    }
    return m_Protocol.CallReplaceURL(pContext, target, url);
}

// 0x0039DB7C | fefates:bytes [tier B]
void nn::nex::SecureConnectionClient::SendReport(unsigned int reportId, const void* pData, unsigned int size)
{
    if (pData == nullptr || size == 0) {
        return;
    }
    qBuffer data(0, 0);
    data.push_back(static_cast<const u8*>(pData), size);
    // (the data was logged)
    StringStream stream;
    noshowbase(stream);
    zerohex(stream);
    for (u32 i = 0; i < data.size(); i++) {
        stream.StreamNumber(data.m_pBegin[i]);
    }
    stream.GetLength();
    ProtocolCallContext context;
    m_Protocol.CallSendReport(&context, reportId, data);
}

// 0x0039DC74 | fefates:callgraph [tier C]
void nn::nex::SecureConnectionClient::PrepareURLs(nn::nex::qList<nn::nex::StationURL>& urls)
{
    Network::GetInstance()->GetStationURLs(urls);
    StreamManager* pStreamManager = m_pDefaultCredentials != nullptr ? m_pDefaultCredentials->m_pStreamManager : nullptr;
    ConnectionOrientedStream* pStream = pStreamManager->m_pConnectionManager->m_StreamTable.FindStream(const_cast<wchar_t*>(L"prudp"));
    for (qList<StationURL>::Node* pNode = urls.GetFirst(); !urls.IsEnd(pNode); pNode = pNode->m_pNext) {
        StationURL& url = pNode->m_Value;
        url.SetStreamID(pStream->m_Port);
        url.SetNATMapping(0);
        url.SetNATFiltering(0);
        url.SetUPnPSupport(0);
        url.SetNatPMPSupport(0);
        url.SetPlatformType();
        Network::GetInstance()->UpdateStationURL(url);
    }
}

// 0x0039DDBC slot 0x20
void nn::nex::SecureConnectionClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    m_Protocol.SetDefaultCredentials(pCredentials);
}

// 0x0039DDF4 slot 0x00
// 0x0039DDC4 (deleting dtor)
nn::nex::SecureConnectionClient::~SecureConnectionClient()
{
    AnyDataAdapter::Unregister();
}

} // namespace nex
} // namespace nn
