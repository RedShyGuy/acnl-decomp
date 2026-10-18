#include "nn/nex/nex_JobProcessProtocolEvent.h"

namespace nn {
namespace nex {
// 0x003B113C slot 0x00
// 0x003B1100 (deleting dtor)
nn::nex::JobProcessProtocolEvent::~JobProcessProtocolEvent()
{
    if (!ProtocolRequestBrokerInterface::s_IsEndPointNotReferenced) {
        m_pEndPoint->Close();
    }
}

// 0x003B10FC slot 0x30 (name is ours)
void nn::nex::JobProcessProtocolEvent::vf_0x30()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
