#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local26LocalDestroyNetworkMessageE @ 0x008CFCD0
// vtable 0x00901188 (vptr 0x00901190), offset_to_top 0, 4 entries
class LocalDestroyNetworkMessage : public ::nn::pia::local::LocalMessage
{
public:
    LocalDestroyNetworkMessage(); // ctor candidate(s) 0x00420BEC (unverified)
    virtual void vf_0x00(); // 0x00420C20 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x00420C1C slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x00420BA0 slot 0x08 | slot vf_0x08 of nn::pia::local::LocalMessage
    virtual void ParseMessageHeader(); // 0x00420B60 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalMessage
};
} // namespace local
} // namespace pia
} // namespace nn
