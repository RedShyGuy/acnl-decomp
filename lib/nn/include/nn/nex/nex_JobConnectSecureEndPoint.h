#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24JobConnectSecureEndPointE @ 0x008CEDA4
// vtable 0x008FE7B4 (vptr 0x008FE7BC), offset_to_top 0, 12 entries
class JobConnectSecureEndPoint : public ::nn::nex::Job
{
public:
    JobConnectSecureEndPoint(); // ctor candidate(s) 0x003B35FC (unverified)
    virtual ~JobConnectSecureEndPoint(); // 0x003B3788 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B3778 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void Execute(); // 0x003B32A8 slot 0x0C | fefates:callseq
    virtual void CancelJob(); // 0x003B35E0 slot 0x24 | fefates:bytes
    void BuildListURLs(nn::nex::qList<nn::nex::StationURL>*); // 0x003B2A8C | mk7dlp:callseq [tier A]
    void PerformConnect(); // 0x003B2BAC | fefates:bytes-fuzzy [tier B]
    void ProcessConnectResult(); // 0x003B2D58 | fefates:bytes-fuzzy [tier B]
    void RequestConnectionData(); // 0x003B2EAC | fefates:bytes-fuzzy [tier B]
    void InitializeBufferRequest(); // 0x003B3070 | fefates:bytes [tier B]
    void ParseURL(); // 0x003B33A0 | mk7dlp:callseq [tier A]
    JobConnectSecureEndPoint(nn::nex::SecureEndPoint*, const nn::nex::StationURL*, nn::nex::Buffer*, nn::nex::Buffer*, void(*)(nn::nex::EndPoint*, nn::nex::qResult, const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned); // 0x003B35FC | mk7dlp:callseq [tier A]
};
} // namespace nex
} // namespace nn
