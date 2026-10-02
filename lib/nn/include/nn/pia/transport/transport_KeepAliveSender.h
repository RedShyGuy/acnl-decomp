#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class KeepAliveSender
{
public:
    void SetInterval(int); // 0x00450054 | fefates:bytes [tier B]
    void Update(unsigned int, const nn::pia::common::Time&); // 0x0045006C | fefates:bytes [tier B]
    KeepAliveSender(); // 0x00450160 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
