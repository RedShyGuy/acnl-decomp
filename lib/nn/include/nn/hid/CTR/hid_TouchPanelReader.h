#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
class TouchPanelReader
{
public:
    void ReadLatest(nn::hid::CTR::TouchPanelStatus*); // 0x00353834 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace hid
} // namespace nn
