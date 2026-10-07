#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19ProtocolCallContextE @ 0x008CE7FC
// vtable 0x008FD704 (vptr 0x008FD70C), offset_to_top 0, 5 entries
class ProtocolCallContext : public ::nn::nex::CallContext
{
public:
    virtual ~ProtocolCallContext(); // 0x00394A98 slot 0x00 | fefates:callgraph
    // 0x00394A88 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void BeginTransition(nn::nex::CallContext::State, nn::nex::qResult, bool); // 0x003947F4 slot 0x0C | fefates:bytes
    void SetCredentials(nn::nex::Credentials*); // 0x00394768 | fefates:bytes [tier B]
    ProtocolCallContext(); // 0x00394A28 | mk7dlp:callseq-callee [tier A]

    // (the size is from the constructor; the names are ours)
    u32 m_Unknown0x60[5];          // 0x60
    Credentials* m_pCredentials;   // 0x74
};
ASSERT_SIZE(ProtocolCallContext, 0x78);
} // namespace nex
} // namespace nn
