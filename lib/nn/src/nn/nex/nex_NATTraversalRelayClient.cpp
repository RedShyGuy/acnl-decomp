#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_NATTraversalRelayClient.h"

namespace nn {
namespace nex {
// 0x003B160C slot 0x00 | fefates:bytes
nn::nex::NATTraversalRelayClient::~NATTraversalRelayClient()
{
}

// 0x0072D280 slot 0x14 | fefates:bytes
void nn::nex::NATTraversalRelayClient::IsConnected() const
{
}

// 0x003B1310 slot 0x1C | mk7dlp:callseq
void nn::nex::NATTraversalRelayClient::ConnectionStateHasChanged()
{
}

// 0x003B1430 slot 0x20 | fefates:bytes
void nn::nex::NATTraversalRelayClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials*)
{
}

// 0x003B13C0 slot 0x28 | fefates:bytes
void nn::nex::NATTraversalRelayClient::CreateNATTraversalRelayProtocol()
{
}

// 0x003B14A8 | fefates:bytes-fuzzy [tier B]
void nn::nex::NATTraversalRelayClient::Init()
{
}

// 0x003B1580 | fefates:bytes [tier B]
nn::nex::NATTraversalRelayClient::NATTraversalRelayClient()
{
}

} // namespace nex
} // namespace nn
