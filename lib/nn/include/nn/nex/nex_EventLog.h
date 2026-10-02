#pragma once

#include "decomp.h"
#include "nn/nex/nex_Log.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex8EventLogE @ 0x008CF6D0
// vtable 0x008FFC4C (vptr 0x008FFC54), offset_to_top 0, 5 entries
class EventLog : public ::nn::nex::Log
{
public:
    EventLog(); // ctor address unknown
    virtual ~EventLog(); // 0x003D5000 slot 0x00 | slot vf_0x00 of nn::nex::Log
    virtual void vf_0x04(); // 0x003D4F84 slot 0x04 | virtual slot, introduced by nn::nex::Log
    virtual void vf_0x10(); // 0x003D4EBC slot 0x10 | virtual slot, introduced by nn::nex::Log
};
} // namespace nex
} // namespace nn
