#pragma once

#include "decomp.h"
#include "nn/nex/nex_Protocol.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// The server side of a protocol: it dispatches the calls to the methods (in the derived protocol)
// with the SecurityContext of the caller. The names of the members are ours.
// RTTI N2nn3nex14ServerProtocolE @ 0x008CE3D0
// vtable 0x008FCD04 (vptr 0x008FCD0C), offset_to_top 0, 23 entries
class ServerProtocol : public ::nn::nex::Protocol
{
public:
    ServerProtocol(); // TODO: default ctor added so derived stubs compile - does not exist
    explicit ServerProtocol(unsigned int flags); // 0x003744E4 | fefates:bytes [tier B]
    virtual ~ServerProtocol(); // 0x0037E478 slot 0x00 | fefates:callgraph [tier C]
    // 0x00374558 slot 0x04 (deleting dtor)
    // (inline in the original, with copies out of line)
    virtual const wchar_t* GetType() const; // 0x0072B350 slot 0x08
    virtual bool IsAKindOf(const wchar_t* pType) const; // 0x0072B378 slot 0x0C
    virtual void vf_0x10(); // 0x003744DC slot 0x10
    virtual _Type GetProtocolType() const; // 0x0072B348 slot 0x40
    virtual void DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint) = 0; // slot 0x50
    virtual void DispatchProtocolMessageWithAttemptCount(unsigned int callId, nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, int* pAttemptCount, nn::nex::EndPoint* pEndPoint); // 0x003744E0 slot 0x54
    virtual bool UseAttemptCountMethod(); // 0x003744D4 slot 0x58

    // the result of the current call
    void SetCallError(nn::nex::qResult result); // 0x00374230 | mk7dlp:callseq-callee [tier A]
    // dispatches a call with the SecurityContext of the end point
    void DispatchCallRequest(unsigned int callId, nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, int* pAttemptCount, nn::nex::EndPoint* pEndPoint); // 0x00374264 | fefates:callseq [tier C]
};
ASSERT_SIZE(ServerProtocol, 0x40);
} // namespace nex
} // namespace nn
