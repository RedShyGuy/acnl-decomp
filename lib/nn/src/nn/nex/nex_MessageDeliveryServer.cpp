#include "nn/nex/nex__Proto_MessageDeliveryProtocolServer.h"
#include "nn/nex/nex_MessageDeliveryServer.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::MessageDeliveryServer::MessageDeliveryServer()
{
}

// 0x0039A678 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::MessageDeliveryServer::~MessageDeliveryServer()
{
}

// 0x0072CE0C slot 0x4C | virtual slot, introduced by nn::nex::ServerProtocol
void nn::nex::MessageDeliveryServer::vf_0x4C()
{
}

// 0x003CC248 slot 0x50 | mk7dlp:callseq
void nn::nex::MessageDeliveryServer::DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*)
{
}

// 0x0039A5D0 slot 0x5C | virtual slot, introduced by nn::nex::MessageDeliveryServer
void nn::nex::MessageDeliveryServer::vf_0x5C()
{
}

} // namespace nex
} // namespace nn
