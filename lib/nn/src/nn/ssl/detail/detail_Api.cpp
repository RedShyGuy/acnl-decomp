#include "nn/ssl/detail/detail_Api.h"
#include "nn/ssl/detail/ssl_LibManager.h"

namespace nn {
namespace ssl {
namespace detail {

// 0x004673B8 | fefates:bytes [tier B]
nn::Result GenerateRandomBytes(u8* buffer, size_t size)
{
    if (s_LibManager.mInitializeCount <= 0) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return s_LibManager.mConnection.GenerateRandomBytes(buffer, size);
}

} // namespace detail
} // namespace ssl
} // namespace nn
