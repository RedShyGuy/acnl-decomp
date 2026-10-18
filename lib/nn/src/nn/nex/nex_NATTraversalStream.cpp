#include "nn/nex/nex_NATTraversalStream.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_NATTraversalEngine.h"
#include "nn/nex/nex_StationURL.h"

namespace nn {
namespace nex {
// 0x00390170 slot 0x14 | mk7dlp:callseq
void nn::nex::NATTraversalStream::ProcessPayload(const nn::nex::InetAddress* pAddress, unsigned char sourceType, unsigned char sourcePort, nn::nex::Buffer* pPayload,
                                                 const nn::nex::Time& time, unsigned short port)
{
    StationURL url;
    url.SetInetAddress(pAddress);
    url.SetParamValue(L"stream", sourceType);
    url.SetParamValue(L"sid", sourcePort);
    if (m_pEngine != nullptr) {
        m_pEngine->ProcessProbe(url, pPayload->m_pData + pPayload->m_HeaderSize, pPayload->m_ContentSize);
    }
    m_ReceivedCount++;
}

// 0x00390210 slot 0x0C
void nn::nex::NATTraversalStream::DoWork()
{
    if (m_pEngine != nullptr) {
        m_pEngine->DoWork();
    }
}

// 0x003902A8 slot 0x00
// 0x00390224 (deleting dtor)
nn::nex::NATTraversalStream::~NATTraversalStream()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
