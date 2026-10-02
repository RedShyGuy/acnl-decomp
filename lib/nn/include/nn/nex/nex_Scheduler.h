#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex9SchedulerE @ 0x008CF798
// vtable 0x008FFDA8 (vptr 0x008FFDB0), offset_to_top 0, 2 entries
class Scheduler : public ::nn::nex::RootObject
{
public:
    class SchedulerWorkerThread;
    Scheduler(); // ctor address unknown
    virtual ~Scheduler(); // 0x003D987C slot 0x00 | slot vf_0x00 of nn::nex::Scheduler
    // 0x003D984C slot 0x04 | slot vf_0x04 of nn::nex::Scheduler (deleting dtor)
    void PreventBlockCall(bool, unsigned int, bool, bool (*)(nn::nex::RootObject*,unsigned int), nn::nex::RootObject*, bool*); // 0x003D80B0 | fefates:bytes-fuzzy [tier B]
    void PreventRegularBlockCall(bool, unsigned int, bool (*)(nn::nex::RootObject*,unsigned int), nn::nex::RootObject*, nn::nex::qResult*); // 0x003D87C0 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
