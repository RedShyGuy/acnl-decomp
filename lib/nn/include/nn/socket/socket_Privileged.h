#pragma once

#include "decomp.h"

namespace nn {
namespace socket {
class Privileged
{
public:
    void GetHostId(unsigned int*); // 0x00484B74 | fefates:bytes [tier B]
};
} // namespace socket
} // namespace nn
