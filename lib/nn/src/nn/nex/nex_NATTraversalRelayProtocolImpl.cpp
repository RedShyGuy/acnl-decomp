#include "nn/nex/nex_NATTraversalRelayProtocolImpl.h"
#include "nn/nex/nex_NATRelayInterface.h"
#include "nn/nex/nex_NATTraversalRelayClient.h"

namespace nn {
namespace nex {
// 0x003C11E0 slot 0x60 | fefates:bytes
void nn::nex::NATTraversalRelayProtocolImpl::InitiateProbe(const nn::nex::StationURL& url)
{
    if (m_pOwner != nullptr) {
        m_pOwner->m_pRelayInterface->RequestProbe(url);
    }
}

// 0x003C1254 slot 0x00
// 0x003C1200 (deleting dtor)
nn::nex::NATTraversalRelayProtocolImpl::~NATTraversalRelayProtocolImpl()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
