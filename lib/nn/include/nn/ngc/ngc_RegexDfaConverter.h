#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class RegexDfaConverter
{
public:
    void RegisterDfaState(const bool*); // 0x003DE1C4 | fefates:bytes [tier B]
    void GatherEpsilonLinks(bool*); // 0x003DE2B8 | fefates:bytes [tier B]
    RegexDfaConverter(); // 0x003DF038 | fefates:bytes [tier B]
};
} // namespace ngc
} // namespace nn
