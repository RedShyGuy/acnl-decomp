#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_Protocol.h"
#include "nn/nex/nex_ServerProtocol.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003744E4 (unverified)
nn::nex::ServerProtocol::ServerProtocol()
{
}

// 0x0037E478 slot 0x00 | fefates:callgraph
nn::nex::ServerProtocol::~ServerProtocol()
{
}

// 0x0072B350 slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::ServerProtocol::vf_0x08()
{
}

// 0x0072B378 slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::ServerProtocol::vf_0x0C()
{
}

// 0x003744DC slot 0x10 | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::ServerProtocol::vf_0x10()
{
}

// 0x003D5D8C slot 0x20 | fefates:bytes
void nn::nex::ServerProtocol::BeginInitialization()
{
}

// 0x003D5BD0 slot 0x28 | fefates:bytes
void nn::nex::ServerProtocol::BeginTermination()
{
}

// 0x0072B348 slot 0x40 | slot vf_0x40 of nn::nex::ServerProtocol
void nn::nex::ServerProtocol::GetProtocolType() const
{
}

// 0x003D5DF8 slot 0x44 | virtual slot, introduced by nn::nex::ClientProtocol
void nn::nex::ServerProtocol::vf_0x44()
{
}

// 0x003D5A48 slot 0x48 | virtual slot, introduced by nn::nex::ClientProtocol
void nn::nex::ServerProtocol::vf_0x48()
{
}

// 0x0072E9E8 slot 0x4C | virtual slot, introduced by nn::nex::ClientProtocol
void nn::nex::ServerProtocol::vf_0x4C()
{
}

// 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
void nn::nex::ServerProtocol::DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*)
{
}

// 0x003744E0 slot 0x54 | slot vf_0x54 of nn::nex::ServerProtocol
void nn::nex::ServerProtocol::DispatchProtocolMessageWithAttemptCount(unsigned int, nn::nex::Message*, nn::nex::Message*, bool*, int*, nn::nex::EndPoint*)
{
}

// 0x003744D4 slot 0x58 | slot vf_0x58 of nn::nex::ServerProtocol
void nn::nex::ServerProtocol::UseAttemptCountMethod()
{
}

// 0x00374230 | mk7dlp:callseq-callee [tier A]
void nn::nex::ServerProtocol::SetCallError(nn::nex::qResult)
{
}

// 0x003744E4 | fefates:bytes [tier B]
nn::nex::ServerProtocol::ServerProtocol(unsigned int)
{
}

} // namespace nex
} // namespace nn
