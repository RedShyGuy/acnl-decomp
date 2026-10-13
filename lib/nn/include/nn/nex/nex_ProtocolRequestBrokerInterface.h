#pragma once

#include "decomp.h"
#include "nn/nex/nex_BandwidthCounterMap.h"
#include "nn/nex/nex_OperationManager.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30ProtocolRequestBrokerInterfaceE @ 0x008CF1D0
// vtable 0x008FF420 (vptr 0x008FF428), offset_to_top 0, 15 entries
class ProtocolRequestBrokerInterface : public ::nn::nex::RootObject
{
public:
    ProtocolRequestBrokerInterface(); // 0x003C30A4 | fefates:bytes [tier B]
    virtual ~ProtocolRequestBrokerInterface(); // 0x003C3164 slot 0x00 | fefates:callseq [tier C]
    // 0x003C313C slot 0x04 (deleting dtor)
    virtual void ProcessMessage(nn::nex::CallProtocolMethodOperation*, nn::nex::EndPoint*, nn::nex::Buffer*) = 0; // slot 0x08
    virtual void ProcessCallRequest(unsigned short, nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*) = 0; // slot 0x0C
    virtual void ProcessCallResponse(unsigned short, nn::nex::Message*, nn::nex::EndPoint*) = 0; // slot 0x10
    virtual bool RegisterProtocol(nn::nex::Protocol*) = 0; // slot 0x14
    virtual void UnregisterProtocol(nn::nex::Protocol*) = 0; // slot 0x18
    virtual bool vf_0x1C() = 0; // slot 0x1C
    virtual bool vf_0x20() = 0; // slot 0x20
    // (the names and signatures of these four are ours) false if the id has a protocol
    virtual bool RegisterServerProtocol(unsigned short id, nn::nex::ServerProtocol* pProtocol) = 0; // slot 0x24
    virtual bool UnregisterServerProtocol(unsigned short id) = 0; // slot 0x28
    // the protocol with the id
    virtual nn::nex::ServerProtocol* FindProtocol(unsigned short id) = 0; // slot 0x2C
    // a call of the client protocol processed by the server protocol of this station
    virtual nn::nex::qResult ProcessLocalCall(nn::nex::Message* pMessage, nn::nex::Protocol* pClient, nn::nex::Protocol* pServer, u32 unknown) = 0; // slot 0x30
    virtual nn::nex::qResult vf_0x34() = 0; // slot 0x34
    virtual nn::nex::qResult vf_0x38() = 0; // slot 0x38

    BandwidthCounterMap m_BandwidthCounters; // 0x04 (per protocol)
    OperationManager m_OperationManager;     // 0x2C

    // (the functions of the SecurityContextManager for other code; names are ours)
    static u32 (*s_pGetCurrentCID)();
    static u32 (*s_pGetCurrentPID)();
    static bool s_AreGettersSet;
};
ASSERT_SIZE(ProtocolRequestBrokerInterface, 0x60);
} // namespace nex
} // namespace nn
