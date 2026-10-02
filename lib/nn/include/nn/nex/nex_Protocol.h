#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponent.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex8ProtocolE @ 0x008CF708
class Protocol : public ::nn::nex::SystemComponent
{
public:
    struct _Command { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Protocol(); // ctor address unknown
    void RetrieveCallContext(nn::nex::Message*); // 0x00391B54 | fefates:bytes [tier B]
    void AddMethodID(nn::nex::Message*, unsigned); // 0x003D5A30 | mk7dlp:bytes [tier A]
    void AddProtocolKey(nn::nex::Message*, nn::nex::Protocol::_Command, unsigned short); // 0x003D5A5C | fefates:bytes [tier B]
    void AddCallResponse(nn::nex::Message*, nn::nex::CallProtocolMethodOperation*, unsigned int, const nn::nex::Message&); // 0x003D5AE8 | fefates:bytes [tier B]
    void ExtractMethodID(nn::nex::Message*); // 0x003D5BBC | mk7dlp:callseq-callee [tier A]
    void ExtractCallOutcome(nn::nex::Message*, nn::nex::qResult*); // 0x003D5C00 | fefates:bytes [tier B]
    void ExtractProtocolKey(nn::nex::Message*, nn::nex::Protocol::_Command&, unsigned short&); // 0x003D5D08 | fefates:bytes [tier B]
    void RegisterCallContext(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003D5DBC | mk7dlp:callseq-callee [tier A]
    void FlagIsSet(unsigned int) const; // 0x0072E9F0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
