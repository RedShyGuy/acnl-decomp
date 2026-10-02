#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalUpdateSessionMessageE @ 0x008CFCC4
// vtable 0x00901170 (vptr 0x00901178), offset_to_top 0, 4 entries
class LocalUpdateSessionMessage : public ::nn::pia::local::LocalMessage
{
public:
    LocalUpdateSessionMessage(); // ctor candidate(s) 0x00420B28 (unverified)
    virtual void vf_0x00(); // 0x00420B5C slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x00420B58 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x00420AAC slot 0x08 | fefates:callseq
    virtual void ParseMessageHeader(); // 0x00420A50 slot 0x0C | fefates:bytes
};
} // namespace local
} // namespace pia
} // namespace nn
