#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16OperationManagerE @ 0x008CE5F0
// vtable 0x008FD204 (vptr 0x008FD20C), offset_to_top 0, 2 entries
class OperationManager : public ::nn::nex::RootObject
{
public:
    virtual ~OperationManager(); // 0x003831E8 slot 0x00 | fefates:bytes
    // 0x003831B8 slot 0x04 | slot vf_0x04 of nn::nex::OperationManager (deleting dtor)
    void OperationEnds(nn::nex::Operation*); // 0x00382F20 | mk7dlp:bytes [tier A]
    void OperationBegins(nn::nex::Operation*); // 0x00382FB4 | mk7dlp:bytes [tier A]
    OperationManager(); // 0x003830CC | fefates:bytes [tier B]
    void GetCurrentOperation() const; // 0x0072B75C | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
