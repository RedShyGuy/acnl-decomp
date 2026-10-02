#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class Hmac
{
public:
    void Initialize(nn::pia::common::HashContextBase*, const void*, unsigned int); // 0x00428BE0 | fefates:bytes [tier B]
    void Calc(void*, const void*, unsigned int); // 0x00428CD4 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
