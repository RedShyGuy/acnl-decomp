#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class RegexDfaState
{
public:
    void Initialize(nn::ngc::ProfanityFilterTemporaryPool*, unsigned int, bool); // 0x003DC2B8 | fefates:bytes [tier B]
    RegexDfaState(); // 0x003DC350 | fefates:bytes [tier B]
};
} // namespace ngc
} // namespace nn
