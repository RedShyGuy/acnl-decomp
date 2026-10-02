#include "nn/nex/nex_SystemComponent.h"
#include "nn/nex/nex_Protocol.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::Protocol::Protocol()
{
}

// 0x00391B54 | fefates:bytes [tier B]
void nn::nex::Protocol::RetrieveCallContext(nn::nex::Message*)
{
}

// 0x003D5A30 | mk7dlp:bytes [tier A]
void nn::nex::Protocol::AddMethodID(nn::nex::Message*, unsigned)
{
}

// 0x003D5A5C | fefates:bytes [tier B]
void nn::nex::Protocol::AddProtocolKey(nn::nex::Message*, nn::nex::Protocol::_Command, unsigned short)
{
}

// 0x003D5AE8 | fefates:bytes [tier B]
void nn::nex::Protocol::AddCallResponse(nn::nex::Message*, nn::nex::CallProtocolMethodOperation*, unsigned int, const nn::nex::Message&)
{
}

// 0x003D5BBC | mk7dlp:callseq-callee [tier A]
void nn::nex::Protocol::ExtractMethodID(nn::nex::Message*)
{
}

// 0x003D5C00 | fefates:bytes [tier B]
void nn::nex::Protocol::ExtractCallOutcome(nn::nex::Message*, nn::nex::qResult*)
{
}

// 0x003D5D08 | fefates:bytes [tier B]
void nn::nex::Protocol::ExtractProtocolKey(nn::nex::Message*, nn::nex::Protocol::_Command&, unsigned short&)
{
}

// 0x003D5DBC | mk7dlp:callseq-callee [tier A]
void nn::nex::Protocol::RegisterCallContext(nn::nex::Message*, nn::nex::ProtocolCallContext*)
{
}

// 0x0072E9F0 | fefates:bytes [tier B]
void nn::nex::Protocol::FlagIsSet(unsigned int) const
{
}

} // namespace nex
} // namespace nn
