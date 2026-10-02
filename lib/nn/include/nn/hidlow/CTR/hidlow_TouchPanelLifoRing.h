#pragma once

#include "decomp.h"

namespace nn {
namespace hidlow {
namespace CTR {
class TouchPanelLifoRing
{
public:
    void ReadData(nn::hid::CTR::TouchPanelStatus*, int, int*, long long*, int*); // 0x004845BC | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace hidlow
} // namespace nn
