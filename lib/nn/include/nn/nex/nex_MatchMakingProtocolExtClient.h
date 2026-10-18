#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
class ProtocolCallContext;
class String;

// The client of the extended match making protocol; only EndParticipation is
// called in this build. The method and parameter names are from the DDL strings.
// RTTI N2nn3nex28MatchMakingProtocolExtClientE @ 0x008CF09C
// vtable 0x008FF020 (vptr 0x008FF028), offset_to_top 0, 23 entries
class MatchMakingProtocolExtClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol
    enum Method
    {
        METHOD_END_PARTICIPATION = 1,
        METHOD_COUNT = 6
    };
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);

    // id: the protocol id (0: the responder, not registered)
    explicit DECOMP_NOINLINE MatchMakingProtocolExtClient(unsigned short id); // 0x003BDCE4
    virtual ~MatchMakingProtocolExtClient(); // 0x003BDDDC slot 0x00
    // 0x003BDDCC slot 0x04 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BDC34 slot 0x50 | fefates:bytes
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072DE20 slot 0x54

    bool CallEndParticipation(nn::nex::ProtocolCallContext* pContext, bool* pRetval, const unsigned int& idGathering, const nn::nex::String& strMessage); // 0x003BDAC8 | fefates
    static void ProtoReturn_EndParticipation(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BDCB8

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
ASSERT_SIZE(MatchMakingProtocolExtClient, 0x44);
} // namespace nex
} // namespace nn
