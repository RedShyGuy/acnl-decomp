#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex29JobTicketManagerLoginWithDataE @ 0x008CF130
// vtable 0x008FF210 (vptr 0x008FF218), offset_to_top 0, 13 entries
class JobTicketManagerLoginWithData : public ::nn::nex::StepSequenceJob
{
public:
    JobTicketManagerLoginWithData(); // ctor address unknown
    virtual ~JobTicketManagerLoginWithData(); // 0x003C0EBC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003C0E68 slot 0x04 | fefates:bytes (deleting dtor)
    void ValidateKey(); // 0x003C09CC | fefates:bytes [tier B]
    void InitiateLogin(); // 0x003C0B24 | fefates:bytes [tier B]
    void ProcessLoginResult(); // 0x003C0C54 | fefates:bytes [tier B]
    void DeriveKey(); // 0x003C0D14 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
