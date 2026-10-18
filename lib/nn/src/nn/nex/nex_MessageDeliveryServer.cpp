#include "nn/nex/nex_MessageDeliveryServer.h"

namespace nn {
namespace nex {
// 0x0039A5D0 slot 0x5C
void nn::nex::MessageDeliveryServer::DeliverMessage(nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage)
{
    ScopedCS lock(Scheduler::GetInstance()->m_Lock);
    if (m_pHandler != nullptr) {
        m_pHandler->DeliverMessage(oUserMessage);
    }
}

// 0x0039A678 slot 0x00
// 0x0039A624 (deleting dtor)
nn::nex::MessageDeliveryServer::~MessageDeliveryServer()
{
    SetHandler(nullptr);
}

// 0x0072CE0C slot 0x4C
nn::nex::Protocol* nn::nex::MessageDeliveryServer::Clone()
{
    return new MessageDeliveryServer();
}

} // namespace nex
} // namespace nn
