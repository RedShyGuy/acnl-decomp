#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
class HidDevices
{
public:
    void Initialize(const char*); // 0x003525EC | nintendogs:bytes [tier A]
    ~HidDevices(); // 0x00352714 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace hid
} // namespace nn
