#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13WorkerThreadsE @ 0x008CE2C0
// vtable 0x008FCA20 (vptr 0x008FCA28), offset_to_top 0, 5 entries
class WorkerThreads : public ::nn::nex::RootObject
{
public:
    WorkerThreads(); // ctor address unknown
    virtual ~WorkerThreads(); // 0x0036FA40 slot 0x00 | slot vf_0x00 of nn::nex::WorkerThreads
    // 0x0036FA0C slot 0x04 | slot vf_0x04 of nn::nex::WorkerThreads (deleting dtor)
    virtual void vf_0x08(); // 0x0036F478 slot 0x08 | virtual slot, introduced by nn::nex::WorkerThreads
    virtual void vf_0x0C(); // 0x0036FA08 slot 0x0C | virtual slot, introduced by nn::nex::WorkerThreads
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
};
} // namespace nex
} // namespace nn
