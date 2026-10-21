#include "nn/nex/nex_JobProcessProtocolMessage.h"
#include "nn/nex/nex_Buffer.h"

namespace nn {
namespace nex {
// 0x003B6F6C slot 0x30 (name is ours)
void nn::nex::JobProcessProtocolMessage::vf_0x30()
{
    // nothing to do
}

// 0x003B6F70 slot 0x0C | fefates:bytes
void nn::nex::JobProcessProtocolMessage::Execute()
{
    m_pBroker->ProcessMessage(&m_Operation, m_pEndPoint, static_cast<Buffer*>(m_Operation.m_pMessage));
    if (m_Operation.m_Unknown0x14) {
        SetToWaiting(m_Operation.m_Unknown0x1C);
    } else {
        SetToComplete();
    }
}

// 0x003B6FBC | fefates:callgraph [tier C]
nn::nex::JobProcessProtocolMessage::JobProcessProtocolMessage(nn::nex::ProtocolRequestBrokerInterface* pBroker, nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pData)
    : JobProcessProtocolEvent(pBroker, pEndPoint)
{
    m_Operation.m_pMessage = pData;
    pData->AcquireRef();
}

// 0x003B712C slot 0x00
// 0x003B708C (deleting dtor)
nn::nex::JobProcessProtocolMessage::~JobProcessProtocolMessage()
{
    static_cast<Buffer*>(m_Operation.m_pMessage)->ReleaseRef();
}

} // namespace nex
} // namespace nn
