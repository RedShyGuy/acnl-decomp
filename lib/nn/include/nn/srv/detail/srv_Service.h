#pragma once

#include "decomp.h"

namespace nn {
namespace srv {
namespace detail {
class Service
{
public:
    void RegisterClient(); // 0x0011E4A0 | nintendogs:bytes [tier A]
    void EnableNotification(nn::Handle*); // 0x001200BC | nintendogs:bytes [tier A]
    void GetServiceHandle(nn::Handle*, const char*, int, unsigned); // 0x0012A958 | nintendogs:bytes [tier A]
};
} // namespace detail
} // namespace srv
} // namespace nn
