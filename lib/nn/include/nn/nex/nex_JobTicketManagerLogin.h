#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21JobTicketManagerLoginE @ 0x008CEB0C
// vtable 0x008FDE34 (vptr 0x008FDE3C), offset_to_top 0, 13 entries
class JobTicketManagerLogin : public ::nn::nex::StepSequenceJob
{
public:
    JobTicketManagerLogin(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~JobTicketManagerLogin(); // 0x0039A4A4 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0039A448 slot 0x04 | fefates:bytes (deleting dtor)
    void ValidateKey(); // 0x00399DF4 | fefates:bytes [tier B]
    void InitiateLogin(); // 0x00399F6C | fefates:bytes [tier B]
    void ProcessLoginResult(); // 0x0039A0F0 | fefates:bytes [tier B]
    void DeriveKey(); // 0x0039A1D4 | fefates:bytes [tier B]
    JobTicketManagerLogin(unsigned, nn::nex::TicketManager*, nn::nex::qResult*, const nn::nex::String&, const char*, unsigned*, nn::nex::RVConnectionData*, nn::nex::String*, nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>*); // 0x0039A320 | mk7dlp:callseq [tier A]
};
} // namespace nex
} // namespace nn
