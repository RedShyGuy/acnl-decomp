#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class Credentials;

// The call context of a protocol call: the pointers where the call writes its return values and
// the credentials of the call. The names of the members are ours.
// RTTI N2nn3nex19ProtocolCallContextE @ 0x008CE7FC
// vtable 0x008FD704 (vptr 0x008FD70C), offset_to_top 0, 5 entries
class ProtocolCallContext : public ::nn::nex::CallContext
{
public:
    // (the reference symbols have no parameter; this build takes the credentials)
    explicit ProtocolCallContext(nn::nex::Credentials* pCredentials = nullptr); // 0x00394A28 | mk7dlp:callseq-callee [tier A]
    virtual ~ProtocolCallContext(); // 0x00394A98 slot 0x00 | fefates:callgraph [tier C]
    // 0x00394A88 slot 0x04 (deleting dtor)
    // on a failure the result is written to m_pResult (and false to m_pIsSuccess)
    virtual void BeginTransition(nn::nex::CallContext::State state, nn::nex::qResult result, bool isSignaled); // 0x003947F4 slot 0x0C | fefates:bytes [tier B]

    void SetCredentials(nn::nex::Credentials* pCredentials); // 0x00394768 | fefates:bytes [tier B]
    // (symbols.json names this Protocol::FlagIsSet)
    void AddReturnValuePtr(void* pValue); // 0x00394888 | fefates:bytes [tier B]
    void* GetReturnValuePtr(unsigned int index); // 0x00394A1C | fefates:callgraph [tier C]

    qVector<void*> m_ReturnValues;  // 0x60
    bool* m_pIsSuccess;             // 0x6C
    qResult* m_pResult;             // 0x70
    Credentials* m_pCredentials;    // 0x74
};
ASSERT_SIZE(ProtocolCallContext, 0x78);
} // namespace nex
} // namespace nn
