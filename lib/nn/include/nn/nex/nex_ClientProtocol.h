#pragma once

#include "decomp.h"
#include "nn/nex/nex_Protocol.h"

namespace nn {
namespace nex {
class Credentials;

// The client side of a protocol: it sends the calls and processes the responses (the derived
// protocol extracts the results of each method). The names of the members are ours.
// RTTI N2nn3nex14ClientProtocolE @ 0x008CE2D8
// vtable 0x008FCA54 (vptr 0x008FCA5C), offset_to_top 0, 23 entries
class ClientProtocol : public ::nn::nex::Protocol
{
public:
    explicit ClientProtocol(unsigned int flags); // 0x00370E64 | fefates:bytes [tier B]
    virtual ~ClientProtocol(); // 0x00370EF0 slot 0x00 | fefates:bytes [tier B]
    // 0x00370EDC slot 0x04 (deleting dtor)
    // (inline in the original, with copies out of line)
    virtual const wchar_t* GetType() const; // 0x0072AE64 slot 0x08
    virtual bool IsAKindOf(const wchar_t* pType) const; // 0x0072AE8C slot 0x0C
    virtual void vf_0x10(); // 0x00370E60 slot 0x10
    virtual _Type GetProtocolType() const; // 0x0072AE5C slot 0x40
    // the results of the method of a successful call
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext) = 0; // slot 0x50
    virtual nn::nex::ClientProtocol* CreateResponder() const = 0; // slot 0x54
    virtual void SetDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x00370DD4 slot 0x58 | fefates:bytes [tier B]

    // the response of a call: its outcome, then the results
    void ProcessResponse(nn::nex::Message* pMessage, nn::nex::EndPoint* pEndPoint); // 0x00370A18 | fefates:bytes [tier B]
    // the request of a call goes to the connection of the call (or the local server protocol);
    // false if it failed
    bool SendRequest(nn::nex::ProtocolCallContext* pContext, nn::nex::Message* pMessage); // 0x00370834 (name is ours)
    // a call to the server protocol of this station; false if it failed
    bool SendOverLocalLoopback(nn::nex::ProtocolCallContext* pContext, nn::nex::Message* pMessage); // 0x00370C88 | fefates:bytes [tier B]
    // (only a nop in this build)
    void SetCallError(nn::nex::qResult result); // 0x003D5828 | fefates:callgraph [tier C]

    Credentials* m_pDefaultCredentials; // 0x40
};
ASSERT_SIZE(ClientProtocol, 0x44);
} // namespace nex
} // namespace nn
