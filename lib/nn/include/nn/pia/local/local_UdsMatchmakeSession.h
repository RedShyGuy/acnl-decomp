#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMatchmakeSession.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19UdsMatchmakeSessionE @ 0x008CFBC8
// vtable 0x00900D1C (vptr 0x00900D24), offset_to_top 0, 45 entries
class UdsMatchmakeSession : public ::nn::pia::local::LocalMatchmakeSession
{
public:
    UdsMatchmakeSession(); // ctor address unknown
    virtual ~UdsMatchmakeSession(); // 0x0041AC70 slot 0x00 | slot vf_0x00 of nn::pia::session::CommonMatchmakeSession
    // 0x0041AC14 slot 0x04 | slot vf_0x04 of nn::pia::session::CommonMatchmakeSession (deleting dtor)
    virtual void vf_0x18(); // 0x0041A79C slot 0x18 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual void vf_0x1C(); // 0x0041AA20 slot 0x1C | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual void vf_0x24(); // 0x0041A98C slot 0x24 | virtual slot, introduced by nn::pia::session::CommonMatchmakeSession
    virtual void vf_0xA8(); // 0x0041AA28 slot 0xA8 | virtual slot, introduced by nn::pia::local::LocalMatchmakeSession
    virtual void vf_0xAC(); // 0x0041AAAC slot 0xAC | virtual slot, introduced by nn::pia::local::LocalMatchmakeSession
    virtual void vf_0xB0(); // 0x007311D4 slot 0xB0 | virtual slot, introduced by nn::pia::local::LocalMatchmakeSession
};
} // namespace local
} // namespace pia
} // namespace nn
