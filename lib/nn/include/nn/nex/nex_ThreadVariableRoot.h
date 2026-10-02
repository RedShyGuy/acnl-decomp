#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18ThreadVariableRootE @ 0x008CE760
// vtable 0x008FD588 (vptr 0x008FD590), offset_to_top 0, 5 entries
class ThreadVariableRoot : public ::nn::nex::RootObject
{
public:
    ThreadVariableRoot(); // ctor candidate(s) 0x003906C8 (unverified)
    virtual ~ThreadVariableRoot(); // 0x003907AC slot 0x00 | slot vf_0x00 of nn::nex::ThreadVariableRoot
    // 0x00390764 slot 0x04 | slot vf_0x04 of nn::nex::ThreadVariableRoot (deleting dtor)
    virtual void vf_0x08(); // 0x003906C4 slot 0x08 | virtual slot, introduced by nn::nex::ThreadVariableRoot
    virtual void vf_0x0C(); // 0x00390698 slot 0x0C | virtual slot, introduced by nn::nex::ThreadVariableRoot
    virtual void vf_0x10(); // 0x00390694 slot 0x10 | virtual slot, introduced by nn::nex::ThreadVariableRoot
};
} // namespace nex
} // namespace nn
