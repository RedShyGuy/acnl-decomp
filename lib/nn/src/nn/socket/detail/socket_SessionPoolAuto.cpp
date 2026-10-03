#include "nn/socket/detail/socket_SessionPoolAuto.h"

namespace nn {
namespace socket {
namespace detail {

// 0x00486C84 | fefates:bytes [tier B]
nn::socket::detail::SessionPoolAuto::~SessionPoolAuto()
{
    // nothing to do (~SessionPool finalizes the pool)
}

} // namespace detail
} // namespace socket
} // namespace nn
