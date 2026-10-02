#pragma once

#include "decomp.h"
#include "nn/socket/detail/socket_SessionPool.h"

namespace nn {
namespace socket {
namespace detail {
// RTTI N2nn6socket6detail15SessionPoolAutoE @ 0x008D0508
// vtable 0x00902338 (vptr 0x00902340), offset_to_top 0, 1 entries
class SessionPoolAuto : public ::nn::socket::detail::SessionPool
{
public:
    SessionPoolAuto(); // ctor candidate(s) 0x0079C3B0 (unverified)
    ~SessionPoolAuto(); // 0x00486C84 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace socket
} // namespace nn
