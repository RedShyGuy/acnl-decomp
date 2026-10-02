#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_UserMessage.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11UserMessageE @ 0x008CE0A4
// vtable 0x008FC490 (vptr 0x008FC498), offset_to_top 0, 9 entries
class UserMessage : public ::nn::nex::_DDL_UserMessage
{
public:
    UserMessage(); // ctor address unknown
    virtual void vf_0x00(); // 0x0035C418 slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~UserMessage(); // 0x0035C3F0 slot 0x04 | slot vf_0x04 of nn::nex::Data
    virtual void vf_0x20(); // 0x0035C3C0 slot 0x20 | virtual slot, introduced by nn::nex::UserMessage
};
} // namespace nex
} // namespace nn
