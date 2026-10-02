#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class OutputCapture
{
public:
    void Write(short*, int); // 0x00461F20 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn
