#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local15LocalAckMessageE @ 0x008CFAF8
// vtable 0x00900904 (vptr 0x0090090C), offset_to_top 0, 4 entries
class LocalAckMessage : public ::nn::pia::local::LocalMessage
{
public:
    LocalAckMessage(); // ctor candidate(s) 0x004169F4 (unverified)
    virtual void vf_0x00(); // 0x00416A28 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x00416A24 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x004169A0 slot 0x08 | fefates:bytes
    virtual void ParseMessageHeader(); // 0x00416954 slot 0x0C | fefates:bytes
};
} // namespace local
} // namespace pia
} // namespace nn
