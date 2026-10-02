#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex28MatchMakingProtocolExtClientE @ 0x008CF09C
// vtable 0x008FF020 (vptr 0x008FF028), offset_to_top 0, 23 entries
class MatchMakingProtocolExtClient : public ::nn::nex::ClientProtocol
{
public:
    MatchMakingProtocolExtClient(); // ctor candidate(s) 0x003BDCE4 (unverified)
    virtual ~MatchMakingProtocolExtClient(); // 0x003BDDDC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003BDDCC slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003BDC34 slot 0x50 | fefates:bytes
    virtual void CreateResponder() const; // 0x0072DE20 slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
};
} // namespace nex
} // namespace nn
