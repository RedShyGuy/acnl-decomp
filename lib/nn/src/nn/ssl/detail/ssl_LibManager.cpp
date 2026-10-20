#include "nn/ssl/detail/ssl_LibManager.h"

namespace nn {
namespace ssl {
namespace detail {

// the object of ssl_CommonImpl.cpp
// 0x00AEEA58
LibManager s_LibManager;

// 0x00467384 slot 0x00 | fefates:bytes
// 0x00467348 slot 0x04 (deleting dtor)
nn::ssl::detail::LibManager::~LibManager()
{
    // nothing to do (the session closes its handle)
}

} // namespace detail
} // namespace ssl
} // namespace nn

// ARMCC's static initializer of the file: the constructors of the globals above
// 0x00798F54 (static initializer)
