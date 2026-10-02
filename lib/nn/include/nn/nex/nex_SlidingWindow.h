#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class SlidingWindow
{
public:
    SlidingWindow(); // TODO: default ctor added so derived stubs compile - may not exist
    SlidingWindow(unsigned short, unsigned int); // 0x0036E1AC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
