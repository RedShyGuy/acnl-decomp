#pragma once

#include "decomp.h"
#include "nn/nex/nex_Log.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex8TraceLogE @ 0x008CF720
// vtable 0x008FFCC8 (vptr 0x008FFCD0), offset_to_top 0, 5 entries
class TraceLog : public ::nn::nex::Log
{
public:
    TraceLog(); // ctor address unknown
    virtual ~TraceLog(); // 0x003D5FD4 slot 0x00 | slot vf_0x00 of nn::nex::Log
    virtual void vf_0x04(); // 0x003D5F58 slot 0x04 | virtual slot, introduced by nn::nex::Log
};
} // namespace nex
} // namespace nn
