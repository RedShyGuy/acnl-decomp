#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30ProtocolRequestBrokerInterfaceE @ 0x008CF1D0
// vtable 0x008FF420 (vptr 0x008FF428), offset_to_top 0, 15 entries
class ProtocolRequestBrokerInterface : public ::nn::nex::RootObject
{
public:
    virtual ~ProtocolRequestBrokerInterface(); // 0x003C3164 slot 0x00 | fefates:callseq
    virtual void vf_0x04(); // 0x003C313C slot 0x04 | fefates:callseq
    virtual void ProcessMessage(nn::nex::CallProtocolMethodOperation*, nn::nex::EndPoint*, nn::nex::Buffer*); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void ProcessCallRequest(unsigned short, nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void ProcessCallResponse(unsigned short, nn::nex::Message*, nn::nex::EndPoint*); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void RegisterProtocol(nn::nex::Protocol*); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void UnregisterProtocol(nn::nex::Protocol*); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    // the protocol with the id (names and signatures of these two are ours)
    virtual nn::nex::Protocol* FindProtocol(unsigned short id) = 0; // slot 0x2C
    // a call of the client protocol processed by the server protocol of this station
    virtual nn::nex::qResult ProcessLocalCall(nn::nex::Message* pMessage, nn::nex::Protocol* pClient, nn::nex::Protocol* pServer, u32 unknown) = 0; // slot 0x30
    virtual void vf_0x34(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x38(); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    ProtocolRequestBrokerInterface(); // 0x003C30A4 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
