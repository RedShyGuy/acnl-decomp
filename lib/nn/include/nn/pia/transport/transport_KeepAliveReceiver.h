#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class KeepAliveReceiver
{
public:
    void Update(unsigned int, const nn::pia::common::Time&); // 0x0045419C | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
