#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16JobManageAccountE @ 0x008CE59C
// vtable 0x008FD12C (vptr 0x008FD134), offset_to_top 0, 13 entries
class JobManageAccount : public ::nn::nex::StepSequenceJob
{
public:
    JobManageAccount(); // ctor address unknown
    virtual ~JobManageAccount(); // 0x003820F8 slot 0x00 | fefates:bytes
    // 0x003820E8 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void CompleteJob(); // 0x00380FD4 | fefates:bytes [tier B]
    void CallCreateAccount(); // 0x003810AC | fefates:bytes [tier B]
    void CallUpdateAccount(); // 0x003811FC | fefates:bytes [tier B]
    void PasswordInitialHash(); // 0x00381484 | fefates:bytes [tier B]
    void CallCustomCreateAccount(); // 0x0038159C | fefates:bytes [tier B]
    void CallNintendoCreateAccount(); // 0x0038182C | fefates:bytes [tier B]
    void CallAccountManagementCommand(); // 0x00381988 | fefates:bytes [tier B]
    void CallCreateAccountWithCustomData(); // 0x00381D04 | fefates:bytes [tier B]
    void Cancel(); // 0x00381E54 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
