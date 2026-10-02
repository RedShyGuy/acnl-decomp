#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_TextMessage.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11TextMessageE @ 0x008CE098
// vtable 0x008FC464 (vptr 0x008FC46C), offset_to_top 0, 9 entries
class TextMessage : public ::nn::nex::_DDL_TextMessage
{
public:
    TextMessage(); // ctor candidate(s) 0x0051353C (unverified)
    virtual void vf_0x00(); // 0x0035C380 slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~TextMessage(); // 0x0035C340 slot 0x04 | slot vf_0x04 of nn::nex::Data
    virtual void vf_0x20(); // 0x0035C3BC slot 0x20 | virtual slot, introduced by nn::nex::UserMessage
};
} // namespace nex
} // namespace nn
