#pragma once

#include "decomp.h"
#include "nn/nex/nex_UserMessage.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16_DDL_TextMessageE @ 0x008CE650
// vtable 0x008FD2D8 (vptr 0x008FD2E0), offset_to_top 0, 9 entries
class _DDL_TextMessage : public ::nn::nex::UserMessage
{
public:
    _DDL_TextMessage(); // ctor address unknown
    virtual void vf_0x00(); // 0x00384288 slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~_DDL_TextMessage(); // 0x00384248 slot 0x04 | slot vf_0x04 of nn::nex::Data
    virtual void Clone() const; // 0x0072B950 slot 0x08 | slot vf_0x08 of nn::nex::Data
    virtual void GetDataType() const; // 0x0072B908 slot 0x0C | slot vf_0x0C of nn::nex::Data
    virtual void vf_0x10(); // 0x0072B928 slot 0x10 | virtual slot, introduced by nn::nex::Data
    virtual void vf_0x14(); // 0x0072BB0C slot 0x14 | virtual slot, introduced by nn::nex::Data
    virtual void StreamIn(nn::nex::Message*) const; // 0x0072BA3C slot 0x18 | slot vf_0x18 of nn::nex::Data
    virtual void vf_0x1C(); // 0x00384120 slot 0x1C | virtual slot, introduced by nn::nex::Data
};
} // namespace nex
} // namespace nn
