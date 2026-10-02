#pragma once

#include "decomp.h"
#include "nn/nex/nex_JobLogin.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11JobCTRLoginE @ 0x008CE024
// vtable 0x008FC2BC (vptr 0x008FC2C4), offset_to_top 0, 14 entries
class JobCTRLogin : public ::nn::nex::JobLogin
{
public:
    JobCTRLogin(); // ctor candidate(s) 0x003D6D98 (unverified)
    virtual ~JobCTRLogin(); // 0x003586D4 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x00358668 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void StepFirst(); // 0x0035857C slot 0x34 | fefates:bytes
    void InitializeEvent(); // 0x00357C40 | fefates:bytes [tier B]
    void StepWaitingForGameLogin(); // 0x00357CDC | fefates:bytes [tier B]
    void StepGetGameAuthentication(); // 0x00357DBC | fefates:bytes [tier B]
    void StepRequestAuthenticationToken(); // 0x00357F2C | fefates:bytes [tier B]
    void StepWaitingForRequestAuthenticationToken(); // 0x00358324 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
