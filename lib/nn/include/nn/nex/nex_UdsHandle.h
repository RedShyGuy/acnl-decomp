#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class UdsHandle
{
public:
    // (the mk7dlp bytes match 0x00425F64, but that is pia::local::UdsHandle::UdsHandle)
    UdsHandle();
};
} // namespace nex
} // namespace nn
