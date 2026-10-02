#pragma once

#include "decomp.h"

namespace net {
namespace nex {
class Error
{
public:
    void Set(net::nex::Command::ID, unsigned char); // 0x0051B740 | libgarden [tier A]
};
} // namespace nex
} // namespace net
