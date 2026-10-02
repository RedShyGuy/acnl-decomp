#pragma once

#include "decomp.h"

namespace nn {
namespace socket {
namespace detail {
// RTTI N2nn6socket6detail11SessionPoolE @ 0x008D04F8
// vtable 0x0090231C (vptr 0x00902324), offset_to_top 0, 1 entries
class SessionPool
{
public:
    SessionPool(); // ctor candidate(s) 0x00486C84 (unverified)
    virtual void vf_0x00(); // 0x00485A9C slot 0x00 | virtual slot, introduced by nn::socket::detail::SessionPool
    void SemiFinalize(); // 0x00485888 | fefates:bytes [tier B]
    void AddNewSession(); // 0x00485994 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace socket
} // namespace nn
